#include "iostream"
#include "vector"
#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"

using namespace log4cxx;
using namespace std;

class VectorRef
{
public:
	std::vector<int> vecInts;
public:
	VectorRef(int size);
	~VectorRef();
	std::vector<int>& GetVecInts()//使用情景之一：引用作为函数返回值，若不使用引用将会发生拷贝构造（速度慢）
	{
		return vecInts;
	}
};

//构造函数
VectorRef::VectorRef(int size)
{
	for (int i = 0; i < size; i++)
	{
		vecInts.push_back(i);
	}
}


VectorRef::~VectorRef()
{
}

//打印容器
void PrintVecInts(const std::vector<int>& vecInts)
{
	printf("\n");
	for (size_t i = 0; i < vecInts.size(); i++)
	{
		printf("%d current value = %d\n", i, vecInts[i]);
	}
}

//测试容器
void TestVecInts()
{
	VectorRef vRef(4);
	vector<int>& v = vRef.GetVecInts();//使用情景之二：赋值时使用引用，否则同样会发生拷贝构造（速度慢）
	v[0] = 100;
	PrintVecInts(v);
	PrintVecInts(vRef.GetVecInts());
}

void main()
{
	log4cxx::PropertyConfigurator::configure("./log4cxx/log4cxx.properties");//加载日志配置

	for (int i = 0; i < 500; i++)
	{
		LOG4CXX_INFO(log4cxx::Logger::getLogger("hello"), i);
	}
	//TestVecInts();
	getchar();
	return;
}