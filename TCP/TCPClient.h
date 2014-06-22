#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <thread>

typedef void(*t_OnServerSendData)(void* data, DWORD datasize);

class TCPClient
{
	SOCKET m_socket = INVALID_SOCKET;

public:
	TCPClient(const char* hostname, WORD port);
	virtual ~TCPClient();

	virtual int SendData(void* data, DWORD datasize);

private:
	void Listener();

protected:
	virtual void OnServerSendData(void* data, DWORD datasize) = 0;
};

#endif //TCPCLIENT_H