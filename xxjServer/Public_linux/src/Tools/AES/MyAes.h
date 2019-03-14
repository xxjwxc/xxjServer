
/********************************************************************
    创建时间:     2017/12/04  20:33:03 
    文件名:       MyAes.h
    作者:         谢小军
    emal:        346944475@qq.com
    
    功能:        1.AES加密算法
                2.
                
    说明:        1. 
                2. 
*********************************************************************/
#include <string>

// 常量
const unsigned char g_key[] =
{
	0x2b, 0x7e, 0x15, 0x16,
	0x28, 0xae, 0xd2, 0xa6,
	0xab, 0xf7, 0x15, 0x88,
	0x09, 0xcf, 0x4f, 0x3c
};

class CMyAesTools
{
public:
	//加密
	int Encrypt(std::string S, unsigned char * & des);

	//解密
	std::string Decrypt(unsigned char *& des, int len);
};