#ifndef TDFLIST_H
#define TDFLIST_H

#include <vector>

#include "Tdf.h"

class TdfList : public Tdf
{
private:
	TdfTypes m_subType = TDF_INVALID;
	std::vector<DWORD> m_values;

public:
	TdfList();
	TdfList(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values);
	~TdfList();

	static TdfList* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	virtual TdfTypes getSubType() { return m_subType; }
	virtual void setSubType(TdfTypes SubType) { m_subType = SubType; }

	template<typename T>
	std::vector<T> getValues()
	{
		return *(std::vector<T>*)&m_values;
	}
	/*template<typename T>
	void setValues(T Values)
	{
		switch (m_subType)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
			m_values_int = Values;
			break;
		case TDF_STRING:
			m_values_string = Values;
			break;
		case TDF_STRUCT:
			m_values_struct = Values;
			break;
		}
	}*/
};

#endif //TDFLIST_H