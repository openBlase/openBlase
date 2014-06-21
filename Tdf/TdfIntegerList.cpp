#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfIntegerList.h"

TdfIntegerList::TdfIntegerList() : Tdf()
{

}


TdfIntegerList::TdfIntegerList(DWORD Label, TdfTypes Type, std::vector<QWORD> Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	m_values = Values;
}

TdfIntegerList::~TdfIntegerList()
{

}

TdfIntegerList* TdfIntegerList::fromMemory(void* buffer, DWORD * size)
{
	DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)buffer; offset += sizeof(TdfHeader);

	TdfIntegerList* ret = new TdfIntegerList();

	ret->m_label = ret->DecompressLabel(Header->CompressedLabel);
	ret->m_type = (TdfTypes)Header->Type;

	DWORD count = (DWORD)ret->DecompressInteger(buffer, &offset);
	for (QWORD i = 0; i < count; i++)
		ret->m_values.push_back(ret->DecompressInteger(buffer, &offset));

	if (size)
		*size = offset;

	return ret;
}

DWORD TdfIntegerList::toMemory(void* buffer, DWORD size)
{
	return 0;
}