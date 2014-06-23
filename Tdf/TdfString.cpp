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
	if (m_value)
		delete [] m_value;
}

TdfString* TdfString::fromPacket(BlazeInStream* stream)
{
	TdfHeader Header = stream->Read<TdfHeader>();

	TdfString* ret = new TdfString();

	ret->m_label = ret->DecompressLabel(Header.CompressedLabel);
	ret->m_type = (TdfTypes)Header.Type;
	DWORD len = stream->ReadCompressedInt();

	ret->m_value = new char[len];

	memcpy(ret->m_value, stream->ReadP(len), len);

	return ret;
}

DWORD TdfString::toPacket(void* buffer, DWORD size)
{
	return 0;
}