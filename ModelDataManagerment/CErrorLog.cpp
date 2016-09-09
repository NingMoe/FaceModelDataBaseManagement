/************************************************************************
* ������־��															*
************************************************************************/

/************************************************************************
* ���ߣ����޹�                                                          *
* ʱ�䣺2015.1.15														*
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
// ���캯��
CErrorLog::CErrorLog(string filename) {

	// ����ӷ�ʽ���ļ�
	errorLog_.open(filename.c_str(), ios::app); 	
}

//////////////////////////////////////////////////////////////////////////
// ��������
CErrorLog::~CErrorLog() {

	// �ر��ļ���
	errorLog_.close();
}

//////////////////////////////////////////////////////////////////////////
// ��¼������Ϣ
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

