#ifndef TDFVECTOR3D_H
#define TDFVECTOR3D_H

#include "Tdf.h"

class TdfVector3D : public Tdf
{
private:
	DWORD m_values[3];

public:
	TdfVector3D();
	TdfVector3D(DWORD Label, TdfTypes Type, DWORD* Values);
	~TdfVector3D();

	static TdfVector3D* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	DWORD* getValues() { return m_values; }
	void setValues(DWORD* Values) { memcpy(m_values, Values, sizeof(DWORD) * 3); }
};

#endif //TDFVECTOR3D_H