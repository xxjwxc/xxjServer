#include "MyCereal.h"
//定义类

struct MyRecord
{
	uint8_t x, y;
	float z;
	std::map < std::string, std::string > m_map;

	META(x, y, z,m_map);

};

struct SomeData
{
	int32_t id;
	MyRecord  data;

	META(id, data);

};

//执行
int main()
{
	SomeData myData;
	{
		myData.id = 11;
		myData.data.x = 10;
		myData.data.y = 123;
		myData.data.z = 456;
		myData.data.m_map["xxj"] = "wwc";
		CMyCereal::OnSaveBinary("out.cereal", myData);
	}
	{
		myData = SomeData();
		CMyCereal::OnLoadBinary("out.cereal", myData);
	}



	return 0;
}
//