#ifndef BLAZEINSTREAM_HPP
#define BLAZEINSTREAM_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>

#ifndef SEEK_SET
#define SEEK_SET    0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR    1
#endif
#ifndef SEEK_END
#define SEEK_END    2
#endif
#ifndef SEEK_BACK
#define SEEK_BACK    3
#endif

class BlazeInStream
{
	BYTE* m_buffer = nullptr;
	DWORD m_size = 0;
	DWORD m_offset = 0;

public:
	BlazeInStream(void* buffer, DWORD size)
	{
		m_buffer = (BYTE*) buffer;
		m_size = size;
	}
	
	virtual ~BlazeInStream()
	{

	}

	DWORD size()
	{
		return m_size;
	}

	void seek(DWORD offset, int origin = SEEK_CUR)
	{
		switch (origin)
		{
		case SEEK_SET:
			m_offset = offset;
		case SEEK_CUR:
			m_offset += offset;
		case SEEK_END:
			m_offset = m_size - offset;
		case SEEK_BACK:
			m_offset -= offset;
		}
	}

	DWORD tell()
	{
		return m_offset;
	}

	template<typename T>
	T Read()
	{
		T ret = *(T*)(m_buffer + m_offset);
		m_offset += sizeof(T);
		return ret;
	}

	BYTE* ReadP(DWORD len)
	{
		BYTE* ret = m_buffer + m_offset;
		m_offset += len;
		return ret;
	}

	virtual DWORD ReadCompressedInt()
	{
		DWORD res = Read<BYTE>() & 0x3F;

		seek(1, SEEK_BACK);

		if (Read<BYTE>() < 0x80)
			return res;

		for (int i = 1; i < 8; i++)
		{
			res |= (DWORD)(Read<BYTE>() & 0x7F) << (i * 7 - 1);

			seek(1, SEEK_BACK);

			if (Read<BYTE>() < 0x80)
				break;
		}

		return res;
	}

};

#endif //BLAZEINSTREAM_HPP