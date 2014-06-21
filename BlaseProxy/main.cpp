#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#pragma comment(lib, "ws2_32.lib")

#include <SSLClient.h>
#include <SSLServer.h>

#pragma pack(push, 1)

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

#pragma pack(pop)

void hexDump(void *addr, int len)
{
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				printf("  %s\n", buff);

			// Output the offset.
			printf("%08x ", i);
		}

		// Now the hex code for the specific character.
		printf(" %02x", pc[i]);

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		printf("   ");
		i++;
	}

	// And print the final ASCII bit.
	printf("  %s\n", buff);
}

char* DecompressLabel(DWORD Tag)
{
	char* res = new char[5];

	res[0] = ((Tag & 0x30) >> 2);
	res[0] |= ((Tag & 0x0C) >> 2);
	res[0] |= ((Tag & 0x40) >> 2);
	res[0] |= ((Tag & 0x80) >> 1);

	res[1] = ((Tag & 0xFF& 0x02) << 5);
	res[1] |= ((Tag & 0xFF& 0x01) << 4);
	res[1] |= (((Tag >> 8) & 0xF0) >> 4);

	res[2] = (((Tag >> 8) & 0x08) << 3);
	res[2] |= (((Tag >> 8) & 0x04) << 2);
	res[2] |= (((Tag >> 8) & 0x03) << 2);
	res[2] |= (((Tag >> 16) & 0xC0) >> 6);

	res[3] = (((Tag >> 16) & 0x20) << 1);
	res[3] |= (((Tag >> 16) & 0x1F));

	res[4] = '\0';

	return res;
}

typedef unsigned __int64       QWORD;

QWORD DecompressInteger(void* data, DWORD & offset)
{
	BYTE* buff = new BYTE[8];
	memset(buff, 0, 8);
	
	for (int i = 0; i < 8; ++i)
	{
		buff[i] = *((BYTE *)data + offset); ++offset;

		if (buff[i] < 0x80)
			break;
	}

	int currshift = 6;
	DWORD result = buff[0] & 0x3F;

	for (int i = 1; i < 8; i++)
	{
		result |= (DWORD)(buff[i] & 0x7F) << currshift;
		currshift += 7;
	}

	return result;
}

#pragma pack(push, 1)

struct TdfHeader
{
	DWORD CompressedLabel : 24;
	DWORD Type : 8; //Fuck 3 byte vars -.-" ¡(^^^)
};

#pragma pack(pop)

enum TdfTypes : BYTE
{
	TDF_INTEGER_1,
	TDF_STRING,
	TDF_INTEGER_2,
	TDF_STRUCT,
	TDF_LIST,
	TDF_DOUBLE_LIST,
	TDF_INTEGER_3,
	TDF_INTEGER_LIST,
	TDF_VECTOR2D,
	TDF_VECTOR3D,
};

DWORD LogTdf(void * data, char* spacer = "\0")
{
	DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)((BYTE *)data + offset); offset += sizeof(TdfHeader);

	printf("%s%s (%i): ", spacer, DecompressLabel(Header->CompressedLabel), Header->Type);

	switch (Header->Type)
	{
	case TDF_INTEGER_1:
	case TDF_INTEGER_2:
	case TDF_INTEGER_3:
	{
		printf("%X\n", DecompressInteger(data, offset));
		break;
	}
	case TDF_STRING:
	{
		DWORD len = (DWORD)DecompressInteger(data, offset);
		
		char* buf = new char[len];
		memcpy(buf, (BYTE *)data + offset, len);
		offset += len;

		printf("%s (len: %i)\n", buf, len);

		delete [] buf;
		break;
	}
	case TDF_STRUCT:
	{
		printf("\n");

		char* new_spacer = new char[strlen(spacer) + 2];
		sprintf_s(new_spacer, strlen(spacer) + 2, "%s\t", spacer);

		while (*((BYTE *)data + offset) != 0)
		{
			if (*((BYTE *)data + offset) == 2) //WTF?!
				++offset;

			offset += LogTdf((BYTE *)data + offset, new_spacer);
		}
		++offset;

		delete[] new_spacer;
		break;
	}
	case TDF_LIST:
	{
		BYTE subType = *((BYTE *)data + offset); ++offset;
		QWORD count = DecompressInteger(data, offset);
		for (QWORD i = 0; i < count; i++)
		{
			switch (subType)
			{
			case TDF_INTEGER_1:
			case TDF_INTEGER_2:
			case TDF_INTEGER_3:
			{
				if (i == count - 1)
					printf("%X", DecompressInteger(data, offset));
				else
					printf("%X, ", DecompressInteger(data, offset));
				break;
			}
			case TDF_STRING:
			{
				DWORD len = (DWORD)DecompressInteger(data, offset);

				char* buf = new char[len];
				memcpy(buf, (BYTE *)data + offset, len);
				offset += len;

				if (i == count - 1)
					printf("%s (len: %i)", buf, len);
				else
					printf("%s (len: %i), ", buf, len);

				delete[] buf;
				break;
			}
			default:
				printf("Unsupported subType: %i\n", subType);
			}
		}
		break;
	}
	case TDF_DOUBLE_LIST:
	{
		BYTE subType1 = *((BYTE *)data + offset); ++offset;
		BYTE subType2 = *((BYTE *)data + offset); ++offset;
		QWORD count = DecompressInteger(data, offset);
		for (QWORD i = 0; i < count; i++)
		{
			switch (subType1)
			{
			case TDF_INTEGER_1:
			case TDF_INTEGER_2:
			case TDF_INTEGER_3:
			{
				if (i == count - 1)
					printf("%X", DecompressInteger(data, offset));
				else
					printf("%X, ", DecompressInteger(data, offset));
				break;
			}
			case TDF_STRING:
			{
				DWORD len = (DWORD)DecompressInteger(data, offset);

				char* buf = new char[len];
				memcpy(buf, (BYTE *)data + offset, len);
				offset += len;

				if (i == count - 1)
					printf("%s (len: %i)", buf, len);
				else
					printf("%s (len: %i), ", buf, len);

				delete[] buf;
				break;
			}
			default:
				printf("Unsupported subType1: %i\n", subType1);
			}

			switch (subType2)
			{
			case TDF_INTEGER_1:
			case TDF_INTEGER_2:
			case TDF_INTEGER_3:
			{
				if (i == count - 1)
					printf("%X", DecompressInteger(data, offset));
				else
					printf("%X, ", DecompressInteger(data, offset));
				break;
			}
			case TDF_STRING:
			{
				DWORD len = (DWORD)DecompressInteger(data, offset);

				char* buf = new char[len];
				memcpy(buf, (BYTE *)data + offset, len);
				offset += len;

				if (i == count - 1)
					printf("%s (len: %i)", buf, len);
				else
					printf("%s (len: %i), ", buf, len);

				delete[] buf;
				break;
			}
			default:
				printf("Unsupported subType2: %i\n", subType2);
			}
		}
		break;
	}
	case TDF_INTEGER_LIST:
	{
		QWORD count = DecompressInteger(data, offset);

		for (QWORD i = 0; i < count; i++)
			if (i == count - 1)
				printf("%X", DecompressInteger(data, offset));
			else
				printf("%X, ", DecompressInteger(data, offset));

		printf("\n");

		break;
	}
	case TDF_VECTOR2D:
	{
		printf("%X, %X\n", DecompressInteger(data, offset), DecompressInteger(data, offset));
		break;
	}
	case TDF_VECTOR3D:
	{
		printf("%X, %X, %X\n", DecompressInteger(data, offset), DecompressInteger(data, offset), DecompressInteger(data, offset));
		break;
	}
	default:
		printf("Unsupported type: %i\n", Header->Type);
		break;
	}

	return offset;
}

void LogBlasePacket(void* data)
{
	BlazePacketHeader BPH;
	memset(&BPH, 0, sizeof(BlazePacketHeader));

	DWORD offset = 0;

	BPH.Length = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.Component = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.Command = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.Error = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.QType = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	BPH.ID = _byteswap_ushort(*(WORD *)((BYTE *)data + offset)); offset += sizeof(WORD);
	/*if ((BPH.QType & 0x10) == 0)
		BPH.extLength = 0;
	else
		BPH.extLength = _byteswap_ushort(*(WORD *)((char *)data + offset)); offset += sizeof(WORD);*/

	DWORD len = BPH.Length + (BPH.extLength << 16);

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

	while (offset < len - 4)
		offset += LogTdf(pTdfs + offset);

	hexDump(pTdfs, len);
}

static SSLServer * g_server = nullptr;

class BlazeClient : public SSLClient
{
public:
	BlazeClient(const char* hostname, WORD port) : SSLClient(hostname, port) { }
	virtual ~BlazeClient() { }

protected:
	virtual void OnServerSendData(void* data, DWORD datasize)
	{
		printf("Server sent data! (len: %i)\n", datasize);

		LogBlasePacket(data);

		g_server->SendClientData(cid, data, datasize);
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

		LogBlasePacket(data);

		m_blaseClients[cid]->SendData(data, datasize);
	}

};

int main(int argc, char* argv[])
{
	WSADATA wsadata;
	int stertres = WSAStartup(MAKEWORD(2, 2), &wsadata);
	assert(stertres == 0);

	SSL_library_init();

	g_server = new BlazeServer(42127);

	while (true); //main loop

	delete g_server;

	int cleanres = WSACleanup();
	assert(cleanres == 0);

	system("pause");

	return 0;
}

