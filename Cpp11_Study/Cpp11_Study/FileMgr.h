#pragma once
#include <iostream>



namespace File {
	class FileMgr
	{
	public:
		FileMgr();
		~FileMgr();
	public:
		static bool FileExist(const std::string& strFile);
		static std::string GetProgramDir();
		static bool SetProgramCompanyName(std::string& strCompanyName, std::string& strCompanyEnglishName);

	};



	class OutputLogInfo
	{
	public:
		OutputLogInfo();
		~OutputLogInfo();
	public:
		static 	void OutputDebugStringEx(const wchar_t* strOutputString, ...);
		static void OutputDebugStringEx(const char* strOutputString, ...);

	};
}