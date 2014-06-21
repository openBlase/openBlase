#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <thread>
#include <vector>
#include <unordered_map>

class SSLServer
{
	SSL_CTX *m_ssl_ctx = nullptr;
	SOCKET m_socket = NULL;

	std::thread* m_Taccepter = nullptr;
	std::thread* m_Tlistener = nullptr;

	std::vector<SSL*> m_clients;
	std::unordered_map<DWORD, bool> m_isClientInitalised;

public:
	SSLServer(WORD port);
	virtual ~SSLServer();

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

#endif //SSLSERVER_H