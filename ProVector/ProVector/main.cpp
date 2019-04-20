#include<iostream>
#include<vector>
#include<stdio.h>
#include <iosfwd>
#include <string.h>
#include <fstream>
#include <sstream>
using namespace std;

typedef struct A{
	int id;
	int length;
	int width;
	//对于一些向量的元素是结构体的，可以在结构体里面定义比较函数，下面按照id,length,width升序排序
	bool operator<(const A &a)const{
		if (id != a.id)
			return id < a.id;
		else{
			if (length != a.length)
				return length < a.length;
			else
				return width < a.width;
		}
	}
}A;

int main(){
	//容器测试
	vector<int> vi;
	vi.push_back(12);  //这个是从向量的末尾插入元素
	vi.push_back(13);
	vi.pop_back();   //从末尾删除元素
	vi.push_back(32);
	vi.push_back(64);
	vector<int>::iterator iter; //通过迭代器遍历vector中的元素
	cout << "/通过迭代器遍历vector中的元素" << ": ";
	for (iter = vi.begin(); iter != vi.end(); iter++)
	{
		cout << *iter << " ";
	}
	cout << endl;

	vi.insert(vi.begin() + 1, 10);  //在指定的位置插入元素
	vi.insert(vi.end() - 1, 3, 100);  //在指定的位置前插入元素
	cout << "指定的位置和指定位置之前插入元素" << ": ";
	for (iter = vi.begin(); iter != vi.end(); iter++){
		cout << *iter << " ";
	}
	cout << endl;

	cout << "删除指定位置的元素" << ": ";
	vi.erase(vi.begin() + 1);  //删除指定位置的元素
	for (iter = vi.begin(); iter != vi.end(); iter++){
		cout << *iter << " ";
	}
	cout << endl;

	vi.erase(vi.begin() + 1, vi.end());  //这个是删除指定区间的元素，区间为[i,j]
	cout << "删除指定区间的元素" << ": ";
	for (iter = vi.begin(); iter != vi.end(); iter++){
		cout << *iter << " ";
	}
	cout << endl;

	//向量测试
	vector <A> vec;
	A a;  //初始化一个结构体元素
	a.id = 1;
	a.length = 2;
	a.width = 3;

	vec.push_back(a);//插入进向量中
	vector<A>::iterator ite;
	ite = vec.begin();
	cout << (*ite).id << " " << (*ite).length << " " << (*ite).width << endl;

	std::ifstream is("data.txt");
	const int line_size = 500;
	char line[line_size];

	is.seekg(0, is.end);
	int file_size = is.tellg();//返回位置指示  例如当前是在txt中第一行  file_size:字符串总数
	is.seekg(0, is.beg);

	int line_number = 1;
	std::vector< std::vector<double> > result;
	
	while (is.good()) {   //good函数：若流未出现错误，则返回为true
		is.getline(line, file_size); //默认结束符为Enter  所以txt中每一行取一次
		if (strlen(line) == 0) {   //若txt首行是空格时循环继续
			continue;
		}
		std::stringstream ss;
		ss << line;
		char number_str[300];
		std::vector<double> line_doubles;    //临时容器变量 必须是临时变量 否则会将前面的值重复压入容器
		while (ss.good()) {
			ss.getline(number_str, line_size, ' ');
			if (strlen(number_str) == 0) { //若为空格则循环继续
				continue;
			}
			line_doubles.push_back(atof(number_str));
			//std::cout << "get number:" << atof(number_str) << std::endl;
		}
		//std::cout << "get line_doubles size:" << line_doubles.size() << std::endl;    
		result.push_back(line_doubles);
		//std::cout << "get line" << "(" << line_number++ <<"):" << line << std::endl;
	}
	
	for (size_t i = 0; i<result.size(); i++)
	{		
		cout << "水平偏移量：" << result[i][1] << endl;
		cout << "垂直偏移量：" << result[i][2] << endl;
		cout << "扭转量：" << result[i][3] << endl;	
	}
	//std::cout << "get result size:" << result.size() << std::endl;
	
	is.close();
	system("pause");
}