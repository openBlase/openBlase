#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfList.h"

TdfList::TdfList() : Tdf()
{

}


TdfList::TdfList(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	//m_values = Values;
}

TdfList::~TdfList()
{

}

TdfList* TdfList::fromPacket(BlazeInStream* stream)
{
	TdfHeader Header = stream->Read<TdfHeader>();

	TdfList* ret = new TdfList();

	ret->m_label = ret->DecompressLabel(Header.CompressedLabel);
	ret->m_type = (TdfTypes)Header.Type;

	ret->m_subType = (TdfTypes)stream->Read<BYTE>();
	DWORD count = stream->ReadCompressedInt();
	for (DWORD i = 0; i < count; i++)
	{
		switch (ret->m_subType)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
		{
			ret->m_values.push_back(stream->ReadCompressedInt());
			break;
		}
		case TDF_STRING:
		{
			DWORD len = stream->ReadCompressedInt();

			char* buf = new char[len];
			memcpy(buf, stream->ReadP(len), len);

			ret->m_values.push_back((DWORD) buf);
			break;
		}
		case TDF_STRUCT:
		{
			std::vector<Tdf*>* vec = new std::vector<Tdf*>;

			while (stream->Read<BYTE>() != 0)
			{
				stream->seek(1, SEEK_BACK);
				if (stream->Read<BYTE>() != 2) //WTF?!
					stream->seek(1, SEEK_BACK);

				DWORD len = 0;
				vec->push_back(Tdf::fromPacket(stream));
			}

			ret->m_values.push_back((DWORD)vec);
			break;
		}
		default:
			printf("Unsupported subType: %i\n", ret->m_subType);
		}
	}

	return ret;
}

DWORD TdfList::toPacket(void* buffer, DWORD size)
{
	return 0;
}