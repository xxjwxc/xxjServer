/********************************************************************
    创建时间：    2016/10/07  18:36:10 
    文件名：      PointBase.h
    作者：        谢小军
    
    功能：        1.指针操作符工具
                 2.
                
    说明：        1. 
                 2. 
*********************************************************************/

#ifndef  _POINTBASE_H_
#define  _POINTBASE_H_


template <class T>
class CPointBase
{
public:
	T & operator*() const  // never throws
	{
		assert(px != 0);
		return *px;
	}

	T * operator&() const  // never throws
	{
		assert(px != 0);
		return px;
	}

	T * operator->() const  // never throws
	{
		assert(px != 0);
		return px;
	}

	bool IsNull()
	{
		return px == nullptr;
	}
protected:
	T * px = nullptr;
};

#endif