#include "MyStreamBase.hpp"

CMyStreamBase::CMyStreamBase()
{
	Clear();
}

void CMyStreamBase::OnSetBuffer(PTR(array<Byte>) src, MY_UINT32 len)
{
	Clear();
	const char * data = MyTools::ArrayToChar(src);
	m_ver = gcnew List<Byte>(0);

	const char *p2 = (const char *)data;
	size_t n = len;
	if (n) {
		n++;
		while (--n > 0) {
			m_ver->Add(*p2++);
		}
	}

	//m_ver.assign(src.get(), src.get() + len);
}

bool CMyStreamBase::IsEnd()
{
	return Size() == m_indexPop;
}

MY_UINT32 CMyStreamBase::OnGetBuffer(REF(PTR(array<Byte>)) ptr)
{
	MY_UINT32 len = Size();
	ptr = gcnew array<Byte>(len);
	
	char * des = MyTools::ArrayToChar(MyTools::ListToArray(m_ver));

	MyTools::memcpy(MyTools::ArrayToChar(ptr), (const void*)des, len * sizeof(unsigned char));

	return len;
}

PTR(array<Byte>) CMyStreamBase::OnGetBuffer()
{
	return MyTools::ListToArray(m_ver);
}

MY_UINT32 CMyStreamBase::Size()
{
	return MyTools::ListToArray(m_ver)->Length;
}

void CMyStreamBase::Clear()
{
	if (m_ver)
		m_ver->Clear();
	else
		m_ver = gcnew List<Byte>();
	m_indexPop = 0;
}

PTR(CMyStreamBase) CMyStreamBase::PushString(PTR(string) val)
{
	array<unsigned char>^ bts = System::Text::Encoding::UTF8->GetBytes(val);
	Push((MY_UINT32)bts->Length);
	for (int i = 0; i != bts->Length; i++)
	{
		Push(bts[i]);
	}

	return this;
}

PTR(CMyStreamBase) CMyStreamBase::Push(const void * _Src, size_t _Size)
{
	Memcpy(_Src, _Size);
	return this;
}

PTR(CMyStreamBase) CMyStreamBase::Pop(void * _Src, size_t _Size)
{
	Memcpy(_Src, _Size);
	return this;
}


void CMyStreamBase::Memcpy(const void * _Src, size_t _Size)
{
	const unsigned char *  src = (const unsigned char *)_Src;
	for (unsigned i = 0; i < _Size; i++)
	{
		m_ver->Add(*(src + i));
	}
}

void CMyStreamBase::Memcpy(void * _Des, size_t _Size)
{
	if ((m_indexPop + _Size) > Size())
	{
		throw gcnew System::SystemException("sequence error£¬size out");
		return;
	}

	assert((m_indexPop + _Size) <= Size() && "sequence error£¬size out");

	unsigned char *  des = (unsigned char *)_Des;
	for (size_t i = 0; i < _Size; i++)
	{
		des[i] = m_ver[m_indexPop + i];
	}

	m_indexPop += _Size;
}