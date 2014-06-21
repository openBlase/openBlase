#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfStruct.h"

TdfStruct::TdfStruct() : Tdf()
{

}


TdfStruct::TdfStruct(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	m_values = Values;
}

TdfStruct::~TdfStruct()
{

}

TdfStruct* TdfStruct::fromMemory(void* buffer, DWORD * size)
{
	DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)buffer; offset += sizeof(TdfHeader);

	TdfStruct* ret = new TdfStruct();

	ret->m_label = ret->DecompressLabel(Header->CompressedLabel);
	ret->m_type = (TdfTypes)Header->Type;

	while (*((BYTE *)buffer + offset) != 0)
	{
		if (*((BYTE *)buffer + offset) == 2) //WTF?!
			++offset;

		DWORD len = 0;
		ret->m_values.push_back(Tdf::fromMemory((BYTE *)buffer + offset, &len));
		offset += len;
	}
	++offset;

	if (size)
		*size = offset;

	return ret;
}

DWORD TdfStruct::toMemory(void* buffer, DWORD size)
{
	return 0;
}