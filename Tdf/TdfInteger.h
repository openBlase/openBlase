#ifndef TDFINTEGER_H
#define TDFINTEGER_H

#include "Tdf.h"

class TdfInteger : public Tdf
{
private:
	QWORD m_value = 0;

public:
	TdfInteger();
	TdfInteger(DWORD Label, TdfTypes Type, QWORD Value);
	~TdfInteger();

	static TdfInteger* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	QWORD getValue() { return m_value; }
	void setValue(QWORD Value) { m_value = Value; }
};

#endif //TDFINTEGER_H