#ifndef TDFDOUBLELIST_H
#define TDFDOUBLELIST_H

#include <vector>

#include "Tdf.h"

class TdfDoubleList : public Tdf
{
private:
	TdfTypes m_subType1 = TDF_INVALID;
	TdfTypes m_subType2 = TDF_INVALID;

public:
	TdfDoubleList();
	TdfDoubleList(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values);
	~TdfDoubleList();

	static TdfDoubleList* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	//std::vector<Tdf*> getValues() { return m_values; }
	//void setValues(const char* Value) { }
};

#endif //TDFDOUBLELIST_H