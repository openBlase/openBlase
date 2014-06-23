#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <thread>

class SSLClient
{
	SSL_CTX *m_ssl_ctx = nullptr;
	SSL *m_ssl = nullptr;

public:
	SSLClient(const char* hostname, WORD port);
	virtual ~SSLClient();

	virtual int SendData(void* data, DWORD datasize);

private:
	void Listener();

protected:
	virtual void OnServerSendData(void* data, DWORD datasize) = 0;
};

#endif //SSLCLIENT_H