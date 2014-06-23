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

Tdf* Tdf::fromPacket(BlazeInStream* stream)
{
	TdfHeader* Header = (TdfHeader *)stream->ReadP(0); //HACK

	switch (Header->Type)
	{
	case TDF_INTEGER_1:
	case TDF_INTEGER_2:
	case TDF_INTEGER_3:
		return TdfInteger::fromPacket(stream);
	case TDF_STRING:
		return TdfString::fromPacket(stream);
	case TDF_STRUCT:
		return TdfStruct::fromPacket(stream);
	case TDF_LIST:
		return TdfList::fromPacket(stream);
	case TDF_DOUBLE_LIST:
		return TdfDoubleList::fromPacket(stream);
	case TDF_INTEGER_LIST:
		return TdfIntegerList::fromPacket(stream);
	case TDF_VECTOR2D:
		return TdfVector2D::fromPacket(stream);
	case TDF_VECTOR3D:
		return TdfVector3D::fromPacket(stream);
	default:
		printf("Unsupported type: %i\n", Header->Type);
	}

	return nullptr;
}

DWORD Tdf::CompressLabel(DWORD Label)
{
	DWORD ret = 0;

	for (int i = 0; i < 4; ++i)
		ret |= (0x20 | ((((BYTE *)&Label)[i]) & 0x1F)) << ((3 - i) * 6);

	return _byteswap_ulong(ret) >> 8;
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

/*DWORD Tdf::CompressInteger(DWORD integer)
{
	if (integer < 0x40)
		return integer;

	DWORD result = 0x80 | (integer & 0x3F);

	for (int i = 1; i < 4; i++)
	{
		if (integer >> (i * 7 - 1) == 0)
			break;

		((BYTE *)&result)[i] = 0x80 | ((integer >> (i * 7 - 1)) & 0x7F);
	}

	return result;
}

DWORD Tdf::DecompressInteger(void* data, DWORD * offset)
{
	BYTE* buff = (BYTE *)data + *offset;

	DWORD res = buff[0] & 0x3F; ++*offset;

	if (buff[0] < 0x80)
		return res;

	for (int i = 1; i < 8; i++)
	{
		res |= (DWORD)(buff[i] & 0x7F) << (i * 7 - 1); ++*offset;
		
		if (buff[i] < 0x80)
			break;
	}

	return res;
}*/