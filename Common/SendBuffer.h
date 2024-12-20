#pragma once
#include "Constants.h"

class SendBuffer
{
	static constexpr int SEND_BUFF_SIZE = 1024 * 64 * 4;
public:
	void Append(const char* const source, const int pkt_size)noexcept;

	template<typename T>
	void Append(T&& pkt_)noexcept { Append((char*)&pkt_, sizeof(pkt_)); }

	const char* const GetBuff()const noexcept { return m_sendBuff; }
	char* const GetBuff()noexcept { return m_sendBuff; }

	int GetLen()const noexcept { return m_len; }
	void Clear()noexcept { m_len = 0; }
private:
	int m_len = 0;
	char m_sendBuff[SEND_BUFF_SIZE];
};