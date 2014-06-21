#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <WinSock2.h>

#include "TCPServer.h"

TCPServer::TCPServer(WORD port)
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

	m_Taccepter = new std::thread(&TCPServer::Accepter, this);
	m_Taccepter->detach();

	m_Tlistener = new std::thread(&TCPServer::Listener, this);
	m_Tlistener->detach();
}

TCPServer::~TCPServer()
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
}

char* TCPServer::getClientIP(DWORD cid)
{
	SOCKADDR_IN clientaddr;
	int len = sizeof(clientaddr);
	getpeername(m_clients[cid], (sockaddr*)&clientaddr, &len);
	return inet_ntoa(clientaddr.sin_addr);
}

WORD TCPServer::getClientPort(DWORD cid)
{
	SOCKADDR_IN clientaddr;
	int len = sizeof(clientaddr);
	getpeername(m_clients[cid], (sockaddr*)&clientaddr, &len);
	return ntohs(clientaddr.sin_port);
}

int TCPServer::SendClientData(DWORD cid, void* data, DWORD datasize)
{
	return send(m_clients[cid], (char*) data, datasize, 0);
}

void TCPServer::Accepter()
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

		m_isClientInitalised[m_clients.size()] = false;
		m_clients.push_back(socket);

		OnClientConnect(m_clients.size() - 1);

		m_isClientInitalised[m_clients.size() - 1] = true;
	}
}

void TCPServer::Listener()
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
			if (m_isClientInitalised[i])
				FD_SET(m_clients[i], &read_set);

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
			if (FD_ISSET(m_clients[i], &read_set))
			{
				char * rbuf = new char[10240];
				memset(rbuf, 0, 10240);
				int recved = recv(m_clients[i], rbuf, 10240, 0);

				if (recved <= 0)
				{
					OnClientDisconnect(i);

					closesocket(m_clients[i]);

					m_clients.erase(m_clients.begin() + i, m_clients.begin() + i + 1); //pop(i); -.-"

					continue;
				}

				OnClientSendData(i, rbuf, recved);

				delete[] rbuf;
			}
		}
	}
}