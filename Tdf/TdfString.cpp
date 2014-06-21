#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfString.h"

TdfString::TdfString() : Tdf()
{

}


TdfString::TdfString(DWORD Label, TdfTypes Type, const char* Value) : Tdf()
{
	m_label = Label;
	m_type = Type;
	m_value = new char[strlen(Value)];
	strcpy_s(m_value, strlen(Value), Value);
}

TdfString::~TdfString()
{

}

TdfString* TdfString::fromMemory(void* buffer, DWORD * size)
{
	DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)buffer; offset += sizeof(TdfHeader);

	TdfString* ret = new TdfString();

	ret->m_label = ret->DecompressLabel(Header->CompressedLabel);
	ret->m_type = (TdfTypes)Header->Type;
	DWORD len = (DWORD)ret->DecompressInteger(buffer, &offset);

	printf("len: %i\n", len);

	ret->m_value = new char[len];
	memcpy(ret->m_value, (BYTE*)buffer + offset, len);
	offset += len;

	if (size)
		*size = offset;

	return ret;
}

DWORD TdfString::toMemory(void* buffer, DWORD size)
{
	return 0;
}