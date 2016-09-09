
// ModelDataManagermentDlg.h : ͷ�ļ�
//

#pragma once
#include "CMysql.h"
#include "afxcmn.h"

// CModelDataManagermentDlg �Ի���
class CModelDataManagermentDlg : public CDialogEx
{
// ����
public:
	CModelDataManagermentDlg(CWnd* pParent = NULL);	// ��׼���캯��

	~CModelDataManagermentDlg();
// �Ի�������
	enum { IDD = IDD_MODELDATAMANAGERMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	CString m_editId;
	CString m_editName;
	CString m_editImagePath;
	CString m_editModelVec;

private:
	CMysql * m_sql;
public:
	afx_msg void OnBnClickedInsertButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedQueryButton();
	CListCtrl m_queryList;
	afx_msg void OnBnClickedUpdateButton();
	afx_msg void OnBnClickedShowimageButton();
	
};
