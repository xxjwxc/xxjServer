#ifndef __CHARSETCVT_H_
#define __CHARSETCVT_H_


#include  <string> 
#include  <clocale> 
using namespace std;

class CharsetCvt
{
public:
	static string ws2s(const wstring& ws)
	{
		string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C"; 
		setlocale(LC_ALL, "chs");
		const wchar_t* _Source = ws.c_str();
		size_t _Dsize = 2 * ws.size() + 1;
		char *_Dest = new char[_Dsize];
		memset(_Dest, 0, _Dsize);
		wcstombs(_Dest, _Source, _Dsize);
		string result = _Dest;
		delete[]_Dest;
		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}


	static wstring s2ws(const string& s)
	{
		setlocale(LC_ALL, "chs");
		const char* _Source = s.c_str();
		size_t _Dsize = s.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		int nret = mbstowcs(_Dest, _Source, _Dsize);
		wstring result = _Dest;
		delete[]_Dest;
		setlocale(LC_ALL, "C");

		return result;

	}



	static wstring UTF2Uni(const char* src, std::wstring &t)
	{
		if (src == NULL)
		{
			return L"";
		}

		int size_s = strlen(src);
		int size_d = size_s + 10;          //? 

		wchar_t *des = new wchar_t[size_d];
		memset(des, 0, size_d * sizeof(wchar_t));

		int s = 0, d = 0;
		bool toomuchbyte = true; //set true to skip error prefix. 

		while (s < size_s && d < size_d)
		{
			unsigned char c = src[s];
			if ((c & 0x80) == 0)
			{
				des[d++] += src[s++];
			}
			else if ((c & 0xE0) == 0xC0)  /// < 110x-xxxx 10xx-xxxx 
			{
				WCHAR &wideChar = des[d++];
				wideChar = (src[s + 0] & 0x3F) << 6;
				wideChar |= (src[s + 1] & 0x3F);

				s += 2;
			}
			else if ((c & 0xF0) == 0xE0)  /// < 1110-xxxx 10xx-xxxx 10xx-xxxx 
			{
				WCHAR &wideChar = des[d++];

				wideChar = (src[s + 0] & 0x1F) << 12;
				wideChar |= (src[s + 1] & 0x3F) << 6;
				wideChar |= (src[s + 2] & 0x3F);

				s += 3;
			}
			else if ((c & 0xF8) == 0xF0)  /// < 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx  
			{
				WCHAR &wideChar = des[d++];

				wideChar = (src[s + 0] & 0x0F) << 18;
				wideChar = (src[s + 1] & 0x3F) << 12;
				wideChar |= (src[s + 2] & 0x3F) << 6;
				wideChar |= (src[s + 3] & 0x3F);

				s += 4;
			}
			else
			{
				WCHAR &wideChar = des[d++]; /// < 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx  

				wideChar = (src[s + 0] & 0x07) << 24;
				wideChar = (src[s + 1] & 0x3F) << 18;
				wideChar = (src[s + 2] & 0x3F) << 12;
				wideChar |= (src[s + 3] & 0x3F) << 6;
				wideChar |= (src[s + 4] & 0x3F);

				s += 5;
			}
		}

		t = des;
		delete[] des;
		des = NULL;

		return t;
	}


	static int Uni2UTF(const wstring& strRes, char *utf8, int nMaxSize)
	{
		if (utf8 == NULL) {
			return -1;
		}
		int len = 0;
		int size_d = nMaxSize;


		for (wstring::const_iterator it = strRes.begin(); it != strRes.end(); ++it)
		{
			wchar_t wchar = *it;
			if (wchar < 0x80)
			{  // 
				//length = 1; 
				utf8[len++] = (char)wchar;
			}
			else if (wchar < 0x800)
			{
				//length = 2; 

				if (len + 1 >= size_d)
					return -1;

				utf8[len++] = 0xc0 | (wchar >> 6);
				utf8[len++] = 0x80 | (wchar & 0x3f);
			}
			else if (wchar < 0x10000)
			{
				//length = 3; 
				if (len + 2 >= size_d)
					return -1;

				utf8[len++] = 0xe0 | (wchar >> 12);
				utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
				utf8[len++] = 0x80 | (wchar & 0x3f);
			}
			else if (wchar < 0x200000)
			{
				//length = 4; 
				if (len + 3 >= size_d)
					return -1;

				utf8[len++] = 0xf0 | ((int)wchar >> 18);
				utf8[len++] = 0x80 | ((wchar >> 12) & 0x3f);
				utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
				utf8[len++] = 0x80 | (wchar & 0x3f);
			}

		}


		return len;
	}

	static string s2utfs(const  string&  strSrc)
	{
		string  strRes;
		wstring  wstrUni = s2ws(strSrc);

		char*  chUTF8 = new char[wstrUni.length() * 3];
		memset(chUTF8, 0x00, wstrUni.length() * 3);
		Uni2UTF(wstrUni, chUTF8, wstrUni.length() * 3);
		strRes = chUTF8;
		delete[]chUTF8;
		return strRes;
	}


	static string  utfs2s(const string& strutf)
	{
		wstring  wStrTmp;
		UTF2Uni(strutf.c_str(), wStrTmp);
		return ws2s(wStrTmp);
	}

};
#endif