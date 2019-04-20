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
	//����һЩ������Ԫ���ǽṹ��ģ������ڽṹ�����涨��ȽϺ��������水��id,length,width��������
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
	//��������
	vector<int> vi;
	vi.push_back(12);  //����Ǵ�������ĩβ����Ԫ��
	vi.push_back(13);
	vi.pop_back();   //��ĩβɾ��Ԫ��
	vi.push_back(32);
	vi.push_back(64);
	vector<int>::iterator iter; //ͨ������������vector�е�Ԫ��
	cout << "/ͨ������������vector�е�Ԫ��" << ": ";
	for (iter = vi.begin(); iter != vi.end(); iter++)
	{
		cout << *iter << " ";
	}
	cout << endl;

	vi.insert(vi.begin() + 1, 10);  //��ָ����λ�ò���Ԫ��
	vi.insert(vi.end() - 1, 3, 100);  //��ָ����λ��ǰ����Ԫ��
	cout << "ָ����λ�ú�ָ��λ��֮ǰ����Ԫ��" << ": ";
	for (iter = vi.begin(); iter != vi.end(); iter++){
		cout << *iter << " ";
	}
	cout << endl;

	cout << "ɾ��ָ��λ�õ�Ԫ��" << ": ";
	vi.erase(vi.begin() + 1);  //ɾ��ָ��λ�õ�Ԫ��
	for (iter = vi.begin(); iter != vi.end(); iter++){
		cout << *iter << " ";
	}
	cout << endl;

	vi.erase(vi.begin() + 1, vi.end());  //�����ɾ��ָ�������Ԫ�أ�����Ϊ[i,j]
	cout << "ɾ��ָ�������Ԫ��" << ": ";
	for (iter = vi.begin(); iter != vi.end(); iter++){
		cout << *iter << " ";
	}
	cout << endl;

	//��������
	vector <A> vec;
	A a;  //��ʼ��һ���ṹ��Ԫ��
	a.id = 1;
	a.length = 2;
	a.width = 3;

	vec.push_back(a);//�����������
	vector<A>::iterator ite;
	ite = vec.begin();
	cout << (*ite).id << " " << (*ite).length << " " << (*ite).width << endl;

	std::ifstream is("data.txt");
	const int line_size = 500;
	char line[line_size];

	is.seekg(0, is.end);
	int file_size = is.tellg();//����λ��ָʾ  ���統ǰ����txt�е�һ��  file_size:�ַ�������
	is.seekg(0, is.beg);

	int line_number = 1;
	std::vector< std::vector<double> > result;
	
	while (is.good()) {   //good����������δ���ִ����򷵻�Ϊtrue
		is.getline(line, file_size); //Ĭ�Ͻ�����ΪEnter  ����txt��ÿһ��ȡһ��
		if (strlen(line) == 0) {   //��txt�����ǿո�ʱѭ������
			continue;
		}
		std::stringstream ss;
		ss << line;
		char number_str[300];
		std::vector<double> line_doubles;    //��ʱ�������� ��������ʱ���� ����Ὣǰ���ֵ�ظ�ѹ������
		while (ss.good()) {
			ss.getline(number_str, line_size, ' ');
			if (strlen(number_str) == 0) { //��Ϊ�ո���ѭ������
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
		cout << "ˮƽƫ������" << result[i][1] << endl;
		cout << "��ֱƫ������" << result[i][2] << endl;
		cout << "Ťת����" << result[i][3] << endl;	
	}
	//std::cout << "get result size:" << result.size() << std::endl;
	
	is.close();
	system("pause");
}