#ifdef WIN32
#pragma warning(disable:4996)
#endif // _WIN32
#include "MyTools.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>

#include <boost/locale.hpp>
#include <boost/xpressive/xpressive.hpp>//动态正则表达式库  
#include <boost/thread/mutex.hpp>


#include <boost/algorithm/string.hpp>

#include <memory>
#ifdef _WIN32
#include <direct.h>
#else
#include <time.h>
#include <sys/stat.h> 
#endif // _WIN32





//#define MAX_WAIT_TIME 60
//using namespace boost::xpressive;//正则表达式库


boost::mutex g_oMutex;
void MyTools::PrintToFile( std::string fname,std::vector<std::string> & prints,bool isClear)
{
	boost::mutex::scoped_lock oLock(g_oMutex);//互斥锁
	//MessageBox(nullptr,fname.c_str(),"写文件",MB_OK);
	setlocale(LC_ALL, "chs");
	std::string path = fname; // GetModulePath(strF, fname);
	//path = "\\\\QGFPVZ3ZLRZTTEH\\iPad\\TXT\\"+ fname;
	namespace newfs = boost::filesystem;
	newfs::ofstream ofs(path.c_str(),(isClear?(std::ios_base::out|std::ios::trunc):(std::ios_base::app|std::ios::ate)));
	if (!ofs.is_open())
	{
		unsigned int loc = fname.rfind("/", fname.length());
		if( loc != std::string::npos )
		{
			std::string dir(fname.begin(),fname.begin()+loc);
#ifdef _WIN32
			_mkdir(dir.c_str());
#else
			mkdir(dir.c_str(), S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
#endif // _WIN32
			
			std::cout<<"build dir \n";
			oLock.unlock();
			PrintToFile(fname,prints,isClear);
			return;
		}
	}

	if (ofs.is_open())//如果打开了
	{
		std::vector<std::string>::const_iterator it = prints.begin();
		while(it != prints.end())
		{
			ofs<<*it<<std::endl;
			it++;
		}
		ofs.close();
	}
	else//没有打开
		assert(false);
}

bool MyTools::ReadFromFile( std::string fname,std::vector<std::string> & reads, std::string strF)
{
	reads.clear();//清除老元素

	setlocale(LC_ALL, "chs");
	std::string path = fname;
	namespace newfs = boost::filesystem;
	newfs::ifstream ifs(path.c_str(),std::ios::in);
	std::string str;
	if (ifs.is_open())//如果打开了
	{
		while(!ifs.eof())
		{
			std::getline(ifs,str);
			if (str.length() >0)//空行取消存取
				reads.push_back(str);
		}
		ifs.close();

		return true;
	}
	else//没有打开
		return false;

}


int MyTools::Get_filenames(const std::string& dir, std::vector<std::string>& filenames, bool isSubDir /*= true */, bool isImage /*= false*/)
{
	filenames.clear();
	boost::filesystem::path path(dir);
	if (!boost::filesystem::exists(path))
	{
		boost::filesystem::create_directories(path);//创建目录	
													//boost::filesystem::create_directory(path);//创建目录(不能创建数型目录)	
		return -1;
	}

	boost::filesystem::directory_iterator end_iter;
	for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter)
	{
		if (boost::filesystem::is_regular_file(iter->status()))
		{
			if (!isImage || boost::icontains(".JPEG.JPG.TIFF.TIF.BMP.GIF.PNG", iter->path().extension().string()))
				filenames.push_back(iter->path().filename().string());
		}
		else if (boost::filesystem::is_directory(iter->status()))
		{
			if (isSubDir)//查找子目录
			{
				Get_filenames(iter->path().string(), filenames, isSubDir, isImage);
			}
			else
			{
				//boost::filesystem::remove(iter->path());//删除文件
				boost::filesystem::remove_all(iter->path());//删除目录
			}
		}
	}

	return filenames.size();
}

bool MyTools::Delete_File(const std::string & strPath)
{
	boost::filesystem::path path(strPath);
	if (boost::filesystem::exists(path))
		boost::filesystem::remove(path);//删除文件

	return true;
}

bool MyTools::FileIsExist(const std::string & strPath)
{
	boost::filesystem::path path(strPath);
	return boost::filesystem::exists(path);
}


int MyTools::get_filesize(const std::string &filename)
{
	struct stat f_stat;
	if (stat(filename.c_str(), &f_stat) == -1)
	{
		return -1;
	}
	return f_stat.st_size;
}

bool MyTools::BuildDir(const std::string & strPath)
{
	size_t loc = strPath.rfind("/", strPath.length());
	if (loc != std::string::npos)
	{
		std::string dir(strPath.begin(), strPath.begin() + loc);
		boost::filesystem::path path_file(dir);
		if (boost::filesystem::exists(path_file) &&
			boost::filesystem::is_directory(path_file))
		{
			return true;
		}
		else
		{

			loc = 3;
			loc = dir.find("/", loc);
			while (loc != std::string::npos)
			{
				std::string ddd(dir.begin(), dir.begin() + loc);
#ifdef _WIN32
				_mkdir(ddd.c_str());
#else
				mkdir(ddd.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
				loc = dir.find("/", loc + 1);
			}
#ifdef _WIN32
			_mkdir(dir.c_str());
#else
			mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif // _WIN32
			boost::filesystem::path path_file1(dir);
			return boost::filesystem::exists(path_file1) && boost::filesystem::is_directory(path_file1);
		}
			
	}

	return true;
}

std::vector<std::string> MyTools::RegexAnalysis(const std::string & str, const std::string & strRegex)
{
	boost::xpressive::cregex _reg = boost::xpressive::cregex::compile(str, boost::xpressive::icase);//更新的正则表达式不会变的，故为了加快速度，icase表示大小写不敏感
	boost::xpressive::cmatch what;//匹配结果列表
	std::vector<std::string> ver;
	try {
		boost::xpressive::cregex_iterator pos(str.c_str(), str.c_str() + str.length() + 1, _reg);
		boost::xpressive::cregex_iterator end;
		while (pos != end)
		{
			ver.push_back((*pos)[0]);
		}
	}
	catch (boost::xpressive::regex_error e) {
		//printf("regex error by compile :%d", e.code());
	}

	return ver;
}

std::string MyTools::FindFirstStr( std::string & strSrc,int n,bool isBlank )
{
	boost::iterator_range<std::string::iterator> ir;
	ir = boost::find_head(strSrc,n);
	std::string str = std::string(ir.begin(),ir.end());
	if (!isBlank)
	{
		boost::trim_left(str);
		boost::trim_right(str);
	}
	return str;
}

std::string MyTools::FindLastStr( std::string & strSrc,int n,bool isBlank )
{
	boost::iterator_range<std::string::iterator> ir;
	ir = boost::find_tail(strSrc,n);
	std::string str = std::string(ir.begin(),ir.end());
	if (!isBlank)
	{
		boost::trim_left(str);
		boost::trim_right(str);
	}
	return str;
}

void MyTools::TrimBlank(std::string & strSrc,bool isfirst /*= true*/, bool isEnd /*= true*/)
{
	if (isfirst)
		boost::trim_left(strSrc);
	if (isEnd)
		boost::trim_right(strSrc);
}

std::string MyTools::SplitString(std::string str, unsigned int nsplit /*= 4*/, const std::string addStr /*= ","*/)
{
	std::string strTemp(str.rbegin(),str.rend());
	std::string strResult = "";
	int n = 0;
	while(true)
	{
		n = (strTemp.length() > nsplit) ? nsplit:strTemp.length();
		strResult.append(FindFirstStr(strTemp, n, true));
		strTemp = FindLastStr(strTemp,strTemp.length() - n,true);
		if (strTemp.length() != 0)
			strResult.append(addStr);
		else
			break;
	}
	return std::string(strResult.rbegin(), strResult.rend());
}

std::vector<std::string> MyTools::SplitString(std::string str, const std::string splitStr /*= ","*/)
{
	std::vector<std::string> vecSegTag;
	boost::split(vecSegTag, str, boost::is_any_of(splitStr));

	return vecSegTag;
}

std::string MyTools::GetUtf8(const std::string & str)
{
	return boost::locale::conv::between(str,"UTF-8","GBK");
}

std::string MyTools::GetGBK(const std::string & str)
{
	return boost::locale::conv::between(str, "GBK", "UTF-8");
}

std::string MyTools::GetGB2312(const std::string & str)
{
	return boost::locale::conv::between(str, "GBK", "UTF-8");
}

std::vector<std::string> MyTools::SplitStringUtf8(const std::string & str,unsigned int len)
{
	int size = str.length();
	std::string strTemp;
	std::vector<std::string> verStr;
	int n = 0;
	int n1 = 0;
	int i = 0;
	while (i<size)
	{
		if (str[i] > 0)//直接添加
		{
			strTemp.append(1,str[i]);
			i++;
			n1++;
		}
		else
		{
			strTemp.append(str.begin() + i, str.begin() + i + 3);
			i += 3;
			n++;
		}

		
		if ((n + (n1+1)/2 ) == len)
		{
			verStr.push_back(strTemp);
			strTemp.clear();
			n = 0;
			n1 = 0;
		}
	}

	if (strTemp.length() > 0)
		verStr.push_back(strTemp);

	return verStr;
}

void MyTools::replace_all(std::string & Input, const std::string & Search, const std::string & Format)
{
	boost::replace_all(Input, Search, Format);
}

std::string MyTools::BuildString(const std::string & str0, const std::string & str1)
{
	std::string str = "";
	str.append(str0);
// 	int n = 5 - str0.length()/3;
// 	str.append(n,' ');

//	n = str1.length();
// 	if (n < 6)
// 		str.append("[");
	str.append("["+str1+"]");

	return str;
}


std::string MyTools::GetCurrentDate()
{
	std::string ret;
	time_t t = time(NULL);
	char date[64];
	struct tm tmTmp = {0};
#ifdef _WIN32
	localtime_s(&tmTmp, &t);
#else
	localtime_r(&t, &tmTmp);
#endif
	memset(date, 0, sizeof(date));
	strftime(date, sizeof(date), "%Y/%m/%d %H:%M:%S", &tmTmp);
	ret = date;
	return date;
}

std::string MyTools::GetCurrentDay()
{
	std::string ret;
	time_t t = time(NULL);
	struct tm tmTmp;
#ifdef _WIN32
	localtime_s(&tmTmp, &t);
#else
	localtime_r(&t, &tmTmp);
#endif
	char date[64];
	memset(date, 0, sizeof(date));
	strftime(date, sizeof(date), "%Y-%m-%d", &tmTmp);
	ret = date;
	return date;
}

void MyTools::PrintError(const std::string & strerror)
{
#ifdef _WIN32
	_mkdir("loginfo");
#else
	mkdir("loginfo", S_IRWXU);
#endif // _WIN32
	std::string path = "loginfo/" + MyTools::GetCurrentDay() + ".txt";
	std::vector<std::string> ver;
	ver.push_back(strerror + "\t[" + MyTools::GetCurrentDate() + "]");
	PrintToFile(path, ver, false);
}

std::string MyTools::HexToBin(const std::string &strHex)
{
	if (strHex.size() % 2 != 0)
	{
		return "";
	}

	std::string strBin;
	strBin.resize(strHex.size() / 2);
	for (size_t i = 0; i < strBin.size(); i++)
	{
		uint8_t cTemp = 0;
		for (size_t j = 0; j < 2; j++)
		{
			char cCur = strHex[2 * i + j];
			if (cCur >= '0' && cCur <= '9')
			{
				cTemp = (cTemp << 4) + (cCur - '0');
			}
			else if (cCur >= 'a' && cCur <= 'f')
			{
				cTemp = (cTemp << 4) + (cCur - 'a' + 10);
			}
			else if (cCur >= 'A' && cCur <= 'F')
			{
				cTemp = (cTemp << 4) + (cCur - 'A' + 10);
			}
			else
			{
				return "";
			}
		}
		strBin[i] = cTemp;
	}

	return strBin; 
}

std::string MyTools::BinToHex(const std::string &strBin, bool bIsUpper /*= false*/)
{
	std::string strHex;
	strHex.resize(strBin.size() * 2);
	for (size_t i = 0; i < strBin.size(); i++)
	{
		uint8_t cTemp = strBin[i];
		for (size_t j = 0; j < 2; j++)
		{
			uint8_t cCur = (cTemp & 0x0f);
			if (cCur < 10)
			{
				cCur += '0';
			}
			else
			{
				cCur += ((bIsUpper ? 'A' : 'a') - 10);
			}
			strHex[2 * i + 1 - j] = cCur;
			cTemp >>= 4;
		}
	}

	return strHex;
}
