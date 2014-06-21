#ifndef TDFLIST_H
#define TDFLIST_H

#include <vector>

#include "Tdf.h"

class TdfList : public Tdf
{
private:
	TdfTypes m_subType = TDF_INVALID;

public:
	TdfList();
	TdfList(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values);
	~TdfList();

	static TdfList* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	//std::vector<Tdf*> getValues() { return m_values; }
	//void setValues(const char* Value) { }
};

#endif //TDFLIST_H