#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <assert.h>

#include "SSLServer.h"

SSLServer::SSLServer(WORD port)
{
	m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(m_socket != INVALID_SOCKET);
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(m_socket, (struct sockaddr*)&addr, sizeof(sockaddr_in)) != 0)
	{
		printf("%i\n", WSAGetLastError());
		perror("can't bind port");
		abort();
	}

	if (listen(m_socket, 10) != 0)
	{
		perror("Can't configure listening port");
		abort();
	}

	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();

	m_ssl_ctx = SSL_CTX_new(SSLv3_server_method());
	if (m_ssl_ctx == NULL)
	{
		ERR_print_errors_fp(stderr);
		abort();
	}

	if (SSL_CTX_load_verify_locations(m_ssl_ctx, "gosredirector.ea.com.crt", "gosredirector.ea.com.key") != 1)
		ERR_print_errors_fp(stderr);

	if (SSL_CTX_set_default_verify_paths(m_ssl_ctx) != 1)
		ERR_print_errors_fp(stderr);

	if (SSL_CTX_use_certificate_file(m_ssl_ctx, "gosredirector.ea.com.crt", SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		abort();
	}

	if (SSL_CTX_use_PrivateKey_file(m_ssl_ctx, "gosredirector.ea.com.key", SSL_FILETYPE_PEM) <= 0)
	{
		ERR_print_errors_fp(stderr);
		abort();
	}

	if (!SSL_CTX_check_private_key(m_ssl_ctx))
	{
		fprintf(stderr, "Private key does not match the public certificate\n");
		abort();
	}

	m_Taccepter = new std::thread(&SSLServer::Accepter, this);
	m_Taccepter->detach();

	m_Tlistener = new std::thread(&SSLServer::Listener, this);
	m_Tlistener->detach();
}

SSLServer::~SSLServer()
{
	if (m_Taccepter != nullptr)
	{
		delete m_Taccepter;
		m_Taccepter = nullptr;
	}

	if (m_Tlistener != nullptr)
	{
		delete m_Tlistener;
		m_Tlistener = nullptr;
	}

	closesocket(m_socket);
	SSL_CTX_free(m_ssl_ctx);
}

char* SSLServer::getClientIP(DWORD cid)
{
	SOCKADDR_IN clientaddr;
	int len = sizeof(clientaddr);
	getpeername(SSL_get_fd(m_clients[cid]), (sockaddr*)&clientaddr, &len);
	return inet_ntoa(clientaddr.sin_addr);
}

WORD SSLServer::getClientPort(DWORD cid)
{
	SOCKADDR_IN clientaddr;
	int len = sizeof(clientaddr);
	getpeername(SSL_get_fd(m_clients[cid]), (sockaddr*)&clientaddr, &len);
	return ntohs(clientaddr.sin_port);
}

int SSLServer::SendClientData(DWORD cid, void* data, DWORD datasize)
{
	return SSL_write(m_clients[cid], data, datasize);
}

void SSLServer::Accepter()
{
	while (true)
	{
		SOCKADDR_IN addr;
		int len = sizeof(addr);
		SOCKET socket = accept(m_socket, (sockaddr*)&addr, &len);

		if (socket == -1)
		{
			printf("failed to accept client");
			continue;
		}

		SSL *ssl = SSL_new(m_ssl_ctx);
		SSL_set_fd(ssl, socket);

		if (SSL_accept(ssl) == -1)
		{
			ERR_print_errors_fp(stderr);
			abort();
		}

		m_isClientInitalised[m_clients.size()] = false;
		m_clients.push_back(ssl);

		OnClientConnect(m_clients.size() - 1);

		m_isClientInitalised[m_clients.size() - 1] = true;
	}
}

void SSLServer::Listener()
{
	fd_set read_set;

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	while (true)
	{
		if (m_clients.size() == 0)
			continue;

		FD_ZERO(&read_set);

		for (DWORD i = 0; i < m_clients.size(); i++)
			if (m_isClientInitalised[i] && SSL_is_init_finished(m_clients[i]))
				FD_SET(SSL_get_fd(m_clients[i]), &read_set);

		if (read_set.fd_count == 0)
			continue;

		int rv = select(NULL, &read_set, nullptr, nullptr, &timeout);

		if (rv == -1)
		{
			printf("failed to select\n");
			continue;
		}

		if (rv == 0)
			continue;

		for (DWORD i = 0; i < m_clients.size(); i++)
		{
			if (FD_ISSET(SSL_get_fd(m_clients[i]), &read_set))
			{
				char * rbuf = new char[1024];
				memset(rbuf, 0, 1024);
				int recved = SSL_read(m_clients[i], rbuf, 1024);

				if (recved <= 0)
				{
					OnClientDisconnect(i);

					//SOCKET client = SSL_get_fd(m_clients[i]);
					SSL_free(m_clients[i]);
					//closesocket(client);

					m_clients.erase(m_clients.begin() + i, m_clients.begin() + i + 1); //pop(i); -.-"

					continue;
				}

				OnClientSendData(i, rbuf, recved);

				delete[] rbuf;
			}
		}
	}
}