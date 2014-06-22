#ifndef TDFVECTOR2D_H
#define TDFVECTOR2D_H

#include "Tdf.h"

class TdfVector2D : public Tdf
{
private:
	QWORD m_values[2];

public:
	TdfVector2D();
	TdfVector2D(DWORD Label, TdfTypes Type, QWORD* Values);
	~TdfVector2D();

	static TdfVector2D* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	QWORD* getValues() { return m_values; }
	void setValues(QWORD* Values) { memcpy(m_values, Values, sizeof(QWORD) * 2); }
};

#endif //TDFVECTOR2D_H