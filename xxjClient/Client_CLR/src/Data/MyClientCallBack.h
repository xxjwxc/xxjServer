/********************************************************************
    创建时间:     2017/04/07  15:03:10 
    文件名:       MyClientDataModel.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.与服务器的数据接口的数据回调接口
                2.
                
    说明:        1. 
                2. 
*********************************************************************/
#ifndef MY_CLIENTCALLBACK_H_
#define MY_CLIENTCALLBACK_H_
#include "GameDefine.h"

namespace MyClientCallBack {
	public delegate void OnVoidCallBack();//空函数返回定义
	public delegate void OnENetStateCallBack(ENetState e);//网络状态改变回调

	public ref class CMyClientCallBack
	{

	public:
		//OnVoidCallBack^ theConnectCallBack = nullptr;
		OnENetStateCallBack^ theENetStateCallBack = nullptr;
	public:
		//网络状态改变回调
		inline void OnSetENetStateCallBack(OnENetStateCallBack^ e) { theENetStateCallBack = e;}
		inline void OnDealENetStateCallBack(ENetState e)
		{
			if (theENetStateCallBack)
				theENetStateCallBack(e);
		}

	};
}


#endif
