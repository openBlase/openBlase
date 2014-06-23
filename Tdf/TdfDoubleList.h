#ifndef TDFDOUBLELIST_H
#define TDFDOUBLELIST_H

#include <vector>

#include "Tdf.h"

class TdfDoubleList : public Tdf
{
private:
	TdfTypes m_subType1 = TDF_INVALID;
	TdfTypes m_subType2 = TDF_INVALID;
	std::vector<DWORD> m_values1_int;
	std::vector<DWORD> m_values2_int;
	std::vector<char*> m_values1_string;
	std::vector<char*> m_values2_string;
	std::vector<std::vector<Tdf*>> m_values1_struct;
	std::vector<std::vector<Tdf*>> m_values2_struct;

public:
	TdfDoubleList();
	TdfDoubleList(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values);
	~TdfDoubleList();

	static TdfDoubleList* fromMemory(void* buffer, DWORD * size = nullptr);

	virtual DWORD toMemory(void* buffer, DWORD size);

	virtual TdfTypes getSubType1() { return m_subType1; }
	virtual void setSubType1(TdfTypes SubType) { m_subType1 = SubType; }

	virtual TdfTypes getSubType2() { return m_subType2; }
	virtual void setSubType2(TdfTypes SubType) { m_subType2 = SubType; }

	template<typename T>
	std::vector<T> getValues1()
	{
		switch (m_subType1)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
			return *(std::vector<T>*)&m_values1_int;
		case TDF_STRING:
			return *(std::vector<T>*)&m_values1_string;
		case TDF_STRUCT:
			return *(std::vector<T>*)&m_values1_struct;
		}
	}
	/*template<typename T>
	void setValues1(T Values)
	{
		switch (ret->m_subType)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
			m_values1_int = Values;
			break;
		case TDF_STRING:
			m_values1_string = Values;
			break;
		case TDF_STRUCT:
			m_values1_struct = Values;
			break;
		}
	}*/

	template<typename T>
	std::vector<T> getValues2()
	{
		switch (m_subType2)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
			return *(std::vector<T>*)&m_values2_int;
		case TDF_STRING:
			return *(std::vector<T>*)&m_values2_string;
		case TDF_STRUCT:
			return *(std::vector<T>*)&m_values2_struct;
		}
	}
	/*template<typename T>
	void setValues2(T Values)
	{
		switch (ret->m_subType)
		{
		case TDF_INTEGER_1:
		case TDF_INTEGER_2:
		case TDF_INTEGER_3:
			m_values2_int = Values;
			break;
		case TDF_STRING:
			m_values2_string = Values;
			break;
		case TDF_STRUCT:
			m_values2_struct = Values;
			break;
		}
	}*/
};

#endif //TDFDOUBLELIST_H