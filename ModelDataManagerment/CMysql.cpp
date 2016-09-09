/************************************************************************
* ���ݿ������															*
************************************************************************/

/************************************************************************
* ���ߣ���ʤ                                                           *
* ʱ�䣺2015.1.18														*
************************************************************************/
#include "stdafx.h"
#include "CMysql.h"

using std::vector;
using std::string;

//////////////////////////////////////////////////////////////////////////
// ���캯��
//////////////////////////////////////////////////////////////////////////
CMysql::CMysql(const string& host, const string& username,
	const string& passwd, const string& database,
	unsigned int port) : conn_(NULL), resptr_(NULL) {

		// �����ݿ�����
		MysqlConnectWrap(host, username, passwd, database, port);	
}

//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////
CMysql::~CMysql() {

	// �ر����ݶ���
	if (resptr_ != NULL) {
		mysql_free_result(resptr_);
		resptr_ = NULL;
	}

	// �ر����ݿ�����
	if (conn_ != NULL) {
		mysql_close(conn_);
		conn_ = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// ���ݿ����Ӻ���
// host:��������IP��ַ
// username:�û���
// passwd:����
// database:���ݿ���
// port:�˿ں�
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlConnectWrap(const string& host, const string& username, 
	const string& passwd, const string& database, 
	unsigned int port) {

		// ��ʼ�����Ӿ��
		conn_ = mysql_init(NULL);
		mysql_options(conn_, MYSQL_SET_CHARSET_NAME, "gb2312");

		if (conn_ == NULL) {
#ifdef _DEBUG
			CErrorLog errorLog;
			errorLog.Write(__FILE__, __LINE__, "��ʼ�����ݿ����Ӿ��ʧ�ܣ�");
#endif
			return 1;
		}

		// �������ݿ�
		conn_ = mysql_real_connect(conn_, host.c_str(), username.c_str(), 
			passwd.c_str(), database.c_str(), port, NULL, 0);
		if (conn_ == NULL) {
#ifdef _DEBUG
			CErrorLog errorLog;
			errorLog.Write(__FILE__, __LINE__, "�������ݿ�ʧ�ܣ�");
#endif      
			return 2;
		}
}

//////////////////////////////////////////////////////////////////////////
// �ͷ��Ѳ�ѯ�����ݣ������ٴβ�ѯ�������ͷ�����
//////////////////////////////////////////////////////////////////////////
void CMysql::MysqlFreeWrap() {

	// �ر����ݶ���
	if (resptr_ != NULL) {
		mysql_free_result(resptr_);
		resptr_ = NULL;
	}

	// �������������
	colname_.clear();
}

//////////////////////////////////////////////////////////////////////////
// ��������
// query:ִ�����
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlInsertWrap(const std::string& query) {

	return query.empty() || mysql_query(conn_, query.c_str()) ? -1 : 0;
}

//////////////////////////////////////////////////////////////////////////
// ��������
// query:ִ�����
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlUpdateWrap(const std::string& query) {

	return query.empty() || mysql_query(conn_, query.c_str()) ? -1 : 0;
}

//////////////////////////////////////////////////////////////////////////
// ɾ������
// query:ִ�����
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlDeleteWrap(const std::string& query) {

	return query.empty() || mysql_query(conn_, query.c_str()) ? -1 : 0;
}

//////////////////////////////////////////////////////////////////////////
// ��ѯ����
// query:ִ�����
// ����ֵ�����ز�ѯ���ļ�¼����ʧ�ܷ���-1
//////////////////////////////////////////////////////////////////////////
int CMysql::MysqlQueryWrap(const std::string& query) {

	// ִ�����Ϊ��
	if (query.empty()) {
		return -1;
	}

	//����ʱ���ж�
	/*DWORD t1 = timeGetTime();*/

	if (mysql_query(conn_, query.c_str()) != 0) {

	 //   DWORD t2 = timeGetTime();

		//if ((t2-t1)/1000.0 > 10)//���10s��û���Ϸ�����������Ϊ�޷�����
		//{
		//	AfxMessageBox("�޷����ӵ���������");

		//}

		return -1;
	}

	if ((resptr_ = mysql_store_result(conn_)) == NULL) {
		return -1;
	}

	// �����ҵ����ݵ������г�Ա����
	MYSQL_FIELD* fieldPtr = NULL;
	while ((fieldPtr = mysql_fetch_field(resptr_)) != NULL) {
		colname_.push_back(fieldPtr->name);
	}

	// ���ز�ѯ���ļ�¼��
	return MysqlNumRowsWrap();
}

//////////////////////////////////////////////////////////////////////////
// ͨ���±��ҵ�ָ����
// idx:���±�
// ����ֵ��ָ���е�ֵ
//////////////////////////////////////////////////////////////////////////
string CMysql::Proxy::operator[] (unsigned int idx) const {

	// ���ݶ���Ϊ��
	if (base_->resptr_ == NULL) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "��ȡ����ʱ�����ݶ���Ϊ��");
#endif
		return string("");
	}

	// �ж��±��Ƿ񳬳��з�Χ
	if (idx >= base_->MysqlFieldCountWrap()) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "��ȡ����ʱ���±��Ƿ񳬳��з�Χ");
#endif
		return string("");
	}

	// ��ȡָ����
	base_->sqlrow_ = mysql_fetch_row(base_->resptr_);

	// ����ָ����
	return base_->sqlrow_[idx];
}

//////////////////////////////////////////////////////////////////////////
// ͨ���ؼ����ҵ�ָ����
// key:�����ƹؼ���
// ����ֵ��ָ���е�ֵ
//////////////////////////////////////////////////////////////////////////
string CMysql::Proxy::operator[] (const string& key) const {

	// ���ݶ���Ϊ��
	if (base_->resptr_ == NULL) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "��ȡ����ʱ�����ݶ���Ϊ��");
#endif
		return string("");
	}

	// ���Ҹ����ؼ�����
	unsigned int idx = 0;
	vector<string>::const_iterator iter;
	for (iter = base_->colname_.begin(); iter != base_->colname_.end(); ++iter) {
		if (key == *iter) {
			break;
		}
		++idx;
	}

	// δ�ҵ������ؼ��ֵ���
	if (iter == base_->colname_.end()) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "��ȡ����ʱ�������ؼ��ֲ�����");
#endif
		return string("");
	}

	return (*this)[idx];
}

//////////////////////////////////////////////////////////////////////////
// ͨ���±��ҵ�ָ����
// idx:���±�
//////////////////////////////////////////////////////////////////////////
const CMysql::Proxy CMysql::operator[] (unsigned int idx) {

	// ���ݶ���Ϊ��
	if (resptr_ == NULL) {
		return Proxy(this);
	}

	// �ж��±��Ƿ񳬳��з�Χ
	if (idx >= MysqlNumRowsWrap()) {
#ifdef _DEBUG
		CErrorLog errorLog;
		errorLog.Write(__FILE__, __LINE__, "��ȡ����ʱ���±��Ƿ񳬳��з�Χ");
#endif
		idx = 0;
	}

	// �����ݶ�����ƫ�Ƶ�ָ����
	mysql_data_seek(resptr_, idx);

	return Proxy(this);
}

//////////////////////////////////////////////////////////////////////////
// �������ݶ��������
//////////////////////////////////////////////////////////////////////////
unsigned int CMysql::MysqlNumRowsWrap() {

	// ���ݶ���Ϊ��
	if (resptr_ != NULL) {
		return (unsigned int)mysql_num_rows(resptr_);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// �������ݶ��������
//////////////////////////////////////////////////////////////////////////
unsigned int CMysql::MysqlFieldCountWrap() {

	// ���Ӷ��������ݶ���Ϊ��
	if (conn_ != NULL && resptr_ != NULL) {
		return mysql_field_count(conn_);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// ͨ���ؼ����ҵ�ָ���е��±�
// key:�����ƹؼ��֣�table:���ݿ�������,keyValue:Ҫ���ҵĶ�Ӧ�ؼ��ֵ�ֵ
// ����ֵ��ָ���е�λ��
//////////////////////////////////////////////////////////////////////////
unsigned int CMysql::MysqlFindMethod(const std::string & key,const std::string & table,const std::string & keyValue)
{
	std::string sql = "select * from " + table;//������ѯһ��
	int totalRows = 0;
	if( this->MysqlQueryWrap(sql) != -1){
		totalRows  = this->MysqlQueryWrap(sql); //ͨ����ѯ�ҵ��ı����������Ŀ
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