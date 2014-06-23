#ifndef TDFINTEGER_H
#define TDFINTEGER_H

#include "Tdf.h"

class TdfInteger : public Tdf
{
private:
	DWORD m_value = 0;

public:
	TdfInteger();
	TdfInteger(DWORD Label, TdfTypes Type, DWORD Value);
	~TdfInteger();

	static TdfInteger* fromPacket(BlazeInStream* stream);

	virtual DWORD toPacket(void* buffer, DWORD size);

	DWORD getValue() { return m_value; }
	void setValue(DWORD Value) { m_value = Value; }
};

#endif //TDFINTEGER_H