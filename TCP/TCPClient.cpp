#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <assert.h>

#include "TCPClient.h"

TCPClient::TCPClient(const char* hostname, WORD port)
{
	struct hostent *host = gethostbyname(hostname);
	assert(host != nullptr);

	m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(m_socket != INVALID_SOCKET);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);

	int connect_res = connect(m_socket, (struct sockaddr*)&addr, sizeof(addr));
	assert(connect_res == 0);

	//OnConneced....

	std::thread listener(&TCPClient::Listener, this);
	listener.detach();
}

TCPClient::~TCPClient()
{
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

int TCPClient::SendData(void* data, DWORD datasize)
{
	return send(m_socket, (char*)data, datasize, 0);
}

void TCPClient::Listener()
{
	while (this != nullptr && m_socket != INVALID_SOCKET)
	{
		char * rbuf = new char[10240];
		memset(rbuf, 0, 10240);
		int recved = recv(m_socket, rbuf, 10240, 0);

		if (recved <= 0)
		{
			/*if (m_pOnClientDisconnectCallback)
			m_pOnClientDisconnectCallback(i);*/

			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
			break;
		}

		OnServerSendData(rbuf, recved);

		delete[] rbuf;
	}
}