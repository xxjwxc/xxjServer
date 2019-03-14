/********************************************************************
    创建时间:     2017/04/19  20:07:40 
    文件名:       TimeOut.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.超时回调
                2.单位为秒
                
    说明:        1. 
                2. 
*********************************************************************/
#ifndef _TIME_OUT_H_
#define _TIME_OUT_H_
#include "../DefineBase.h"

class CTimeOut
{
public:
	void SetTimeOut(MY_UINT32);
public:
	//滴答时间回调
	virtual void OnTimeCallBack() final;

	//超时之后的处理函数
	virtual void OnTimeOut() = 0;
private:
	bool m_isInit = false;
	MY_UINT32 m_TimeOut = 0 ;
	MY_UINT32 m_nowTick = 0;
};

#endif // !_TIME_OUT_H_
