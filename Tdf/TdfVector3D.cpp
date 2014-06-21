#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfVector3D.h"

TdfVector3D::TdfVector3D() : Tdf()
{

}


TdfVector3D::TdfVector3D(DWORD Label, TdfTypes Type, QWORD x, QWORD y, QWORD z) : Tdf()
{
	m_label = Label;
	m_type = Type;
	m_x = x;
	m_y = y;
	m_z = z;
}

TdfVector3D::~TdfVector3D()
{

}

TdfVector3D* TdfVector3D::fromMemory(void* buffer, DWORD * size)
{
	DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)buffer; offset += sizeof(TdfHeader);

	TdfVector3D* ret = new TdfVector3D();

	ret->m_label = ret->DecompressLabel(Header->CompressedLabel);
	ret->m_type = (TdfTypes)Header->Type;
	ret->m_x = ret->DecompressInteger(buffer, &offset);
	ret->m_y = ret->DecompressInteger(buffer, &offset);
	ret->m_z = ret->DecompressInteger(buffer, &offset);

	if (size)
		*size = offset;

	return ret;
}

DWORD TdfVector3D::toMemory(void* buffer, DWORD size)
{
	return 0;
}