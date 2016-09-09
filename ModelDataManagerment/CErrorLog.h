/************************************************************************
* ������־��															*
************************************************************************/

/************************************************************************
* ���ߣ����޹�                                                          *
* ʱ�䣺2015.1.15														*
************************************************************************/

#ifndef _CERROR_LOG_H_
#define _CERROR_LOG_H_


class CErrorLog {

public:
	CErrorLog(std::string filename = "errorLog.txt");

	~CErrorLog();

	// ��¼������Ϣ
	void Write(const std::string& errorFile, int line, const std::string& errorMsg);	

private:
	// ������־�ļ�
	std::ofstream errorLog_;		
};

#endif

