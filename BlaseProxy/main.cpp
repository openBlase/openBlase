#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#pragma comment(lib, "ws2_32.lib")

#include <SSLClient.h>
#include <SSLServer.h>

#include <Tdf.h>

#include <TCPServer.h>
#include <TCPClient.h>

void LogTdf(Tdf* pTdf, char* spacer = "\0")
{
	printf("%s%s (%i): ", spacer, pTdf->getLabelString(), pTdf->getType());

	switch (pTdf->getType())
	{
	case TDF_INTEGER_1:
	case TDF_INTEGER_2:
	case TDF_INTEGER_3:
		printf("0x%X\n", ((TdfInteger *)pTdf)->getValue());
		break;
	case TDF_STRING:
		printf("%s\n", ((TdfString *)pTdf)->getValue());
		break;
	case TDF_STRUCT:
	{
		printf("\n");

		char* new_spacer = new char[strlen(spacer) + 2];
		sprintf_s(new_spacer, strlen(spacer) + 2, "%s\t", spacer);

		for (DWORD i = 0; i < ((TdfStruct *)pTdf)->getValues().size(); ++i)
			LogTdf(((TdfStruct *)pTdf)->getValues()[i], new_spacer);

		delete[] new_spacer;
		break;
	}
	case TDF_LIST:
	case TDF_DOUBLE_LIST:
	{
		printf("TODO..\n");
		break;
	}
	case TDF_INTEGER_LIST:
	{
		for (DWORD i = 0; i < ((TdfIntegerList *)pTdf)->getValues().size(); ++i)
			if (i == ((TdfIntegerList *)pTdf)->getValues().size() - 1)
				printf("0x%X\n", ((TdfIntegerList *)pTdf)->getValues()[i]);
			else
				printf("0x%X, ", ((TdfIntegerList *)pTdf)->getValues()[i]);

		break;
	}
	case TDF_VECTOR2D:
	{
		DWORD * vals = ((TdfVector2D *)pTdf)->getValues();
		printf("{0x%X, 0x%X}\n", vals[0], vals[1]);
		break;
	}
	case TDF_VECTOR3D:
	{
		DWORD * vals = ((TdfVector3D *)pTdf)->getValues();
		printf("{0x%X, 0x%X, 0x%X}\n", vals[0], vals[1], vals[2]);
		break;
	}
	default:
		printf("Unsupported type: %i - LogTdf\n", pTdf->getType());
	}
}

struct BlazePacketHeader
{
	WORD Length;
	WORD Component;
	WORD Command;
	WORD Error;
	WORD QType;
	WORD ID;
	WORD extLength;
};

void hexDump(void *addr, int len)
{
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	for (i = 0; i < len; i++)
	{
		if ((i % 16) == 0)
		{
			if (i != 0)
				printf("  %s\n", buff);

			printf("%08x ", i);
		}

		printf(" %02x", pc[i]);

		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	while ((i % 16) != 0)
	{
		printf("   ");
		i++;
	}

	printf("  %s\n", buff);
}

void LogBlasePacket(void* data, DWORD datasize)
{
	if (datasize < sizeof(WORD) * 6)
	{
		printf("LogBlasePacket - Packet too small!\n");
		return;
	}

	BlazePacketHeader BPH;
	memset(&BPH, 0, sizeof(BlazePacketHeader));

	DWORD offset = 0;

	BPH.Length = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.Component = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.Command = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.Error = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.QType = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.ID = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	if ((BPH.QType & 0x10) == 0)
		BPH.extLength = 0;
	else
	{
		BPH.extLength = _byteswap_ushort(*(WORD *)((char *)data + offset)); offset += sizeof(WORD);
	}

	DWORD len = BPH.Length + (BPH.extLength << 16);

	if (len > datasize - offset)
		printf("\n\n\nWARNING!!!! len: %i datasize - offset: %i\n\n\n", len, datasize - offset);

	BYTE * pTdfs = new BYTE[len];
	memcpy(pTdfs, (char *)data + offset, len);

	printf("Length: %i\n", BPH.Length);
	printf("Component: %i\n", BPH.Component);
	printf("Command: %i\n", BPH.Command);
	printf("Error: %i\n", BPH.Error);
	printf("QType: %i\n", BPH.QType);
	printf("ID: %i\n", BPH.ID);
	printf("extLength: %i\n", BPH.extLength);
	printf("len: %i\n", len);
	printf("Tdfs:\n");

	offset = 0;
	while (offset < len)
	{
		DWORD offset2 = 0;

		Tdf* pTdf = Tdf::fromMemory((BYTE*)pTdfs + offset, &offset2);

		if (!pTdf)
			break;

		LogTdf(pTdf);

		offset += offset2;
	}
}

static SSLServer * g_BlazeServer = nullptr;

class BlazeClient : public SSLClient
{
public:
	BlazeClient(const char* hostname, WORD port) : SSLClient(hostname, port) { }
	virtual ~BlazeClient() { }

protected:
	virtual void OnServerSendData(void* data, DWORD datasize)
	{
		printf("Server sent data! (len: %i)\n", datasize);

		hexDump(data, datasize);
		LogBlasePacket(data, datasize);

		g_BlazeServer->SendClientData(cid, data, datasize);
	}

public:
	DWORD cid = 0;
};

class BlazeServer : public SSLServer
{
	std::unordered_map<DWORD, BlazeClient*> m_blaseClients;

public:
	BlazeServer(WORD port) : SSLServer(port) { }

protected:
	void OnClientConnect(DWORD cid)
	{
		printf("client connected! %s:%i\n", getClientIP(cid), getClientPort(cid));

		m_blaseClients[cid] = new BlazeClient("gosredirector.ea.com", 42127);
		m_blaseClients[cid]->cid = cid;
	}

	void OnClientDisconnect(DWORD cid)
	{
		printf("client disconnected! %s:%i\n", getClientIP(cid), getClientPort(cid));

		delete m_blaseClients[cid];
	}

	void OnClientSendData(DWORD cid, void* data, DWORD datasize)
	{
		printf("Client sent data! (len: %i)\n", datasize);

		hexDump(data, datasize);
		LogBlasePacket(data, datasize);

		m_blaseClients[cid]->SendData(data, datasize);
	}
};

int main(int argc, char* argv[])
{
	WSADATA wsadata;
	int startres = WSAStartup(MAKEWORD(2, 2), &wsadata);
	assert(startres == 0);

	SSL_library_init();

	g_BlazeServer = new BlazeServer(42127);

	while (true); //main loop

	delete g_BlazeServer;

	int cleanres = WSACleanup();
	assert(cleanres == 0);

	system("pause");

	return 0;
}

