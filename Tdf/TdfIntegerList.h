#ifndef TDFINTEGERLIST_H
#define TDFINTEGERLIST_H

#include <vector>

#include "Tdf.h"

class TdfIntegerList : public Tdf
{
private:
	std::vector<DWORD> m_values;

public:
	TdfIntegerList();
	TdfIntegerList(DWORD Label, TdfTypes Type, std::vector<DWORD> Values);
	~TdfIntegerList();

	static TdfIntegerList* fromPacket(BlazeInStream* stream);

	virtual DWORD toPacket(void* buffer, DWORD size);

	std::vector<DWORD> getValues() { return m_values; }
	void setValues(std::vector<DWORD> Values) { m_values = Values; }
};

#endif //TDFINTEGERLIST_H