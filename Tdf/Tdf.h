#ifndef TDF_H
#define TDF_H

#include "BlazeInStream.hpp"

enum TdfTypes : BYTE
{
	TDF_INTEGER_1,
	TDF_STRING,
	TDF_INTEGER_2,
	TDF_STRUCT,
	TDF_LIST,
	TDF_DOUBLE_LIST,
	TDF_INTEGER_3,
	TDF_INTEGER_LIST,
	TDF_VECTOR2D,
	TDF_VECTOR3D,

	TDF_COUNT,

	TDF_INVALID = 0xFF,
};

#pragma pack(push, 1)

struct TdfHeader
{
	DWORD CompressedLabel : 24;
	DWORD Type : 8; //Fuck 3 byte vars -.-" (^^^)
};

#pragma pack(pop)

class Tdf
{
protected:
	DWORD m_label = 0;
	TdfTypes m_type = TDF_INVALID;

public:
	Tdf();
	virtual ~Tdf();

	static Tdf* fromPacket(BlazeInStream* stream);

	virtual DWORD toPacket(void* buffer, DWORD size) = 0;

	virtual DWORD getLabel() { return m_label; }
	virtual void setLabel(DWORD Label) { m_label = Label; }

	virtual char* getLabelString()
	{
		char* ret = new char[5];
		memcpy(ret, &m_label, 4);
		ret[4] = '\0';
		return ret;
	}
	virtual void setLabelString(const char* Label) { memcpy(&m_label, Label, 4); }

	virtual TdfTypes getType() { return m_type; }
	virtual void setType(TdfTypes Type) { m_type = Type; }

protected:
	virtual DWORD CompressLabel(DWORD Label);
	virtual DWORD DecompressLabel(DWORD Label);
	/*virtual DWORD CompressInteger(DWORD integer);
	virtual DWORD DecompressInteger(void* data, DWORD * offset);*/
};

#include "TdfInteger.h"
#include "TdfString.h"
#include "TdfStruct.h"
#include "TdfList.h"
#include "TdfDoubleList.h"
#include "TdfIntegerList.h"
#include "TdfVector2D.h"
#include "TdfVector3D.h"

#endif //TDF_H