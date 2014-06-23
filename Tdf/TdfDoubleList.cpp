#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfDoubleList.h"

TdfDoubleList::TdfDoubleList() : Tdf()
{

}


TdfDoubleList::TdfDoubleList(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	//m_values = Values;
}

TdfDoubleList::~TdfDoubleList()
{

}

TdfDoubleList* TdfDoubleList::fromPacket(BlazeInStream* stream)
{
	TdfHeader Header = stream->Read<TdfHeader>();

	TdfDoubleList* ret = new TdfDoubleList();

	ret->m_label = ret->DecompressLabel(Header.CompressedLabel);
	ret->m_type = (TdfTypes)Header.Type;

	ret->m_subType1 = (TdfTypes)stream->Read<BYTE>();
	ret->m_subType2 = (TdfTypes)stream->Read<BYTE>();
	DWORD count = stream->ReadCompressedInt();
	for (DWORD i = 0; i < count; i++)
	{
		switch (ret->m_subType1)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
		{
			ret->m_values1.push_back(stream->ReadCompressedInt());
			break;
		}
		case TDF_STRING:
		{
			DWORD len = stream->ReadCompressedInt();

			char* buf = new char[len];
			memcpy(buf, stream->ReadP(len), len);

			ret->m_values1.push_back((DWORD)buf);
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

			ret->m_values1.push_back((DWORD)vec);
			break;
		}
		default:
			printf("Unsupported subType1: %i\n", ret->m_subType1);
		}

		switch (ret->m_subType2)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
		{
			ret->m_values2.push_back(stream->ReadCompressedInt());
			break;
		}
		case TDF_STRING:
		{
			DWORD len = stream->ReadCompressedInt();

			char* buf = new char[len];
			memcpy(buf, stream->ReadP(len), len);

			ret->m_values2.push_back((DWORD)buf);
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

			ret->m_values2.push_back((DWORD)vec);
			break;
		}
		default:
			printf("Unsupported subType2: %i\n", ret->m_subType2);
		}
	}

	return ret;
}

DWORD TdfDoubleList::toPacket(void* buffer, DWORD size)
{
	return 0;
}