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
	TdfHeader Header = stream->Read<TdfHeader>();

	TdfIntegerList* ret = new TdfIntegerList();

	ret->m_label = ret->DecompressLabel(Header.CompressedLabel);
	ret->m_type = (TdfTypes)Header.Type;

	DWORD count = stream->ReadCompressedInt();
	for (DWORD i = 0; i < count; i++)
		ret->m_values.push_back(stream->ReadCompressedInt());

	return ret;

	return nullptr;
}

DWORD TdfIntegerList::toPacket(void* buffer, DWORD size)
{
	return 0;
}