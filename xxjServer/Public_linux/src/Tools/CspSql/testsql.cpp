/********************************************************************
	创建时间：	2015/04/29  23:52:54
	文件名：	testsql.h
	作者：		谢小军
	
	功能：		1. 数据库测试项目
				2.
				
	说明：	    1. 
				2. 
*********************************************************************/
#include <iostream>
//#define  __SQLLITE  //SQLite数据库需开起此宏
//#define  __MYSQL	//MySql需要开启此红
#define  __ODBC		//ODBC需要开启此宏

//注意，需要引入两个头
#include "ODBCDatabase.h"
#include "DatabaseIO.hpp"//有模版定义函数
#include "..\MyTools.h"
#include <Poco/Data/RecordSet.h>


using namespace std;
int main(int argc, char *argv[])
{
	try
	{
		CODBCDatabaseIO database;//sqlserver调用
		//bool isConnect = database.ConnectionODBC("LENOVO-PC", "test", "sa", "123456", 1, 10, 60);//创建一个数据库连接池最小数1，最大数10，超时60秒
		bool isConnect = database.ConnectionODBC("10_131_171_47\\SQLEXPRESS", "test", "xxj", "123", 1, 10, 60);//创建一个数据库连接池最小数1，最大数10，超时60秒

// 		//需要开起宏__SQLLITE（SQLDefine.h中）
// 		CDatabaseIO database;//sqlite调用
// 		bool isConnect = database.Connection("Sqlite", "database.db",1,10,60);//创建一个数据库连接池最小数1，最大数10，超时60秒
		
// 		//需要开起宏__MySql（SQLDefine.h中）
// 		CODBCDatabaseIO database;//sqlite调用
// 		bool isConnect = database.ConnectionMySql("192.168.70.8", "3306", "test", "root", "123456", 1, 10, 60);
// 		//bool isConnect = database.Connection("MySql", "host=192.168.70.8;port=3306;user=root;password=123456;db=log_test_db;compress=true;auto-reconnect=true", 1, 10, 60);//创建一个数据库连接池最小数1，最大数10，超时60秒

		std::string str = "SELECT * FROM log_tab";
		
		bool bResult = false;
		{
			std::string strTemp = "SELECT max_license_id FROM log_tab where id = 1";
			int item;
			bResult = database.SelectSql(strTemp, item);//查询一条数据，查询结果只有一条记录

			strTemp = "SELECT max_license_id,utc_time FROM log_tab";// "SELECT max_license_id,utc_time FROM log_tab";
			Poco::Data::Statement query(database.GetSession());
			query << strTemp;
			query.execute();
			Poco::Data::RecordSet rs(query);
			bool more = rs.moveFirst();
			while (more) {
				std::cout << "id:" << rs["id"].convert<int>() << " ";
				std::cout << "name:" << rs["name"].convert<std::string>() << " ";
				std::cout << "age:" << rs["age"].convert<int>() << " ";
				std::cout << "text:" << rs["text"].convert<std::string>() << " ";
				std::cout << "num:" << rs["num"].convert<int>() << " ";
				std::cout << "utc_time:" << rs["utc_time"].convert<std::string>() << " ";
				std::cout << std::endl;
				more = rs.moveNext();
			}

			Sleep(60000);
			return 0;
			std::vector<int> items;
			bResult = database.SelectSql(strTemp, items);//查询一列数据，查询结果有多条数据
		}

//		自定义获取
// 		{
// 			std::string strTemp = "SELECT max_license_id FROM log_tab where id = 1";
// 			int item;
// 			bResult = database.SelectSql(strTemp, item);//查询一条数据，查询结果只有一条记录
// 
// 			strTemp = "call XXXX(100);";// "SELECT max_license_id,utc_time FROM log_tab";
// 			Poco::Data::Statement query(database.GetSession());
// 			query << strTemp;
// 			query.execute();
// 			Poco::Data::RecordSet rs(query);
// 			bool more = rs.moveFirst();
// 			while (more) {
// 				std::cout << "max_license_id:" << rs["max_license_id"].convert<int>() << " ";
// 				std::cout << "utc_time:" << rs["utc_time"].convert<std::string>() << " ";
// 				std::cout << std::endl;
// 				more = rs.moveNext();
// 			}
// 
// 			std::vector<int> items;
// 			bResult = database.SelectSql(strTemp, items);//查询一列数据，查询结果有多条数据
// 		}

		{
			std::string strTemp = "SELECT * FROM user_tbl where id = 1";
			//实现无tuple版本，只适用于单个结果集情况-----------new
			struct AA
			{
				int a;
				std::string b;
				int c;
				std::string d;
				int e;
			}aa;
			bResult = database.SelectSql(strTemp, aa.a, aa.b, aa.c, aa.d, aa.e);//查询一组数据，查询结果有多条记录
			//--------------------------------------------------------------------------------new

			std::tuple<int, std::string, int, std::string, int> item;
			bResult = database.SelectSql(strTemp, item);//查询一组数据，查询结果只有一条记录
			//test out put
			std::cout << std::get<0>(item) << std::endl;
			std::cout << std::get<1>(item).c_str() << std::endl;
			std::cout << std::get<2>(item) << std::endl;
			std::cout << std::get<3>(item).c_str() << std::endl;
			std::cout << std::get<4>(item) << std::endl;
		}

		
		{
			str = "SELECT * FROM user_tbl";
			std::vector<std::tuple<int, std::string, int, std::string, int>> items;
			bResult = database.SelectSql(str, items);//查询多组数据，查询结果有多条记录
			//test out put
			for (auto vpk : items)
			{
				std::cout << std::get<0>(vpk) << ",";
				std::cout << std::get<1>(vpk).c_str() << ",";
				std::cout << std::get<2>(vpk) << ",";
				std::cout << std::get<3>(vpk).c_str() << ",";
				std::cout << std::get<4>(vpk) << std::endl;
			}
		}

		
		{
			std::vector<std::tuple<int, std::string, int, std::string, int>> items;
			bResult = database.SelectSql(str, items, 5);//查询多条数据，并返回5条记录 （1）
			//test out put
			for (auto vpk:items)
			{
				std::cout << std::get<0>(vpk) << ",";
				std::cout << std::get<1>(vpk).c_str() << ",";
				std::cout << std::get<2>(vpk) << ",";
				std::cout << std::get<3>(vpk).c_str() << ",";
				std::cout << std::get<4>(vpk) << std::endl;
			}


			//items.clear();
			while (database.ContinueExecute(items))//查询多条数据，并返回下一个5调记录//依次拿出 （2）
			{
				for (auto vpk : items)
				{
					std::cout << std::get<0>(vpk) << ",";
					std::cout << std::get<1>(vpk).c_str() << ",";
					std::cout << std::get<2>(vpk) << ",";
					std::cout << std::get<3>(vpk).c_str() << ",";
					std::cout << std::get<4>(vpk) << std::endl;
				}
				//items.clear();
			}

			//说明：（1）（2），（2）必须与（1）合用才能起作用
		}

		{//insert
			
			std::string strTemp = "INSERT INTO user_tbl(name,age,text,num) VALUES(?,?,?,?)";
			std::tuple<std::string, int, std::string, int> item;
			item = std::make_tuple("xxj", 111, "xxj", 100);
			bResult = database.InsertSql(strTemp, item);//插入一条记录，tuple中顺序必须于(name,age,text,num)中顺序一致
			
// 			strTemp = "INSERT INTO user_tbl VALUES(:id,:name,:age,:text,:num)";
// 			std::tuple<int,std::string, int, std::string, int> item1;
// 			item1 = std::make_tuple(30,"xxj", 111, "xxj", 100);
// 			bResult = database.InsertSql(strTemp, item1);//插入一条记录，tuple 中类型顺序必须与数据库表中顺序一致

			strTemp = "INSERT INTO user_tbl(name,age,text,num) VALUES(?,?,?,?)";
			std::vector<std::tuple<std::string, int, std::string, int>> items;
			items.push_back(item);
			std::get<0>(item) = "我们";
			items.push_back(item);
			items.push_back(item);
			items.push_back(item);
			items.push_back(item);
			std::get<0>(item) = "我们..n";
			items.push_back(item);
			bResult = database.InsertSql(strTemp, items);//插入多条数据，注意tuple中类型顺序
		}

		{
			std::string strTemp = "UPDATE user_tbl SET name = 'xxj' WHERE id = 0";
			bResult = database.ExecuteSql(strTemp);//执行一条语句，更新

			strTemp = "DELETE FROM user_tbl WHERE id = 15";
			bResult = database.ExecuteSql(strTemp);//执行一条语句，删除
		}

		if (!bResult)//异常获取
		{
			std::cerr << database.GetLastError()<<std::endl;//获取错误描述符
		}

	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cin.get();
	return 0;
}
