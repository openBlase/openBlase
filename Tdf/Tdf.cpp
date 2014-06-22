#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "Tdf.h"

Tdf::Tdf()
{

}

Tdf::~Tdf()
{

}

Tdf* Tdf::fromMemory(void* buffer, DWORD * size)
{
	TdfHeader* Header = (TdfHeader *)buffer;

	switch (Header->Type)
	{
	case TDF_INTEGER_1:
	case TDF_INTEGER_2:
	case TDF_INTEGER_3:
		return TdfInteger::fromMemory(buffer, size);
	case TDF_STRING:
		return TdfString::fromMemory(buffer, size);
	case TDF_STRUCT:
		return TdfStruct::fromMemory(buffer, size);
	case TDF_LIST:
		return TdfList::fromMemory(buffer, size);
	case TDF_DOUBLE_LIST:
		return TdfDoubleList::fromMemory(buffer, size);
	case TDF_INTEGER_LIST:
		return TdfIntegerList::fromMemory(buffer, size);
	case TDF_VECTOR2D:
		return TdfVector2D::fromMemory(buffer, size);
	case TDF_VECTOR3D:
		return TdfVector3D::fromMemory(buffer, size);
	default:
		printf("Unsupported type: %i\n", Header->Type);
	}

	return nullptr;
}

DWORD Tdf::DecompressLabel(DWORD Label)
{
	Label = _byteswap_ulong(Label) >> 8;

	DWORD ret = 0;

	for (int i = 0; i < 4; ++i)
	{
		DWORD j = (Label >> (3 - i) * 6) & 0x3F;
		if (j > 0)
			((BYTE *)&ret)[i] = (BYTE)(0x40 | (j & 0x1F));
	}

	return ret;
}

QWORD Tdf::DecompressInteger(void* data, DWORD * offset)
{
	BYTE* buff = new BYTE[8];
	memset(buff, 0, 8);

	for (int i = 0; i < 8; ++i)
	{
		buff[i] = *((BYTE *)data + *offset); ++*offset;

		if (buff[i] < 0x80)
			break;
	}

	DWORD result = buff[0] & 0x3F;

	for (int i = 1; i < 8; i++)
		result |= (DWORD)(buff[i] & 0x7F) << (i * 7 - 1);

	return result;
}
