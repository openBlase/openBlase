#ifndef TDFSTRING_H
#define TDFSTRING_H

#include "Tdf.h"

class TdfString : public Tdf
{
private:
	char* m_value = nullptr;

public:
	TdfString();
	TdfString(DWORD Label, TdfTypes Type, const char* Value);
	~TdfString();

	static TdfString* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	char* getValue() { return m_value; }
	void setValue(const char* Value)
	{
		if (m_value)
			delete[] m_value;
		
		m_value = new char[strlen(Value)];
		strcpy_s(m_value, strlen(Value), Value);
	}
};

#endif //TDFSTRING_H