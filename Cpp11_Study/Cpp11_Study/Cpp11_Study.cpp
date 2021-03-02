// Cpp11_Study.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <io.h>
#include <Windows.h>
#include "FileMgr.h"
using namespace std;

int main()
{
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
    cout << "use_count: " << wp.use_count() << endl;

    int count = 0;

    int x = 9999;

    while (x){   
        count++;
        x = x & (x - 1);
    }


    std::string strExpiryDateFile = "D:\\Github\\STUDY_EVERYDAY\\Cpp11_Study\\Cpp11_Study\\Cpp11_Study.cpp";
    //�ж��ļ��Ƿ���� 
    if (FileMgr::FileExist(strExpiryDateFile))
        MessageBoxA(::GetActiveWindow(), "��Ч���ļ�����!", "��ʾ", MB_OK | MB_ICONERROR);
    else
        MessageBoxA(::GetActiveWindow(), "��Ч���ļ�������!", "��ʾ", MB_OK | MB_ICONERROR);

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
