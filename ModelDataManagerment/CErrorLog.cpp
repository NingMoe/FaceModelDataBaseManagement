/************************************************************************
* 错误日志类															*
************************************************************************/

/************************************************************************
* 作者：缪洲光                                                          *
* 时间：2015.1.15														*
************************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include "CErrorLog.h"

using std::string;
using std::ios;
using std::endl;

//////////////////////////////////////////////////////////////////////////
// 构造函数
CErrorLog::CErrorLog(string filename) {

	// 以添加方式打开文件
	errorLog_.open(filename.c_str(), ios::app); 	
}

//////////////////////////////////////////////////////////////////////////
// 析构函数
CErrorLog::~CErrorLog() {

	// 关闭文件流
	errorLog_.close();
}

//////////////////////////////////////////////////////////////////////////
// 记录错误信息
void CErrorLog::Write(const string& errorFile, int line, const string& errorMsg) {

	//time_t timeStamp = time(NULL);
	//struct tm* stm = localtime(&timeStamp);
	const time_t timeStamp = time(NULL);
	struct tm stm;
	localtime_s(&stm, &timeStamp);
	errorLog_ << (stm.tm_year+1900) << "-"
			<< (stm.tm_mon+1) << "-"
			<< stm.tm_mday << " "
			<< stm.tm_hour << ":"
			<< stm.tm_min << " "
			<< errorFile << "("
			<< line << "):"
			<< errorMsg << endl;
}

