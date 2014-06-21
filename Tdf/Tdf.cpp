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
	BYTE* ret = new BYTE[4];

	ret[0] = ((Label & 0x30) >> 2);
	ret[0] |= ((Label & 0x0C) >> 2);
	ret[0] |= ((Label & 0x40) >> 2);
	ret[0] |= ((Label & 0x80) >> 1);

	ret[1] = ((Label & 0x02) << 5);
	ret[1] |= ((Label & 0x01) << 4);
	ret[1] |= (((Label >> 8) & 0xF0) >> 4);

	ret[2] = (((Label >> 8) & 0x08) << 3);
	ret[2] |= (((Label >> 8) & 0x04) << 2);
	ret[2] |= (((Label >> 8) & 0x03) << 2);
	ret[2] |= (((Label >> 16) & 0xC0) >> 6);

	ret[3] = (((Label >> 16) & 0x20) << 1);
	ret[3] |= (((Label >> 16) & 0x1F));

	return *(DWORD*)ret;
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

	int currshift = 6;
	DWORD result = buff[0] & 0x3F;

	for (int i = 1; i < 8; i++)
	{
		result |= (DWORD)(buff[i] & 0x7F) << currshift;
		currshift += 7;
	}

	return result;
}
