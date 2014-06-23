#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfVector2D.h"

TdfVector2D::TdfVector2D() : Tdf()
{

}


TdfVector2D::TdfVector2D(DWORD Label, TdfTypes Type, DWORD* Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	memcpy(m_values, Values, sizeof(DWORD) * 2);
}

TdfVector2D::~TdfVector2D()
{

}

TdfVector2D* TdfVector2D::fromPacket(BlazeInStream* stream)
{
	TdfHeader Header = stream->Read<TdfHeader>();

	TdfVector2D* ret = new TdfVector2D();

	ret->m_label = ret->DecompressLabel(Header.CompressedLabel);
	ret->m_type = (TdfTypes)Header.Type;

	ret->m_values[0] = stream->ReadCompressedInt();
	ret->m_values[1] = stream->ReadCompressedInt();

	return ret;
}

DWORD TdfVector2D::toPacket(void* buffer, DWORD size)
{
	return 0;
}