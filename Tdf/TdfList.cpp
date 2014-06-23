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
	/*DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)buffer; offset += sizeof(TdfHeader);

	TdfList* ret = new TdfList();

	ret->m_label = ret->DecompressLabel(Header->CompressedLabel);
	ret->m_type = (TdfTypes)Header->Type;

	ret->m_subType = (TdfTypes)*((BYTE *)buffer + offset); ++offset;
	DWORD count = (DWORD)ret->DecompressInteger(buffer, &offset);
	for (DWORD i = 0; i < count; i++)
	{
		switch (ret->m_subType)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
		{
			ret->m_values.push_back(ret->DecompressInteger(buffer, &offset));
			break;
		}
		case TDF_STRING:
		{
			DWORD len = (DWORD)ret->DecompressInteger(buffer, &offset);

			char* buf = new char[len];
			memcpy(buf, (BYTE *)buffer + offset, len);
			offset += len;

			ret->m_values.push_back((DWORD) buf);
			break;
		}
		case TDF_STRUCT:
		{
			std::vector<Tdf*>* vec = new std::vector<Tdf*>;

			while (*((BYTE *)buffer + offset) != 0)
			{
				if (*((BYTE *)buffer + offset) == 2) //WTF?!
					++offset;

				DWORD len = 0;
				vec->push_back(Tdf::fromPacket((BYTE *)buffer + offset, &len));
				offset += len;
			}
			++offset;

			ret->m_values.push_back((DWORD)vec);
			break;
		}
		default:
			printf("Unsupported subType: %i\n", ret->m_subType);
		}
	}

	if (size)
		*size = offset;

	return ret;*/

	return nullptr;
}

DWORD TdfList::toPacket(void* buffer, DWORD size)
{
	return 0;
}