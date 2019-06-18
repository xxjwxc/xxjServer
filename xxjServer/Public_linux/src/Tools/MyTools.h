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
#include <vector>
#include <string>
//#include <direct.h>
//#include "OnLogDeal.h"

//全局函数 初始化 所有
class MyTools
{
public:
  /*
  fname:当前可执行路径里文件夹下data目录下文件名
  prints：要打印的值，一个元素（std::string）代表要打印的一行数据
  isClear:是否清除数据
  strFl: 相对文件夹
  */ 
  static void PrintToFile(std::string fname,std::vector<std::string> & prints,bool isClear=true);//打印结果

  /*
  fname: 当前可执行路径里文件夹下data目录下文件名
  reads：读出存储的位置，一行文件内容存到一个boost::string里
  strFl: 相对文件夹
  */
  static bool ReadFromFile(std::string fname,std::vector<std::string> & reads, std::string strF = "");//读如数据
/*
	 获取目录里所有文件名，
	dir：目录
	filenames：文件列表
	：是否遍历子目录
	isImage:是否只返回图片文件
*/

  static int Get_filenames(const std::string& dir, std::vector<std::string>& filenames, bool isSubDir = true, bool isImage = false);

  //删除一个文件
  static bool Delete_File(const std::string & strPath);

  static bool FileIsExist(const std::string & strPath);//判断文件是否存在

  static int get_filesize(const std::string &filename);//获取为文件大小

  static bool BuildDir(const std::string & strPath);//创建目录 使用"/"


  //str 原数据 strRegex：正则表达式
  static std::vector<std::string> RegexAnalysis(const std::string & str, const std::string & strRegex);//正则表达式匹配

  //取得部分字符串
  static std::string FindFirstStr(std::string & strSrc,int n,bool isBlank = false);//isBlank 是否保留空格
  static std::string FindLastStr(std::string & strSrc,int n,bool isBlank = false);

  //过滤空格
  static void TrimBlank(std::string & strSrc,bool isfirst = true,bool isEnd = true);
  /*
  str：原字符串
  nsplit：拆分长度
  addStr：加入的字符串
  */
  //切割字符串并添加标记组成一个新的字符串
  static std::string SplitString(std::string str, unsigned int nsplit = 4, const std::string addStr = ",");

  //分割字符串
  static std::vector<std::string> SplitString(std::string str,const std::string splitStr = ",");

  //切割字符串 切割utf8字符串
  static std::vector<std::string> SplitStringUtf8(const std::string & str,unsigned int len);

  static void replace_all(std::string & Input, const std::string & Search, const std::string & Format);

  static std::string GetUtf8(const std::string & str);

  static std::string GetGBK(const std::string & str);

  static std::string GetGB2312(const std::string & str);

  static std::string BuildString(const std::string & str0,const std::string & str1);//构造字符串


  static std::string GetCurrentDate(); // 获取当前的时间

  static std::string GetCurrentDay(); // 获取当前的日期

  static void PrintError(const std::string & strerror);

  static std::string HexToBin(const std::string &strHex);//16进制转二进制字符串

  static std::string BinToHex(const std::string &strBin, bool bIsUpper = false);//二进制字符串转16进制字符串

  static std::string GetBigCamel(const std::string & str);//大驼峰命名规则
};

//内存检测 监视
#ifdef _WIN32
#define MEMORY_CHICK \
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);\
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;\
	_CrtSetDbgFlag(tmpFlag);
#else
#define MEMORY_CHICK 
#endif // _WIN32

#endif

