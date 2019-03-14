#pragma once

//libgo在Linux系统上默认使用ucontext做协程上下文切换，开启此选项将使用boost.coroutine来替代ucontext.
#define USE_BOOST_COROUTINE 1

#define USE_UCONTEXT 0

#define USE_FIBER 1

//libgo在Linux系统上默认使用ucontext做协程上下文切换，开启此选项将使用boost.context来替代ucontext.
#define USE_BOOST_CONTEXT 1//这是在linux上性能最佳的编译参数

#define ENABLE_DEBUGGER 0

#define WITH_CARES 0

#ifdef WIN32
#define WITH_SAFE_SIGNAL 0
#else
#define  WITH_SAFE_SIGNAL 1
#endif

#define LIBGO_SINGLE_THREAD 0

