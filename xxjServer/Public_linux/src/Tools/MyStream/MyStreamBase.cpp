#include "MyStreamBase.hpp"
#include "../MyTools.h"

MY_UINT32 CMyStreamBase::Size()
{
	return m_ver.size();
}

void CMyStreamBase::Clear()
{
	m_ver.clear();
	m_indexPop = 0;
}

bool CMyStreamBase::IsEnd()
{
	return m_ver.size() == m_indexPop;
}


MY_UINT32 CMyStreamBase::OnGetBuffer(boost::scoped_array<unsigned char> & ptr)
{
	ptr.reset(new unsigned char[Size()]);
	memcpy(ptr.get(), (const void*)&m_ver[0], m_ver.size()*sizeof(unsigned char));
	return Size();
}


MY_UINT32 CMyStreamBase::OnGetBuffer(boost::scoped_array<signed char> & ptr)
{
	ptr.reset(new signed char[Size()]);
	memcpy(ptr.get(), (const void*)&m_ver[0], m_ver.size() * sizeof(unsigned char));
	return Size();
}

std::vector<unsigned char> CMyStreamBase::OnGetBuffer()
{
	return m_ver;
}

void CMyStreamBase::Memcpy(const void * _Src, size_t _Size)
{
	const unsigned char *  src = (const unsigned char *)_Src;
	for (unsigned i = 0; i < _Size; i++)
	{
		m_ver.push_back(*(src + i));
	}
}

void CMyStreamBase::Memcpy(void * _Des, size_t _Size)
{
	if ((m_indexPop + _Size) > m_ver.size())
	{
		throw std::runtime_error("sequence error£¬size out");
		return;
	}
	assert((m_indexPop+_Size) <= m_ver.size() && "sequence error£¬size out");

	unsigned char *  des = (unsigned char *)_Des;
	for (size_t i = 0; i < _Size; i++)
	{
		des[i] = m_ver.at(m_indexPop + i);
	}

	m_indexPop += _Size;
}

CMyStreamBase & CMyStreamBase::Push(const std::string & val)
{
	std::string temp = MyTools::GetUtf8(val);

	MY_UINT32 len = temp.size();
	Push(len);
	const char * cp = temp.c_str();
	for (int i = 0; i < len; i++)
	{
		Push(*(cp + i));
	}

	return *this;
}

void CMyStreamBase::OnSetBuffer(boost::scoped_array<unsigned char> & src, MY_UINT32 len)
{
	Clear();
	m_ver.assign(src.get(), src.get()+len);
}

void CMyStreamBase::OnSetBuffer(boost::scoped_array<signed char> & src, MY_UINT32 len)
{
	Clear();
	m_ver.assign(src.get(), src.get() + len);
}

CMyStreamBase & CMyStreamBase::Pop(std::string & val)
{
	val.clear();
	MY_UINT32 len = 0;
	Pop(len);
	val.resize(len);
	char* p = NULL;
	if (len > 0)
		p = &val[0];
	char cp =0;
	for (int i = 0; i < len; i++)
	{
		Pop(cp);
		p[i] = cp;
	}

	val = MyTools::GetGB2312(val);
	return *this;
}

