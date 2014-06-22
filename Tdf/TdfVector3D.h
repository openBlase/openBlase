#ifndef TDFVECTOR3D_H
#define TDFVECTOR3D_H

#include "Tdf.h"

class TdfVector3D : public Tdf
{
private:
	QWORD m_values[3];

public:
	TdfVector3D();
	TdfVector3D(DWORD Label, TdfTypes Type, QWORD* Values);
	~TdfVector3D();

	static TdfVector3D* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	QWORD* getValues() { return m_values; }
	void setValues(QWORD* Values) { memcpy(m_values, Values, sizeof(QWORD) * 3); }
};

#endif //TDFVECTOR3D_H