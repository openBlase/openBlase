#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <assert.h>

#include "SSLClient.h"

SSLClient::SSLClient(const char* hostname, WORD port)
{
	struct hostent *host = gethostbyname(hostname);
	assert(host != nullptr);

	SOCKET client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(client != INVALID_SOCKET);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);

	int connect_res = connect(client, (struct sockaddr*)&addr, sizeof(addr));
	assert(connect_res == 0);

	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();

	m_ssl_ctx = SSL_CTX_new(SSLv3_client_method());
	if (m_ssl_ctx == nullptr)
	{
		ERR_print_errors_fp(stderr);
		abort();
	}

	m_ssl = SSL_new(m_ssl_ctx);
	SSL_set_fd(m_ssl, client);
	if (SSL_connect(m_ssl) == -1)
	{
		ERR_print_errors_fp(stderr);
		abort();
	}

	printf("Connected with %s encryption\n", SSL_get_cipher(m_ssl));

	std::thread listener(&SSLClient::Listener, this);
	listener.detach();
}

SSLClient::~SSLClient()
{
	if (m_ssl)
	{
		//SOCKET client = SSL_get_fd(m_ssl);
		SSL_free(m_ssl);
		m_ssl = nullptr;
		//closesocket(client);
	}

	/*if (m_ssl_ctx)
	{
		SSL_CTX_free(m_ssl_ctx);
		m_ssl_ctx = nullptr;
	}*/
}

int SSLClient::SendData(void* data, DWORD datasize)
{
	return SSL_write(m_ssl, data, datasize);
}

void SSLClient::Listener()
{
	while (this != nullptr && m_ssl != nullptr)
	{
		char * rbuf = new char[10240];
		memset(rbuf, 0, 10240);
		int recved = SSL_read(m_ssl, rbuf, 10240);

		if (recved <= 0)
		{
			/*if (m_pOnClientDisconnectCallback)
				m_pOnClientDisconnectCallback(i);*/

			//SOCKET client = SSL_get_fd(m_ssl);
			SSL_free(m_ssl);
			//closesocket(client);
			break;
		}

		OnServerSendData(rbuf, recved);

		delete[] rbuf;
	}
}