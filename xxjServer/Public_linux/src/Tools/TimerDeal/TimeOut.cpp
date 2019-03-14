#include "TimeOut.h"

void CTimeOut::SetTimeOut(MY_UINT32 time_out)
{
	m_TimeOut = time_out;
	m_nowTick = 0;
	m_isInit = true;
}

void CTimeOut::OnTimeCallBack()
{
	assert(m_isInit && "must call SetTimeOut first.");
	if (++m_nowTick >= m_TimeOut)
	{
		m_nowTick = 0;
		this->OnTimeOut();
	}
}


