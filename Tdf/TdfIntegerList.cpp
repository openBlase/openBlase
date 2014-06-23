#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfIntegerList.h"

TdfIntegerList::TdfIntegerList() : Tdf()
{

}


TdfIntegerList::TdfIntegerList(DWORD Label, TdfTypes Type, std::vector<DWORD> Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	m_values = Values;
}

TdfIntegerList::~TdfIntegerList()
{

}

TdfIntegerList* TdfIntegerList::fromPacket(BlazeInStream* stream)
{
	/*DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)buffer; offset += sizeof(TdfHeader);

	TdfIntegerList* ret = new TdfIntegerList();

	ret->m_label = ret->DecompressLabel(Header->CompressedLabel);
	ret->m_type = (TdfTypes)Header->Type;

	DWORD count = (DWORD)ret->DecompressInteger(buffer, &offset);
	for (DWORD i = 0; i < count; i++)
		ret->m_values.push_back(ret->DecompressInteger(buffer, &offset));

	if (size)
		*size = offset;

	return ret;*/

	return nullptr;
}

DWORD TdfIntegerList::toPacket(void* buffer, DWORD size)
{
	return 0;
}