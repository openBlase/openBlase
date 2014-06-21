#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfVector2D.h"

TdfVector2D::TdfVector2D() : Tdf()
{

}


TdfVector2D::TdfVector2D(DWORD Label, TdfTypes Type, QWORD x, QWORD y) : Tdf()
{
	m_label = Label;
	m_type = Type;
	m_x = x;
	m_y = y;
}

TdfVector2D::~TdfVector2D()
{

}

TdfVector2D* TdfVector2D::fromMemory(void* buffer, DWORD * size)
{
	DWORD offset = 0;

	TdfHeader* Header = (TdfHeader *)buffer; offset += sizeof(TdfHeader);

	TdfVector2D* ret = new TdfVector2D();

	ret->m_label = ret->DecompressLabel(Header->CompressedLabel);
	ret->m_type = (TdfTypes)Header->Type;
	ret->m_x = ret->DecompressInteger(buffer, &offset);
	ret->m_y = ret->DecompressInteger(buffer, &offset);

	if (size)
		*size = offset;

	return ret;
}

DWORD TdfVector2D::toMemory(void* buffer, DWORD size)
{
	return 0;
}