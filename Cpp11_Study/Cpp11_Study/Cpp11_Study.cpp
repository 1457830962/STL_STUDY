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
        //weak_ptr对象也绑定到shared_ptr所指向的对象。
        wp = p;
        cout << "use_count: " << wp.use_count() << endl;
    }
    //wp是弱类型的智能指针，不影响所指向对象的生命周期，
    //这里p已经析构，其所指的对象也析构了，因此输出是0

    /******************************判断文件是否存在测试*****************************************/
    std::string strExpiryDateFile = ".\\Cpp11_Study.cpp";
    //判断文件是否存在 
    if (!File::FileMgr::FileExist(strExpiryDateFile))
        MessageBoxA(::GetActiveWindow(), "有效期文件不存在!", "提示", MB_OK | MB_ICONERROR);
    /**********************************************************************************************/


    /******************************注册表创建 获取测试*****************************************/
    std::string strCompanyName = "英语";
    std::string strEnglishCompanyName = "english";
    std::string strValue;
    File::FileMgr::SetProgramCompanyName(strCompanyName, strEnglishCompanyName);
    strValue = File::FileMgr::GetProgramDir();
    OutputDebugString(L"输出调试信息123");
    //:注意HKEY_LOCAL_MACHINE 中
    /******************************注册表创建 获取测试*****************************************/

    /******************************测试耗时*****************************************/

    dTime = timer.Lapse() * 1000.0;
    strTime = std::to_string(dTime);

    /******************************测试耗时*****************************************/

    /******************************测试内存泄漏问题*****************************************/
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(213);
    char* pChars = new char[10];
    delete []pChars;
    /******************************测试内存泄漏问题*****************************************/

    /******************************测试在output窗口输出日志信息*****************************************/
    OutputDebugString(L"\r");
    std::string str = "输出调试\n信测试换行\r测试回车，总共是三行";
    //OutputDebugString(str.c_str());
    File::OutputLogInfo::OutputDebugStringEx("DEBUG_INFO============================================ | %d %s\r\n", 5678, str.c_str());
    /******************************测试在output窗口输出日志信息*****************************************/



    /******************************测试智能指针*****************************************/
    std::shared_ptr<std::string> p2 = make_shared<std::string>("hello11111111");//无需手动释放
    std::cout <<"p2的值为:"<< *p2.get() << std::endl;
    //delete p2.get();
    if (p2 != nullptr)//判断是否为空
        std::cout<< "is not empty!"<< std::endl;


    //_CrtSetBreakAlloc(220);
    std::string* pstr = new std::string("1111111111111");//需手动释放
    std::cout << "pstr的值为:" << *pstr << std::endl;
    if(pstr != nullptr)
		delete pstr;

    std::unique_ptr<std::vector<std::shared_ptr<SmartPointer::Test>>> vec_ptr;
    vec_ptr = SmartPointer::TestHelper::make();
    std::cout << "How many data points do you want to enter: ";
    int num;
    std::cin >> num;
    SmartPointer::TestHelper::fill(*vec_ptr, num);
    SmartPointer::TestHelper::display(*vec_ptr);
    /******************************测试智能指针*****************************************/





    /******************************内存泄漏问题定位*****************************************/
    //方法出处  https://www.cnblogs.com/skynet/archive/2011/02/20/1959162.html  
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1);
    char* pChars = new char[10];//此处发生内存泄漏，调试的输出窗口会有提示
    //delete []pChars;

    _CrtMemCheckpoint(&s2);
    if (_CrtMemDifference(&s3, &s1, &s2))
        _CrtMemDumpStatistics(&s3);

    /******************************内存泄漏问题定位*****************************************/



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
