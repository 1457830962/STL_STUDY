#pragma once
#include <iostream>

class FileMgr
{
public:
	FileMgr();
	~FileMgr();
public:
	static bool FileExist(const std::string& strFile);

};