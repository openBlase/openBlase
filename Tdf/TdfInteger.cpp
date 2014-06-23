#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfInteger.h"

TdfInteger::TdfInteger() : Tdf()
{

}


TdfInteger::TdfInteger(DWORD Label, TdfTypes Type, DWORD Value) : Tdf()
{
	m_label = Label;
	m_type = Type;
	m_value = Value;
}

TdfInteger::~TdfInteger()
{

}

TdfInteger* TdfInteger::fromPacket(BlazeInStream* stream)
{
	TdfHeader Header = stream->Read<TdfHeader>();

	TdfInteger* ret = new TdfInteger();

	ret->m_label = ret->DecompressLabel(Header.CompressedLabel);
	ret->m_type = (TdfTypes) Header.Type;
	ret->m_value = stream->ReadCompressedInt();

	return ret;
}

DWORD TdfInteger::toPacket(void* buffer, DWORD size)
{
	return 0;
}