#ifndef TDFVECTOR2D_H
#define TDFVECTOR2D_H

#include "Tdf.h"

class TdfVector2D : public Tdf
{
private:
	QWORD m_x = 0;
	QWORD m_y = 0;

public:
	TdfVector2D();
	TdfVector2D(DWORD Label, TdfTypes Type, QWORD x, QWORD y);
	~TdfVector2D();

	static TdfVector2D* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	//QWORD getValue() { return m_value; }
	//void setValue(QWORD Value) { m_value = Value; }
};

#endif //TDFVECTOR2D_H