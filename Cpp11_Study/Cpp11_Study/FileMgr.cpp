#include <io.h>
//#include <Windows.h>
#include "RegKeyEx.h"
#include "FileMgr.h"
#include "Define.h"
#include "TypeTranslate.h"
using namespace std;

namespace File {

	FileMgr::FileMgr()
	{
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetBreakAlloc(209);
	}

	FileMgr::~FileMgr()
	{
	}



	bool FileMgr::FileExist(const std::string& strFile)
	{
		bool b = true;
		//判断文件是否存在 
		if (_access(strFile.c_str(), 0) == -1)
			b = false;
		return b;
	}

	//创建键值
	bool FileMgr::SetProgramCompanyName(std::string& strCompanyName, std::string& strCompanyEnglishName)
	{
		MyREG::CRegKeyEx key;

		std::wstring wstrCompanyName = TypeTranslate::string2wstring(strCompanyName);
		std::wstring wstrCompanyEnglishName = TypeTranslate::string2wstring(strCompanyEnglishName);

		if (key.OpenA(HKEY_CURRENT_USER, REG_PROGRAM_PATH, KEY_WRITE) == ERROR_SUCCESS)
		{
		}
		else
			key.CreateA(HKEY_CURRENT_USER, REG_PROGRAM_PATH);

		if (key.OpenA(HKEY_LOCAL_MACHINE, REG_PROGRAM_PATH, KEY_WRITE) == ERROR_SUCCESS)
		{
		}
		else
			key.CreateA(HKEY_LOCAL_MACHINE, REG_PROGRAM_PATH);

		std::string strRegKeyName;
		if (key.OpenA(HKEY_CURRENT_USER, REG_PROGRAM_PATH, KEY_WRITE) == ERROR_SUCCESS &&
			key.SetStringValueW(_T(REG_KEY_COMPANY_NAME), wstrCompanyName.c_str()) == ERROR_SUCCESS &&
			key.SetStringValueW(_T(REG_KEY_COMPANY_ENGLISH_NAME), wstrCompanyEnglishName.c_str()) == ERROR_SUCCESS)
		{
		}
		else if (key.OpenA(HKEY_CURRENT_USER, REG_PROGRAM_PATH, KEY_WRITE | KEY_WOW64_32KEY) == ERROR_SUCCESS &&
			key.SetStringValueW(_T(REG_KEY_COMPANY_NAME), wstrCompanyName.c_str()) == ERROR_SUCCESS &&
			key.SetStringValueW(_T(REG_KEY_COMPANY_ENGLISH_NAME), wstrCompanyEnglishName.c_str()) == ERROR_SUCCESS)
		{
		}

		if (key.OpenA(HKEY_LOCAL_MACHINE, REG_PROGRAM_PATH, KEY_WRITE) == ERROR_SUCCESS &&
			key.SetStringValueW(_T(REG_KEY_COMPANY_NAME), wstrCompanyName.c_str()) == ERROR_SUCCESS &&
			key.SetStringValueW(_T(REG_KEY_COMPANY_ENGLISH_NAME), wstrCompanyEnglishName.c_str()) == ERROR_SUCCESS)
		{
			return true;
		}
		else if (key.OpenA(HKEY_LOCAL_MACHINE, REG_PROGRAM_PATH, KEY_WRITE | KEY_WOW64_32KEY) == ERROR_SUCCESS &&
			key.SetStringValueW(_T(REG_KEY_COMPANY_NAME), wstrCompanyName.c_str()) == ERROR_SUCCESS &&
			key.SetStringValueW(_T(REG_KEY_COMPANY_ENGLISH_NAME), wstrCompanyEnglishName.c_str()) == ERROR_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	//  获取注册表值
	std::string FileMgr::GetProgramDir()
	{
		MyREG::CRegKeyEx key;

		char InstDir[1000];
		ULONG nNameLen = sizeof(InstDir);
		if (key.OpenA(HKEY_LOCAL_MACHINE, REG_PROGRAM_PATH, KEY_READ) == ERROR_SUCCESS &&
			key.QueryStringValueA(REG_KEY_COMPANY_NAME, InstDir, &nNameLen) == ERROR_SUCCESS)
		{
			std::string strInstDir = InstDir;
			strInstDir += "\\";
			return strInstDir;
		}
		else if (key.OpenA(HKEY_LOCAL_MACHINE, REG_PROGRAM_PATH, KEY_READ | KEY_WOW64_32KEY) == ERROR_SUCCESS &&
			key.QueryStringValueA(REG_KEY_COMPANY_NAME, InstDir, &nNameLen) == ERROR_SUCCESS)
		{
			std::string strInstDir = InstDir;
			strInstDir += "\\";
			return strInstDir;
		}
		else
			return std::string();
	}


	//==================================================
	OutputLogInfo::OutputLogInfo()
	{
	}

	OutputLogInfo::~OutputLogInfo()
	{
	}
	void OutputLogInfo::OutputDebugStringEx(const wchar_t* strOutputString, ...)
	{
		va_list vlArgs = NULL;
		va_start(vlArgs, strOutputString);
		size_t nLen = _vscwprintf(strOutputString, vlArgs) + 1;
		wchar_t* strBuffer = new wchar_t[nLen];
		_vsnwprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);
		va_end(vlArgs);
		OutputDebugStringW(strBuffer);
		delete[] strBuffer;
	}

	void OutputLogInfo::OutputDebugStringEx(const char* strOutputString, ...)
	{
		va_list vlArgs = NULL;
		va_start(vlArgs, strOutputString);
		size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;
		char* strBuffer = new char[nLen];
		_vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);
		va_end(vlArgs);
		OutputDebugStringA(strBuffer);
		delete[] strBuffer;
	}



}