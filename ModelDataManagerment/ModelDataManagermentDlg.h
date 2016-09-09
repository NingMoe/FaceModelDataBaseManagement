
// ModelDataManagermentDlg.h : 头文件
//

#pragma once
#include "CMysql.h"
#include "afxcmn.h"

// CModelDataManagermentDlg 对话框
class CModelDataManagermentDlg : public CDialogEx
{
// 构造
public:
	CModelDataManagermentDlg(CWnd* pParent = NULL);	// 标准构造函数

	~CModelDataManagermentDlg();
// 对话框数据
	enum { IDD = IDD_MODELDATAMANAGERMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
