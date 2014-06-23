#ifndef TDFDOUBLELIST_H
#define TDFDOUBLELIST_H

#include <vector>

#include "Tdf.h"

class TdfDoubleList : public Tdf
{
private:
	TdfTypes m_subType1 = TDF_INVALID;
	TdfTypes m_subType2 = TDF_INVALID;
	std::vector<DWORD> m_values1;
	std::vector<DWORD> m_values2;

public:
	TdfDoubleList();
	TdfDoubleList(DWORD Label, TdfTypes Type, std::vector<Tdf*> Values);
	~TdfDoubleList();

	static TdfDoubleList* fromPacket(BlazeInStream* stream);

	virtual DWORD toPacket(void* buffer, DWORD size);

	virtual TdfTypes getSubType1() { return m_subType1; }
	virtual void setSubType1(TdfTypes SubType) { m_subType1 = SubType; }

	virtual TdfTypes getSubType2() { return m_subType2; }
	virtual void setSubType2(TdfTypes SubType) { m_subType2 = SubType; }

	template<typename T>
	std::vector<T> getValues1()
	{
		return *(std::vector<T>*)&m_values1;
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
		return *(std::vector<T>*)&m_values2;
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