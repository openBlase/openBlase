#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfVector2D.h"

TdfVector2D::TdfVector2D() : Tdf()
{

}


TdfVector2D::TdfVector2D(DWORD Label, TdfTypes Type, QWORD* Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	memcpy(m_values, Values, sizeof(QWORD) * 2);
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
	ret->m_values[0] = ret->DecompressInteger(buffer, &offset);
	ret->m_values[1] = ret->DecompressInteger(buffer, &offset);

	if (size)
		*size = offset;

	return ret;
}

DWORD TdfVector2D::toMemory(void* buffer, DWORD size)
{
	return 0;
}