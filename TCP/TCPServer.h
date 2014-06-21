#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <thread>
#include <vector>
#include <unordered_map>

class TCPServer
{
	SOCKET m_socket = INVALID_SOCKET;

	std::thread* m_Taccepter = nullptr;
	std::thread* m_Tlistener = nullptr;

	std::vector<SOCKET> m_clients;
	std::unordered_map<DWORD, bool> m_isClientInitalised;

public:
	TCPServer(WORD port);
	virtual ~TCPServer();

	virtual DWORD getClientCount() { return m_clients.size(); }

	virtual char* getClientIP(DWORD cid);
	virtual WORD getClientPort(DWORD cid);

	virtual int SendClientData(DWORD cid, void* data, DWORD datasize);

private:
	void Accepter();
	void Listener();

protected:
	virtual void OnClientConnect(DWORD cid) = 0;
	virtual void OnClientDisconnect(DWORD cid) = 0;
	virtual void OnClientSendData(DWORD cid, void* data, DWORD datasize) = 0;
};

#endif //TCPSERVER_H