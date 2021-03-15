// Cpp11_Study.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <io.h>
#include <Windows.h>
#include <string>
#include "FileMgr.h"
#include "./Global/SuperTimer.h"
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
    std::string strExpiryDateFile = "D:\\Github\\STUDY_EVERYDAY\\Cpp11_Study\\Cpp11_Study\\Cpp11_Study.cpp";
    //判断文件是否存在 
    if (!FileMgr::FileExist(strExpiryDateFile))
        MessageBoxA(::GetActiveWindow(), "有效期文件不存在!", "提示", MB_OK | MB_ICONERROR);
    /**********************************************************************************************/


    /******************************注册表创建 获取测试*****************************************/
    std::string strCompanyName = "英语";
    std::string strEnglishCompanyName = "english";
    std::string strValue;
    FileMgr::SetProgramCompanyName(strCompanyName, strEnglishCompanyName);
    strValue = FileMgr::GetProgramDir();
    OutputDebugString(L"输出调试信息123");
    //:注意HKEY_LOCAL_MACHINE 中
    /******************************注册表创建 获取测试*****************************************/

    /******************************测试耗时*****************************************/

    dTime = timer.Lapse() * 1000.0;
    strTime = std::to_string(dTime);

    /******************************测试耗时*****************************************/

    /******************************测试内存泄漏问题*****************************************/
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(213);
    char* pChars = new char[10];
    //delete []pChars;
    /******************************测试内存泄漏问题*****************************************/
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
