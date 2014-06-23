#ifndef TDFVECTOR2D_H
#define TDFVECTOR2D_H

#include "Tdf.h"

class TdfVector2D : public Tdf
{
private:
	DWORD m_values[2];

public:
	TdfVector2D();
	TdfVector2D(DWORD Label, TdfTypes Type, DWORD* Values);
	~TdfVector2D();

	static TdfVector2D* fromPacket(BlazeInStream* stream);

	virtual DWORD toPacket(void* buffer, DWORD size);

	DWORD* getValues() { return m_values; }
	void setValues(DWORD* Values) { memcpy(m_values, Values, sizeof(DWORD) * 2); }
};

#endif //TDFVECTOR2D_H