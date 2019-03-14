#ifndef RANDOM_H_
#define RANDOM_H_
#include <vector>
#include<boost/random.hpp>
using std::vector;
class CMyRand{
private:
	boost::mt19937 rng;//随机数发生器
public:
	CMyRand();
  /*
  得到的此随机数范围为[min,max)
  */
  unsigned int GetRand(const unsigned int min,const unsigned int max);//得到一个随机整数
  /*
  得到的此随机数范围为[0,max)
  */
  unsigned int GetRand(const unsigned int max);//得到一个随机整数
  /*
  得到的此随double机数范围为[min,max)
  得到小数点后两位
  */
  double GetDoubleRand(const double max,const double min=0);//得到一个随机double数
};
#endif