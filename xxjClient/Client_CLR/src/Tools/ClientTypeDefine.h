#ifndef _CLIENT_TYPEDEFINE_H
#define	_CLIENT_TYPEDEFINE_H

#include "MyTools.h"

#include <cstdint>
#include <stdio.h>
//#include <cliext/list>
//#include <cliext/vector>

#using <System.dll>
using namespace System;
using namespace System::IO;
using namespace System::Net::Sockets;
using namespace System::Collections::Generic;

#define MAX_RESERVE_SIZE 1024*8  //最大接收尺寸

#ifdef _USING_CLR
#define INT8 System::SByte
#define UINT8 System::Byte
#define INT16 System::Int16
#define UINT16 System::UInt16
#define INT32 System::Int32
#define UINT32 System::UInt32
#define INT64 System::Int64
#define UINT64 System::UInt64
#define BOOLN System::Boolean
#define FLOAT System::Single
#define DOUBLE System::Double
#define MY_UINT32 unsigned int
#define	CString	System::String^
#define std System
#define string String
#define Vector array
#define Set List
#define Map Dictionary

#define QNEW gcnew
#define QDLL_EXPORT public
#define QCLASS ref class
#define QSTRUCT value struct
#define QENUM enum class
#define QCONST_FUNC
#define CHAR_PTR char^
#define NULL_PTR nullptr
#define QABSTRACT abstract
#define QINTERFACE interface class
#define QOVERRIDE override
//#define std cliext

#define TO_PTR(oRef) (%(oRef))
#define TO_REF(pPtr) (*(pPtr))
#define PTR(tType) tType^
#define REF(tType) tType%
#define CONST_PTR(tType) tType^
#define CONST_REF(tType) tType%

#define assert(condition) if(!(condition)) throw QNEW System::Exception(#condition" assert failed")
#else
#include <string>
#define	CString	std::string
typedef     int8_t      INT8;
typedef     uint8_t     UINT8;
typedef     int16_t     INT16;
typedef     uint16_t    UINT16;
typedef     int32_t     INT32;
typedef     uint32_t    UINT32;
typedef     int64_t     INT64;
typedef     uint64_t    UINT64;
typedef		bool		BOOLN;
typedef		float		FLOAT;
typedef		double		DOUBLE;
typedef unsigned int	MY_UINT32;

#define QNEW new
#define QDLL_EXPORT
#define QCLASS class
#define QSTRUCT struct
#define QENUM enum
#define QCONST_FUNC const
#define CHAR_PTR char*
#define NULL_PTR 0
#define QABSTRACT
#define QINTERFACE class
#define QOVERRIDE

#define TO_PTR(oRef) (&(oRef))
#define TO_REF(pPtr) (*(pPtr))
#define PTR(tType) tType*
#define REF(tType) tType&
#define CONST_PTR(tType) const tType*
#define CONST_REF(tType) const tType&
#endif

#endif	/* CTYPEDEFINE_H */

