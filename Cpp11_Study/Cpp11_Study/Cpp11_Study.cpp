// Cpp11_Study.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <io.h>
#include <Windows.h>
#include <string>
#include "FileMgr.h"
#include "./Global/SuperTimer.h"
#include "SmartPointer.h"
using namespace std;

int main()
{

    CSuperTimer timer;
    DOUBLE dTime = 0.0;
    std::string strTime;
    //std::cout << "Hello World!\n";
    //default consstructor
    weak_ptr<string> wp;

    {
        shared_ptr<string> p = make_shared<string>("hello world!\n");
        //weak_ptr����Ҳ�󶨵�shared_ptr��ָ��Ķ���
        wp = p;
        cout << "use_count: " << wp.use_count() << endl;
    }
    //wp�������͵�����ָ�룬��Ӱ����ָ�������������ڣ�
    //����p�Ѿ�����������ָ�Ķ���Ҳ�����ˣ���������0

    /******************************�ж��ļ��Ƿ���ڲ���*****************************************/
    std::string strExpiryDateFile = ".\\Cpp11_Study.cpp";
    //�ж��ļ��Ƿ���� 
    if (!File::FileMgr::FileExist(strExpiryDateFile))
        MessageBoxA(::GetActiveWindow(), "��Ч���ļ�������!", "��ʾ", MB_OK | MB_ICONERROR);
    /**********************************************************************************************/


    /******************************ע����� ��ȡ����*****************************************/
    std::string strCompanyName = "Ӣ��";
    std::string strEnglishCompanyName = "english";
    std::string strValue;
    File::FileMgr::SetProgramCompanyName(strCompanyName, strEnglishCompanyName);
    strValue = File::FileMgr::GetProgramDir();
    OutputDebugString(L"���������Ϣ123");
    //:ע��HKEY_LOCAL_MACHINE ��
    /******************************ע����� ��ȡ����*****************************************/

    /******************************���Ժ�ʱ*****************************************/

    dTime = timer.Lapse() * 1000.0;
    strTime = std::to_string(dTime);

    /******************************���Ժ�ʱ*****************************************/

    /******************************�����ڴ�й©����*****************************************/
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(213);
    char* pChars = new char[10];
    delete []pChars;
    /******************************�����ڴ�й©����*****************************************/

    /******************************������output���������־��Ϣ*****************************************/
    OutputDebugString(L"\r");
    std::string str = "�������\n�Ų��Ի���\r���Իس����ܹ�������";
    //OutputDebugString(str.c_str());
    File::OutputLogInfo::OutputDebugStringEx("DEBUG_INFO============================================ | %d %s\r\n", 5678, str.c_str());
    /******************************������output���������־��Ϣ*****************************************/



    /******************************��������ָ��*****************************************/
    std::shared_ptr<std::string> p2 = make_shared<std::string>("hello11111111");//�����ֶ��ͷ�
    std::cout <<"p2��ֵΪ:"<< *p2.get() << std::endl;
    //delete p2.get();
    if (p2 != nullptr)//�ж��Ƿ�Ϊ��
        std::cout<< "is not empty!"<< std::endl;


    //_CrtSetBreakAlloc(220);
    std::string* pstr = new std::string("1111111111111");//���ֶ��ͷ�
    std::cout << "pstr��ֵΪ:" << *pstr << std::endl;
    if(pstr != nullptr)
		delete pstr;

    std::unique_ptr<std::vector<std::shared_ptr<SmartPointer::Test>>> vec_ptr;
    vec_ptr = SmartPointer::TestHelper::make();
    std::cout << "How many data points do you want to enter: ";
    int num;
    std::cin >> num;
    SmartPointer::TestHelper::fill(*vec_ptr, num);
    SmartPointer::TestHelper::display(*vec_ptr);
    /******************************��������ָ��*****************************************/





    /******************************�ڴ�й©���ⶨλ*****************************************/
    //��������  https://www.cnblogs.com/skynet/archive/2011/02/20/1959162.html  
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1);
    char* pChars = new char[10];//�˴������ڴ�й©�����Ե�������ڻ�����ʾ
    //delete []pChars;

    _CrtMemCheckpoint(&s2);
    if (_CrtMemDifference(&s3, &s1, &s2))
        _CrtMemDumpStatistics(&s3);

    /******************************�ڴ�й©���ⶨλ*****************************************/



}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
