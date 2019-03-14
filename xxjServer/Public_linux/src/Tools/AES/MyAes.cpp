#include "MyAes.h"
#include "AES.h"

int CMyAesTools::Encrypt(std::string S, unsigned char *& des)
{
	int len = S.length() + 1;
	int tempLen = ((len / 16) + 1) * 16;
	des = (new unsigned char[tempLen]);
	memset(des, 0, tempLen);
	memcpy(des, S.c_str(), len);
	AES aes(g_key);
	aes.Cipher((void *)des);
	return tempLen;
}

std::string CMyAesTools::Decrypt(unsigned char *& des, int len)
{
	AES aes(g_key);
	aes.InvCipher((void *)des, len);
	std::string str((char *)des);
	return str;
}
