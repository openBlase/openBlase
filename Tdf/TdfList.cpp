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

TdfList* TdfList::fromMemory(void* buffer, DWORD * size)
{
	DWORD offset = 0;

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
			if (i == count - 1)
				printf("%X", ret->DecompressInteger(buffer, &offset));
			else
				printf("%X, ", ret->DecompressInteger(buffer, &offset));
			break;
		}
		case TDF_STRING:
		{
			DWORD len = (DWORD)ret->DecompressInteger(buffer, &offset);

			char* buf = new char[len];
			memcpy(buf, (BYTE *)buffer + offset, len);
			offset += len;

			if (i == count - 1)
				printf("%s (len: %i)", buf, len);
			else
				printf("%s (len: %i), ", buf, len);

			delete[] buf;
			break;
		}
		case TDF_STRUCT:
		{
			while (*((BYTE *)buffer + offset) != 0)
			{
				if (*((BYTE *)buffer + offset) == 2) //WTF?!
					++offset;

				DWORD len = 0;

				Tdf::fromMemory((BYTE *)buffer + offset, &len);

				offset += len;
			}
			++offset;
			break;
		}
		default:
			printf("Unsupported subType: %i\n", ret->m_subType);
		}
	}











	if (size)
		*size = offset;

	return ret;
}

DWORD TdfList::toMemory(void* buffer, DWORD size)
{
	return 0;
}