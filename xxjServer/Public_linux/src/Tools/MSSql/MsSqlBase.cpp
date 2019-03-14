#include "MsSqlBase.h"
#include <boost\date_time\posix_time\time_formatters.hpp>
#include "..\MyTools.h"

// void CMSsqlBase::GetString(sql::SQLString & str, char *dest, unsigned int len)
// {
// 	const char *t = str.c_str();
// 	if (t == NULL)
// 	{
// 		dest[0] = '\0';
// 	}
// 	else
// 	{
// 		assert(len >= str->length());
// 		strncpy_s(dest, len, t, str->length());
// 		dest[len] = '\0';
// 	}
// }
// 
// void CMSsqlBase::GetString(sql::SQLString & str, std::string & dest, unsigned int len)
// {
// 	char * pTemp = new char[len + 1];
// 	GetString(str, pTemp, len);
// 	dest = std::string(pTemp);
// 	delete[]pTemp;
// }

void CMSsqlBase::destroy_db_res(_RecordsetPtr & db_res)
{
	if (db_res != nullptr)
	{
		try
		{
			db_res->Close();
		}
		catch (std::exception & e)
		{
			printf("fuck!%s",e.what());
		}

		//db_res->Release();
	}
}

bool CMSsqlBase::isClosed(_ConnectionPtr & con)
{
	return con->State == adStateClosed;
}

void CMSsqlBase::OnCatch(std::exception & e, std::string path)
{
	std::vector<std::string> ver;
	ver.push_back(boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time()) + "\t" + e.what());
	MyTools::PrintToFile(path, ver, false);
#ifdef _MFC_VER
	::MessageBox(nullptr, e.what(), "错误消息", MB_OK | MB_ICONERROR);
#endif
	std::cout << "error：" << e.what() << std::endl;//请输入参数
}

_RecordsetPtr CMSsqlBase::OnQuery(_ConnectionPtr & con, const std::string & sql)
{
	if (con && !isClosed(con))
	{
		return con->Execute(_bstr_t(sql.c_str()), NULL, adCmdText);
	}

	return nullptr;
	
}

CMsResultSet::CMsResultSet(_RecordsetPtr & ps)
{
	px_back = ps;
	px = this;
}

CMsResultSet::~CMsResultSet()
{
	OnDestroy();
}

bool CMsResultSet::IsNull()
{
	return px_back == nullptr;
}

bool CMsResultSet::isClosed()
{
	return (px_back && px_back->State == adStateClosed);
}

bool CMsResultSet::next()
{
	if (isNext)
	{
		px_back->MoveNext();
	}
	else
	{
		isNext = true;
	}

	return !px_back->adoEOF;
}

int CMsResultSet::getInt(const std::string & columnLabel)
{
	if (px_back)
	{
		variant_t vt = px_back->GetCollect(columnLabel.c_str());
		if (vt.vt != VT_NULL)
		{
			return (int)vt;
		}
	}
	return 0;
}

MY_UINT32 CMsResultSet::getUint(const std::string & columnLabel)
{
	if (px_back)
	{
		variant_t vt = px_back->GetCollect(columnLabel.c_str());
		if (vt.vt != VT_NULL)
		{
			return (UINT)vt;
		}
	}
	return 0;
}

std::string CMsResultSet::getString(const std::string & columnLabel)
{
	if (px_back)
	{
		variant_t vt = px_back->GetCollect(columnLabel.c_str());
		if (vt.vt != VT_NULL)
		{
			return (char *)(_bstr_t)vt;
		}
	}
	return "";
}

void CMsResultSet::OnDestroy()
{
	destroy_db_res(px_back);
}
