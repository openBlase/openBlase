#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#include "TdfStruct.h"

TdfStruct::TdfStruct() : Tdf()
{

}


TdfStruct::TdfStruct(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values) : Tdf()
{
	m_label = Label;
	m_type = Type;
	m_values = Values;
}

TdfStruct::~TdfStruct()
{

}

TdfStruct* TdfStruct::fromPacket(BlazeInStream* stream)
{
	TdfHeader Header = stream->Read<TdfHeader>();

	TdfStruct* ret = new TdfStruct();

	ret->m_label = ret->DecompressLabel(Header.CompressedLabel);
	ret->m_type = (TdfTypes)Header.Type;

	while (stream->Read<BYTE>() != 0)
	{
		stream->seek(1, SEEK_BACK);
		if (stream->Read<BYTE>() != 2) //WTF?!
			stream->seek(1, SEEK_BACK);

		ret->m_values.push_back(Tdf::fromPacket(stream));
	}

	return ret;
}

DWORD TdfStruct::toPacket(void* buffer, DWORD size)
{
	return 0;
}