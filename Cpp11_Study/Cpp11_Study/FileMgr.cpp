#include <io.h>
//#include <Windows.h>
#include "FileMgr.h"
using namespace std;

bool FileMgr::FileExist(const std::string& strFile)
{
	bool b = true;
	//�ж��ļ��Ƿ���� 
	if (_access(strFile.c_str(), 0) == -1)
		b = false;
	return b;
}