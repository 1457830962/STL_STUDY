#include "iostream"
#include "vector"
using namespace std;

class VectorRef
{
public:
	std::vector<int> vecInts;
public:
	VectorRef(int size);
	~VectorRef();
	std::vector<int>& GetVecInts()//ʹ���龰֮һ��������Ϊ��������ֵ������ʹ�����ý��ᷢ���������죨�ٶ�����
	{
		return vecInts;
	}
};

//���캯��
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

//��ӡ����
void PrintVecInts(const std::vector<int>& vecInts)
{
	printf("\n");
	for (size_t i = 0; i < vecInts.size(); i++)
	{
		printf("%d current value = %d\n", i, vecInts[i]);
	}
}

//��������
void TestVecInts()
{
	VectorRef vRef(4);
	vector<int>& v = vRef.GetVecInts();//ʹ���龰֮������ֵʱʹ�����ã�����ͬ���ᷢ���������죨�ٶ�����
	v[0] = 100;
	PrintVecInts(v);
	PrintVecInts(vRef.GetVecInts());
}

void main()
{
	TestVecInts();
	getchar();
	return;
}