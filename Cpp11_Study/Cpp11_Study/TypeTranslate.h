#pragma once

#include <iostream>
#include <Windows.h>
using namespace std;

class TypeTranslate
{
public:
    TypeTranslate(void);
    ~TypeTranslate(void);

public:
    static std::wstring string2wstring(std::string str);
    static std::string wstring2string(std::wstring wstr);

};


//��stringת����std::wstring  
std::wstring TypeTranslate::string2wstring(std::string str)
{
    std::wstring result;
    //��ȡ��������С��������ռ䣬��������С���ַ�����  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    //���ֽڱ���ת���ɿ��ֽڱ���  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';             //����ַ�����β  
    //ɾ��������������ֵ  
    result.append(buffer);
    delete[] buffer;
    return result;
}

//��std::wstringת����string  
std::string TypeTranslate::wstring2string(std::wstring wstr)
{
    string result;
    //��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    //���ֽڱ���ת���ɶ��ֽڱ���  
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //ɾ��������������ֵ  
    result.append(buffer);
    delete[] buffer;
    return result;
}