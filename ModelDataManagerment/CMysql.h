/************************************************************************/
/* ���ݿ������                                                         */
/************************************************************************/

/************************************************************************/
/* author:��ʤ
 * data:2015-01-18*/
/************************************************************************/
#ifndef _CMYSQL_H_
#define _CMYSQL_H_
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include "winsock2.h"
#include "mysql.h"
#include "CErrorLog.h"


class CMysql{
public:
	//���캯��
	CMysql(const std::string &host,const std::string & username,
		 const std::string &passwd,const std::string &database,
		 unsigned int port);
	//��������
	~CMysql();
	//��������
	//query:ִ�����
	//����ֵ���ɹ�����0��ʧ�ܷ���-1
	int MysqlInsertWrap(const std::string &query);

	//ɾ������
	//query:ִ�����
	//����ֵ���ɹ�����0��ʧ�ܷ���-1
	int MysqlDeleteWrap(const std::string &query);

	//��������
	//query:ִ�����
	//����ֵ���ɹ�����0��ʧ�ܷ���-1
	int MysqlUpdateWrap(const std::string &query);

	//��ѯ����
	//query:ִ�����
	//����ֵ���ɹ�����0��ʧ�ܷ���-1
	int MysqlQueryWrap(const std::string &query);

	//�ͷ��Ѳ�ѯ������
	void MysqlFreeWrap();

	// ���ݿ����Ӻ���
	// host:��������IP��ַ
	// username:�û���
	// passwd:����
	// database:���ݿ���
	// port:�˿ں�
	int MysqlConnectWrap(const std::string& host, const std::string& username,
		const std::string& passwd, const std::string& database,
		unsigned int port);

	//�м��࣬����֧���±����
	class Proxy{
	public:
		//���캯��
		Proxy(CMysql *base): base_(base){}
		//ͨ���±��ҵ�ָ����
		//idx:���±�
		//����ֵ��ָ���е�ֵ
		std::string operator[] (unsigned int idx)const;

		//ͨ���ؼ����ҵ�ָ����
		//key:�����ƹؼ���
		//����ֵ��ָ���е�ֵ
		std::string operator[] (const std::string &key) const;
	private:
		//���ڷ���CMysql��Ķ���
		CMysql *base_;
	};

	//ͨ���±��ҵ�ָ����
	//idx:���±�
	const Proxy operator[] (unsigned int idx);

	// �������ݶ��������
	unsigned int MysqlNumRowsWrap();

	// �������ݶ��������
	unsigned int MysqlFieldCountWrap();

	//ͨ���ؼ���,��ĳ�ű�����Ҷ�Ӧ�ؼ����������±�
	unsigned int MysqlFindMethod(const std::string & key,const std::string & table, const std::string & keyValue);
//protected:
//	// ���ݿ����Ӻ���
//	// host:��������IP��ַ
//	// username:�û���
//	// passwd:����
//	// database:���ݿ���
//	// port:�˿ں�
//	int MysqlConnectWrap(const std::string& host, const std::string& username,
//		const std::string& passwd, const std::string& database,
//		unsigned int port);

private:
	// ���ݿ����Ӷ���
	MYSQL* conn_;
	// ���ݶ���
	MYSQL_RES* resptr_;
	// �����ж���
	MYSQL_ROW sqlrow_;
	// ����������
	std::vector<std::string> colname_;
};
#endif;