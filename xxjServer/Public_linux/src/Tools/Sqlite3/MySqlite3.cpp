#include "MySqlite3.h"
#include <iostream>
#include <functional>

CMySqlite3::~CMySqlite3()
{
}

bool CMySqlite3::OnInit(const std::string & dbNmame)
{
	m_dbName = dbNmame;
	m_ptrDb = Sqlite3Mgr::instance()->GetConnection(dbNmame);
	m_isInit = !m_ptrDb->IsNull();
	return m_isInit;
}

bool CMySqlite3::OnQuery(const std::string & sql, char** & pResult, int & nRow, int & nCol, char * &errMsg)
{
	if (m_isInit && !m_ptrDb->IsNull())
	{
		int rc = sqlite3_get_table(m_ptrDb->Get(), sql.c_str(), &pResult, &nRow, &nCol, &errMsg);
		return (rc == SQLITE_OK);
	}
	return false;
}

bool CMySqlite3::OnQuery(const std::string & sql, std::vector<std::map<std::string, std::string>> & m_result, std::string & errMsg)
{
	if (m_isInit && !m_ptrDb->IsNull())
	{
		m_result.clear();
		char** pResult;
		char *eM;
		int nRow;
		int nCol;
		int rc = sqlite3_get_table(m_ptrDb->Get(), sql.c_str(), &pResult, &nRow, &nCol, &eM);
		int nIndex = nCol;
		for (int i = 0; i < nRow; i++)
		{
			std::map<std::string, std::string> kv;
			for (int j = 0; j < nCol; j++)
			{
				kv[pResult[j]] = (pResult[nIndex]==nullptr?"": pResult[nIndex]);
				++nIndex;
			}
			m_result.push_back(kv);
		}

		if(eM)
			errMsg = std::string(eM);
		sqlite3_free_table(pResult);
		sqlite3_free(eM);

		return (rc == SQLITE_OK);
	}
	return false;
}

int CMySqlite3::OnExec(const std::string & sql, std::string & errMsg)
{
	if (m_isInit && !m_ptrDb->IsNull())
	{
		char *eM;
		int rc = sqlite3_exec(m_ptrDb->Get(), sql.c_str(), NULL, NULL, &eM);
		if (eM)
			errMsg = std::string(eM);
		sqlite3_free(eM);
		return (rc == SQLITE_OK);
	}
	return false;
}

bool CMySqlite3::OnDestory()
{
	if (m_isInit)
	{
		Sqlite3Mgr::instance()->ReleaseConnection(m_dbName);
		m_isInit = false;
	}
	
	return true;
}

