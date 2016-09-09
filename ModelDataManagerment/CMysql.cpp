/************************************************************************
* 数据库操作类															*
************************************************************************/

/************************************************************************
* 作者：张胜                                                           *
* 时间：2015.1.18														*
************************************************************************/
#include "stdafx.h"
#include "CMysql.h"

using std::vector;
using std::string;

//////////////////////////////////////////////////////////////////////////
// 构造函数
//////////////////////////////////////////////////////////////////////////
CMysql::CMysql(const string& host, const string& username,
	const string& passwd, const string& database,
	unsigned int port) : conn_(NULL), resptr_(NULL) {

		// 打开数据库连接
		MysqlConnectWrap(host, username, passwd, database, port);	
}

//////////////////////////////////////////////////////////////////////////
// 析构函数
//////////////////////////////////////////////////////////////////////////
CMysql::~CMysql() {

	// 关闭数据对象
	if (resptr_ != NULL) {
		mysql_free_result(resptr_);
		resptr_ = NULL;
	}

	// 关闭数据库连接
	if (conn_ != NULL) {
		mysql_close(conn_);
		conn_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 数据库连接函数
// host:主机名或IP地址
// username:用户名
// passwd:密码
// database:数据库名
// port:端口号
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlConnectWrap(const string& host, const string& username, 
	const string& passwd, const string& database, 
	unsigned int port) {

		// 初始化连接句柄
		conn_ = mysql_init(NULL);
		mysql_options(conn_, MYSQL_SET_CHARSET_NAME, "gb2312");

		if (conn_ == NULL) {
#ifdef _DEBUG
			CErrorLog errorLog;
			errorLog.Write(__FILE__, __LINE__, "初始化数据库连接句柄失败！");
#endif
			return 1;
		}

		// 连接数据库
		conn_ = mysql_real_connect(conn_, host.c_str(), username.c_str(), 
			passwd.c_str(), database.c_str(), port, NULL, 0);
		if (conn_ == NULL) {
#ifdef _DEBUG
			CErrorLog errorLog;
			errorLog.Write(__FILE__, __LINE__, "连接数据库失败！");
#endif      
			return 2;
		}
}

//////////////////////////////////////////////////////////////////////////
// 释放已查询的数据，可以再次查询，但不释放连接
//////////////////////////////////////////////////////////////////////////
void CMysql::MysqlFreeWrap() {

	// 关闭数据对象
	if (resptr_ != NULL) {
		mysql_free_result(resptr_);
		resptr_ = NULL;
	}

	// 清空数据列名称
	colname_.clear();
}

//////////////////////////////////////////////////////////////////////////
// 插入数据
// query:执行语句
// 返回值：成功返回0，失败返回-1
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlInsertWrap(const std::string& query) {

	return query.empty() || mysql_query(conn_, query.c_str()) ? -1 : 0;
}

//////////////////////////////////////////////////////////////////////////
// 更改数据
// query:执行语句
// 返回值：成功返回0，失败返回-1
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlUpdateWrap(const std::string& query) {

	return query.empty() || mysql_query(conn_, query.c_str()) ? -1 : 0;
}

//////////////////////////////////////////////////////////////////////////
// 删除数据
// query:执行语句
// 返回值：成功返回0，失败返回-1
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlDeleteWrap(const std::string& query) {

	return query.empty() || mysql_query(conn_, query.c_str()) ? -1 : 0;
}

//////////////////////////////////////////////////////////////////////////
// 查询数据
// query:执行语句
// 返回值：返回查询到的记录数，失败返回-1
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlQueryWrap(const std::string& query) {

	// 执行语句为空
	if (query.empty()) {
		return -1;
	}

	//增加时间判断
	/*DWORD t1 = timeGetTime();*/

	if (mysql_query(conn_, query.c_str()) != 0) {

	 //   DWORD t2 = timeGetTime();

		//if ((t2-t1)/1000.0 > 10)//如果10s还没连上服务器，就认为无法连接
		//{
		//	AfxMessageBox("无法连接到服务器！");

		//}

		return -1;
	}

	if ((resptr_ = mysql_store_result(conn_)) == NULL) {
		return -1;
	}

	// 将查找到数据的所有列成员保存
	MYSQL_FIELD* fieldPtr = NULL;
	while ((fieldPtr = mysql_fetch_field(resptr_)) != NULL) {
		colname_.push_back(fieldPtr->name);
	}

	// 返回查询到的记录数
	return MysqlNumRowsWrap();
}

//////////////////////////////////////////////////////////////////////////
// 通过下标找到指定列
// idx:列下标
// 返回值：指定列的值
//////////////////////////////////////////////////////////////////////////
string CMysql::Proxy::operator[] (unsigned int idx) const {

	// 数据对象为空
	if (base_->resptr_ == NULL) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "读取数据时，数据对象为空");
#endif
		return string("");
	}

	// 判断下标是否超出列范围
	if (idx >= base_->MysqlFieldCountWrap()) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "读取数据时，下标是否超出列范围");
#endif
		return string("");
	}

	// 获取指定行
	base_->sqlrow_ = mysql_fetch_row(base_->resptr_);

	// 返回指定列
	return base_->sqlrow_[idx];
}

//////////////////////////////////////////////////////////////////////////
// 通过关键字找到指定列
// key:列名称关键字
// 返回值：指定列的值
//////////////////////////////////////////////////////////////////////////
string CMysql::Proxy::operator[] (const string& key) const {

	// 数据对象为空
	if (base_->resptr_ == NULL) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "读取数据时，数据对象为空");
#endif
		return string("");
	}

	// 查找给定关键字列
	unsigned int idx = 0;
	vector<string>::const_iterator iter;
	for (iter = base_->colname_.begin(); iter != base_->colname_.end(); ++iter) {
		if (key == *iter) {
			break;
		}
		++idx;
	}

	// 未找到给定关键字的列
	if (iter == base_->colname_.end()) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "读取数据时，给定关键字不存在");
#endif
		return string("");
	}

	return (*this)[idx];
}

//////////////////////////////////////////////////////////////////////////
// 通过下标找到指定行
// idx:列下标
//////////////////////////////////////////////////////////////////////////
const CMysql::Proxy CMysql::operator[] (unsigned int idx) {

	// 数据对象为空
	if (resptr_ == NULL) {
		return Proxy(this);
	}

	// 判断下标是否超出行范围
	if (idx >= MysqlNumRowsWrap()) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "读取数据时，下标是否超出行范围");
#endif
		idx = 0;
	}

	// 在数据对象中偏移到指定行
	mysql_data_seek(resptr_, idx);

	return Proxy(this);
}

//////////////////////////////////////////////////////////////////////////
// 计算数据对象的行数
//////////////////////////////////////////////////////////////////////////
unsigned int CMysql::MysqlNumRowsWrap() {

	// 数据对象不为空
	if (resptr_ != NULL) {
		return (unsigned int)mysql_num_rows(resptr_);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 计算数据对象的列数
//////////////////////////////////////////////////////////////////////////
unsigned int CMysql::MysqlFieldCountWrap() {

	// 连接对象与数据对象不为空
	if (conn_ != NULL && resptr_ != NULL) {
		return mysql_field_count(conn_);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// 通过关键字找到指定行的下标
// key:列名称关键字，table:数据库表的名称,keyValue:要查找的对应关键字的值
// 返回值：指定行的位置
//////////////////////////////////////////////////////////////////////////
unsigned int CMysql::MysqlFindMethod(const std::string & key,const std::string & table,const std::string & keyValue)
{
	std::string sql = "select * from " + table;//遍历查询一遍
	int totalRows = 0;
	if( this->MysqlQueryWrap(sql) != -1){
		totalRows  = this->MysqlQueryWrap(sql); //通过查询找到的表的所有行数目
	}
    int index = 0 ;
	while(index < totalRows)
	{
		if ( (*this)[index][key] != keyValue ) 
		  {
			  ++index;
			  continue;
		}
		else
			  break;
	}
	return index;
}