#pragma once
#include <iostream>

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