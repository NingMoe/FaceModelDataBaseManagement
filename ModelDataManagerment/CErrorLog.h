/************************************************************************
* 错误日志类															*
************************************************************************/

/************************************************************************
* 作者：缪洲光                                                          *
* 时间：2015.1.15														*
************************************************************************/

#ifndef _CERROR_LOG_H_
#define _CERROR_LOG_H_


class CErrorLog {

public:
	CErrorLog(std::string filename = "errorLog.txt");

	~CErrorLog();

	// 记录错误信息
	void Write(const std::string& errorFile, int line, const std::string& errorMsg);	

private:
	// 错误日志文件
	std::ofstream errorLog_;		
};

#endif

