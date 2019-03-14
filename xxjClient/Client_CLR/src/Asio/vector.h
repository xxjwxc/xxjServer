#ifdef _USING_CLI
#ifndef H_QWER_VECTOR_H
#define	H_QWER_VECTOR_H

#include "../CTypeDefine.h"
#include "IData.h"

using System::ArgumentOutOfRangeException;

namespace QWER
{
	generic<typename T>
	QDLL_EXPORT QCLASS vector
	{
	public:
		vector()
		{
			m_aData = QNEW array<T>(1);
		}

		void assign(PTR(array<T>) poBuffer, INT32 dwBegin, INT32 dwSize)
		{
			resize(dwSize);
			System::Array::Copy(poBuffer, dwBegin, m_aData, 0, dwSize);
		}

		void push_back(T oData)
		{
			if(size() == capacity())
			{
				reserve(2 * capacity());
			}
			m_aData[m_dwSize++] = oData;
		}

		void pop_back()
		{
			if(m_dwSize == 0)
				throw QNEW ArgumentOutOfRangeException();
			--m_dwSize;
		}

		property T default[INT32]
		{
			T get(INT32 index)
			{
				if(index >= m_dwSize)
					throw QNEW ArgumentOutOfRangeException();
				return m_aData[index];
			}
			void set(INT32 index, T value)
			{
				if(index >= m_dwSize)
					throw QNEW ArgumentOutOfRangeException();
				m_aData[index] = value;
			}
		}

		T back()
		{
			return m_aData[m_dwSize - 1];
		}

		T front()
		{
			return m_aData[0];
		}

		inline INT32 size() QCONST_FUNC
		{
			return m_dwSize;
		}

		inline void clear()
		{
			resize(0);
		}

		inline INT32 capacity() QCONST_FUNC
		{
			return (INT32)m_aData->Length;
		}

		void reserve(INT32 dwCapacity)
		{
			INT32 dwOldCapacity = capacity();
			if(dwOldCapacity >= dwCapacity)
			{
				for(INT32 i = dwCapacity; i != dwOldCapacity; i++)
				{
					m_aData[i] = T();
				}
				return;
			}
			PTR(array<T>) aTemp = m_aData;
			m_aData = QNEW array<T>(dwCapacity);
			aTemp->CopyTo(m_aData, 0);
		}

		inline void resize(INT32 dwSize)
		{
			reserve(dwSize);
			m_dwSize = dwSize;
		}

	protected:
		PTR(array<T>) m_aData;
		INT32 m_dwSize;
	};
};

#endif	/* COCTSTREAM_H */
#endif
