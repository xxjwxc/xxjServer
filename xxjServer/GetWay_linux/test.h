#pragma once

#include <iostream>
#include <Tools/MySql/MySqlPool.h>
#include <Tools/TimerDeal/TimeOut.h>


class DealTest:public CTimeOut, public MySingleton<DealTest>
{
public:
	void OnInit();
public:
	void OnDeal();
	virtual void OnTimeOut();
};