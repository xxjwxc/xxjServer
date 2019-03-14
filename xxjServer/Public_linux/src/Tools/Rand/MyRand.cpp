#include "MyRand.h"
#include <time.h>

unsigned int CMyRand::GetRand( const unsigned int min,const unsigned int max/*=0*/ )
{
	//srand(time(0));
	if (max<=min)
	{
		return 0;
	}
	boost::uniform_int<> ui(min,max-1);
	return ui(rng);
// 	int result=(rand()%(max-min))/2+(rand()%(max-min))/2;//更随机
// 	return result+min;
}

unsigned int CMyRand::GetRand(const unsigned int max)
{
	return GetRand(0, max);
}

CMyRand::CMyRand()
{
	rng.seed(time(0));//设置时间种子
}

double CMyRand::GetDoubleRand( const double max,const double min/*=0*/ )
{
	//srand(time(0));
	if (max<=min)
	{
		return 0;
	}
	boost::uniform_real<> ur(min,max-0.001);
	return ur(rng);
// 	int a=(max-min)/2;//使之更随机
// 	int integ=rand()%a+rand()%a;
// 	int decimal=(rand()%100+rand()%100)/2;
// 	return integ+decimal*0.01;
}

