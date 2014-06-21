#ifndef TDFINTEGERLIST_H
#define TDFINTEGERLIST_H

#include <vector>

#include "Tdf.h"

class TdfIntegerList : public Tdf
{
private:
	std::vector<QWORD> m_values;

public:
	TdfIntegerList();
	TdfIntegerList(DWORD Label, TdfTypes Type, std::vector<QWORD> Values);
	~TdfIntegerList();

	static TdfIntegerList* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	std::vector<QWORD> getValue() { return m_values; }
	void setValue(std::vector<QWORD> Values) { m_values = Values; }
};

#endif //TDFINTEGERLIST_H