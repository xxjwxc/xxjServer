/********************************************************************
	创建时间：	2014/04/13 10: 01: 43
	文件名：		FileInfo.h
	作者：		谢小军
	
	功能：		1. 工具类
				2.
				
	说明：	    1. 
				2. 
*********************************************************************/
#ifndef MYTOOLS_H_
#define MYTOOLS_H_

namespace MyTools {

	inline void* memset(void *src, int c, size_t count)
	{
		if (src)
		{
			char *tmpsrc = (char*)src;
			while (count--)
				*tmpsrc++ = (char)c;
		}

		return src;
	}
	inline void *memcpy(void *s1, const void *s2, register size_t n)
	{
		char *p1 = (char *)s1;
		const char *p2 = (const char *)s2;

		if (n) {
			n++;
			while (--n > 0) {
				*p1++ = *p2++;
			}
		}
		return s1;
	}
	 
	 inline  int atoi(const char *str)
	 {
	 	int s = 0;
	 	bool falg = false;
	 
	 	while (*str == ' ')
	 	{
	 		str++;
	 	}
	 
	 	if (*str == '-' || *str == '+')
	 	{
	 		if (*str == '-')
	 			falg = true;
	 		str++;
	 	}
	 
	 	while (*str >= '0'&&*str <= '9')
	 	{
	 		s = s * 10 + *str - '0';
	 		str++;
	 		if (s < 0)
	 		{
	 			s = 2147483647;
	 			break;
	 		}
	 	}
	 	return s*(falg ? -1 : 1);
	 }

	inline char * ArrayToChar(array<System::Byte>^ byteArray)
	{
		pin_ptr<System::Byte> p = &byteArray[0];
		unsigned char* pby = p;
		char* pch = reinterpret_cast<char*>(pby);
		return pch;
	}
	
	inline char * ListToChar(System::Collections::Generic::List<System::Byte>^ listArray)
	{
		pin_ptr<System::Byte> p = &listArray->ToArray()[0];
		unsigned char* pby = p;
		char* pch = reinterpret_cast<char*>(pby);
		return pch;
	}

	inline array<System::Byte>^ ListToArray(System::Collections::Generic::List<System::Byte>^ listArray)
	{
		return listArray->ToArray();
	}

	inline array<wchar_t>^ StringToArray(System::String^ str)
	{
		return str->ToCharArray();
	}
	//System::Text::Encoding::UTF8->GetString(bts, 0, bts->Length);

}


#endif

