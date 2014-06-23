#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfVector3D.h"

TdfVector3D::TdfVector3D() : Tdf()
{

}


TdfVector3D::TdfVector3D(DWORD Label, TdfTypes Type, DWORD* Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	memcpy(m_values, Values, sizeof(DWORD) * 3);
}

TdfVector3D::~TdfVector3D()
{

}

TdfVector3D* TdfVector3D::fromPacket(BlazeInStream* stream)
{
	/*DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)buffer; offset += sizeof(TdfHeader);

	TdfVector3D* ret = new TdfVector3D();

	ret->m_label = ret->DecompressLabel(Header->CompressedLabel);
	ret->m_type = (TdfTypes)Header->Type;
	ret->m_values[0] = ret->DecompressInteger(buffer, &offset);
	ret->m_values[1] = ret->DecompressInteger(buffer, &offset);
	ret->m_values[2] = ret->DecompressInteger(buffer, &offset);

	if (size)
		*size = offset;

	return ret;*/

	return nullptr;
}

DWORD TdfVector3D::toPacket(void* buffer, DWORD size)
{
	return 0;
}