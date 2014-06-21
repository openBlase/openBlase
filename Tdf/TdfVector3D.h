#ifndef TDFVECTOR3D_H
#define TDFVECTOR3D_H

#include "Tdf.h"

class TdfVector3D : public Tdf
{
private:
	QWORD m_x = 0;
	QWORD m_y = 0;
	QWORD m_z = 0;

public:
	TdfVector3D();
	TdfVector3D(DWORD Label, TdfTypes Type, QWORD x, QWORD y, QWORD z);
	~TdfVector3D();

	static TdfVector3D* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	//QWORD getValue() { return m_value; }
	//void setValue(QWORD Value) { m_value = Value; }
};

#endif //TDFVECTOR3D_H