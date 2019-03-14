#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <libgo/coroutine.h>
#include <Tools/win_exit.h>
#include <mutex>
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;


using namespace std::chrono;

// 一个阻塞操作
hostent* block1(const char *name)
{
	printf("do block1\n");
	return gethostbyname(name);
}

void block2()
{
	printf("do block2\n");
	gethostbyname("www.google.com");
}

void foo()
{
	printf("start co_await\n");
	/** co_await是一个宏, 需要一个参数: 返回类型
	*  co_await(type)后面要写一个可调用对象 (lambda, 函数指针, function对象等等, 规则和go关键字相同)
	*  在co_await内部会将操作任务到线程池中, 并使用channel等待任务完成。
	*/
	hostent* ht = co_await(hostent*) [] { return block1("www.baidu.com"); };
	(void)ht;

	/** co_await也可以用于无返回值的调用
	*
	*/
	co_await(void) block2;

	// 不要投递会无限等待的任务到线程池中, 会导致线程池的一个调度线程被阻塞,
	// 一旦线程池的调度线程全部被阻塞, 线程池就无法工作了.
	printf("done co_await\n");
}
#include <Tools/Libcurl/MyHttpTools.h>

int main(int argc, char** argv)
{
	CMyHttpTools http;
	std::tuple<bool, std::string>  tup = http.OnOpenHttpByjson("http://localhost:9094/api/v1/log/get", "{\"device_id\": \"E10ADC3949BA59ABBE56E057F20F883E\",\"tm\":1}", true, 10);

	std::cout << std::get<1>(tup) << std::endl;

	// 如果需要使用线程池功能, 需要用户自行创建线程去Run线程池.
	boost::thread_group tg;
	co_sched.InitMaxThread(1);
	for (int i = 0; i < 1; ++i) {
		tg.create_thread([] {co_sched.GetThreadPool().RunLoop(); });
	}

	go foo;
	go foo;
	go foo;
	go foo;
	go foo;
	go foo;
	go foo;

	co_sched.RunUntilNoTask();

	return 0;
}

