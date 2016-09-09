
// ModelDataManagermentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ModelDataManagerment.h"
#include "ModelDataManagermentDlg.h"
#include "afxdialogex.h"
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CModelDataManagermentDlg �Ի���



CModelDataManagermentDlg::CModelDataManagermentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModelDataManagermentDlg::IDD, pParent)
	, m_editId(_T(""))
	, m_editName(_T(""))
	, m_editImagePath(_T("")),m_sql(new CMysql("127.0.0.1","root","zxc163wyp","modeltrainlibrary_schema",3306))
	, m_editModelVec(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

CModelDataManagermentDlg::~CModelDataManagermentDlg()
{
	delete m_sql;
}

void CModelDataManagermentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ID_EDIT, m_editId);
	DDX_Text(pDX, IDC_NAME_EDIT, m_editName);
	DDX_Text(pDX, IDC_IMAGEPATH_EDIT, m_editImagePath);
	DDX_Text(pDX, IDC_MODELVEC_EDIT, m_editModelVec);
	DDX_Control(pDX, IDC_QUERY_LIST, m_queryList);
	
}

BEGIN_MESSAGE_MAP(CModelDataManagermentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSERT_BUTTON, &CModelDataManagermentDlg::OnBnClickedInsertButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CModelDataManagermentDlg::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_QUERY_BUTTON, &CModelDataManagermentDlg::OnBnClickedQueryButton)
	ON_BN_CLICKED(IDC_UPDATE_BUTTON, &CModelDataManagermentDlg::OnBnClickedUpdateButton)
	ON_BN_CLICKED(IDC_SHOWIMAGE_BUTTON, &CModelDataManagermentDlg::OnBnClickedShowimageButton)
END_MESSAGE_MAP()


// CModelDataManagermentDlg ��Ϣ�������

BOOL CModelDataManagermentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	//������б���������һЩ��ʼ��������
	CRect rect;
	m_queryList.GetClientRect(&rect);
	m_queryList.SetExtendedStyle(m_queryList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	// Ϊ�б���ͼ�ؼ��������   
	m_queryList.InsertColumn(0, _T("���֤id��"), LVCFMT_CENTER, rect.Width()*3/8, 0);   
	m_queryList.InsertColumn(1, _T("����"), LVCFMT_CENTER, rect.Width()*1/8, 1);   
	m_queryList.InsertColumn(2, _T("����ͼƬ��"), LVCFMT_CENTER, rect.Width()*2/8, 2);
	m_queryList.InsertColumn(3, _T("ģ�������ļ���"), LVCFMT_CENTER, rect.Width()*3/8, 3);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CModelDataManagermentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CModelDataManagermentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CModelDataManagermentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//////////////////////////////////////////////////////////////////////////
// ���ݿ�������ݺ���(����߱�������������)
// ����ֵ����
//////////////////////////////////////////////////////////////////////////
void CModelDataManagermentDlg::OnBnClickedInsertButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	std::string insertSql = std::string("insert into model_table values(");
    //CStringתΪstd::string
	//id
	CStringA strId(m_editId.GetBuffer(0)); 
	m_editId.ReleaseBuffer(); 
	std::string idStr (strId.GetBuffer(0)); 
	strId.ReleaseBuffer();
	if (idStr.empty())
	{
		MessageBox(_T("δ����id��"),_T("��ʾ"),MB_OK);
		return;
	}
	std::string idFormat = "(\\d{17})([xX0-9])";
	std::regex r(idFormat);
	if (!std::regex_match(idStr,r))
	{
		MessageBox(_T("�������֤id��ʽ����ȷ"),_T("��ʾ"),MB_OK);
		return;
	}
	//name
	CStringA strName(m_editName.GetBuffer(0)); 
	m_editName.ReleaseBuffer(); 
	std::string nameStr (strName.GetBuffer(0)); 
	strName.ReleaseBuffer();
	//imagePath
	CStringA strImagePath(m_editImagePath.GetBuffer(0)); 
	m_editImagePath.ReleaseBuffer(); 
	std::string imagePathStr (strImagePath.GetBuffer(0)); 
	imagePathStr = "F:\\\\FaceVertificationServer\\\\modelVec\\\\" + imagePathStr + ".jpg";
	strImagePath.ReleaseBuffer();
	//modelVec
	CStringA strModelVecPath(m_editModelVec.GetBuffer(0)); 
	m_editModelVec.ReleaseBuffer(); 
	std::string modelVecPath (strModelVecPath.GetBuffer(0)); 
	modelVecPath = "F:\\\\FaceVertificationServer\\\\modelVec\\\\" + modelVecPath + ".txt";
	strImagePath.ReleaseBuffer();
    //
	if (idStr.empty() || nameStr.empty() || imagePathStr.empty() || modelVecPath.empty())
	{
		MessageBox(_T("ȱ������"),_T("��ʾ"),MB_OK);
		return;
	}

	insertSql = insertSql + std::string("\"")+ idStr + std::string("\",") + std::string("\"") 
		        +nameStr + std::string("\",") + std::string("\"") + imagePathStr + std::string("\",") 
				+ std::string("\"") + modelVecPath + std::string("\")");  
		         
    if (m_sql->MysqlInsertWrap(insertSql) != -1)
    { 
		MessageBox(_T("����ɹ�"),_T("��ʾ"),MB_OK);
    }
	else
		MessageBox(_T("����ʧ��"),_T("��ʾ"),MB_OK);
	

	UpdateData(FALSE);

	OnBnClickedQueryButton();
}

//////////////////////////////////////////////////////////////////////////
// ���ݿ�ɾ�����ݺ���(ͨ�����֤id��ɾ��)
// ����ֵ����
//////////////////////////////////////////////////////////////////////////
void CModelDataManagermentDlg::OnBnClickedDeleteButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	std::string deleteSql = std::string("delete from model_table where id=");
	CStringA strId(m_editId.GetBuffer(0)); 
	m_editId.ReleaseBuffer(); 
	std::string idStr (strId.GetBuffer(0)); 
	strId.ReleaseBuffer();
	//
	if (idStr.empty())
	{
		MessageBox(_T("δ����id��"),_T("��ʾ"),MB_OK);
		return;
	}
	std::string idFormat = "(\\d{17})([xX0-9])";
	std::regex r(idFormat);
	if (!std::regex_match(idStr,r))
	{
		MessageBox(_T("�������֤id��ʽ����ȷ"),_T("��ʾ"),MB_OK);
		return;
	}

	deleteSql = deleteSql + std::string("\"")+ idStr + std::string("\"");
		         

	if (m_sql->MysqlInsertWrap(deleteSql) != -1)
	{ 
		MessageBox(_T("ɾ���ɹ�"),_T("��ʾ"),MB_OK);
	}
	else
		MessageBox(_T("ɾ��ʧ��"),_T("��ʾ"),MB_OK);


	 UpdateData(FALSE);


	 OnBnClickedQueryButton();
}

//////////////////////////////////////////////////////////////////////////
// ���ݿ��ѯ���ݺ���(ͨ�����֤id�Ų�ѯ)
// ����ֵ����
//////////////////////////////////////////////////////////////////////////
void CModelDataManagermentDlg::OnBnClickedQueryButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ���б���ͼ�ؼ��в����б���������б������ı�   
	UpdateData(TRUE);
	if (m_queryList.GetItemCount() != 0)
	{
		m_queryList.DeleteAllItems();
	}
	std::string sql = "select * from model_table";
	if(m_sql->MysqlQueryWrap(sql) != -1){
		int dataRows = m_sql->MysqlNumRowsWrap();
		for (int i = 0;i < dataRows;++i)
		{   
			CString idStr( ((*m_sql)[i][0]).c_str());
			m_queryList.InsertItem(i,idStr);
			
			CString nameStr( ((*m_sql)[i][1]).c_str());
			m_queryList.SetItemText(i, 1, nameStr);
			
			int imagePathStartFlag = ((*m_sql)[i][2]).rfind('\\');
			int imagePathEndFlag =  ((*m_sql)[i][2]).size();
			std::string strImagePath = ((*m_sql)[i][2]).substr(imagePathStartFlag+1,imagePathEndFlag-imagePathStartFlag-1);
			CString imagePathStr(strImagePath.c_str());
			m_queryList.SetItemText(i, 2, imagePathStr);

			int modelVecStartFlag = ((*m_sql)[i][3]).rfind('\\');
			int modelVecEndFlag =  ((*m_sql)[i][3]).size();
			std::string strModelVecPath = ((*m_sql)[i][3]).substr(modelVecStartFlag+1,modelVecEndFlag-modelVecStartFlag-1);
			CString modelVecPathStr(strModelVecPath.c_str());
			m_queryList.SetItemText(i, 3, modelVecPathStr);
		}
	}
	else{
		MessageBox(_T("��ѯʧ��"),_T("��ʾ"),MB_OK);
	}
	UpdateData(FALSE);

}

//////////////////////////////////////////////////////////////////////////
// ���ݿ��޸����ݺ���
// ����ֵ����
//////////////////////////////////////////////////////////////////////////
void CModelDataManagermentDlg::OnBnClickedUpdateButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	std::string updateSql = std::string("update model_table set ");
	//id
	CStringA strId(m_editId.GetBuffer(0)); 
	m_editId.ReleaseBuffer(); 
	std::string idStr (strId.GetBuffer(0)); 
	strId.ReleaseBuffer();
	if (idStr.empty())
	{
		MessageBox(_T("δ����id��"),_T("��ʾ"),MB_OK);
		return;
	}
	std::string idFormat = "(\\d{17})([xX0-9])";
	std::regex r(idFormat);
	if (!std::regex_match(idStr,r))
	{
		MessageBox(_T("�������֤id��ʽ����ȷ"),_T("��ʾ"),MB_OK);
		return;
	}
	//name
	CStringA strName(m_editName.GetBuffer(0)); 
	m_editName.ReleaseBuffer(); 
	std::string nameStr (strName.GetBuffer(0)); 
	strName.ReleaseBuffer();
	//imagePath
	CStringA strImagePath(m_editImagePath.GetBuffer(0)); 
	m_editImagePath.ReleaseBuffer(); 
	std::string imagePathStr (strImagePath.GetBuffer(0)); 
	imagePathStr = "F:\\\\FaceVertificationServer\\\\modelFaces\\\\" + imagePathStr + ".jpg";
	strImagePath.ReleaseBuffer();
	//modelVecPath 
	CStringA strModelVecPath(m_editModelVec.GetBuffer(0)); 
	m_editModelVec.ReleaseBuffer(); 
	std::string modelVecPathStr (strModelVecPath.GetBuffer(0)); 
	modelVecPathStr = "F:\\\\FaceVertificationServer\\\\modelVec\\\\" + modelVecPathStr + ".txt";
	strModelVecPath.ReleaseBuffer();

	//
	if (idStr.empty() || nameStr.empty() || imagePathStr.empty()|| modelVecPathStr.empty())
	{
		MessageBox(_T("ȱ������"),_T("��ʾ"),MB_OK);
		return;
	}

	updateSql = updateSql + std::string("name=\"")+ nameStr + std::string("\",") 
		        + std::string("faceImagePath=") + std::string("\"") + imagePathStr +  std::string("\",")
				+ std::string("modelVec=\"") + modelVecPathStr + std::string("\"") + std::string(" where id=") 
				+ std::string("\"") + idStr + std::string("\"");

		        
	if (m_sql->MysqlUpdateWrap(updateSql) != -1)
	{ 
		MessageBox(_T("���ĳɹ�"),_T("��ʾ"),MB_OK);
	}
	else
		MessageBox(_T("����ʧ��"),_T("��ʾ"),MB_OK);


	UpdateData(FALSE);

	OnBnClickedQueryButton();
}

//////////////////////////////////////////////////////////////////////////
// ���ݿ���ʾ����ͼƬ����
// ����ֵ����
//////////////////////////////////////////////////////////////////////////
void CModelDataManagermentDlg::OnBnClickedShowimageButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//��id��ʾ,CSTRINGת��Ϊstd::string
	CStringA strId(m_editId.GetBuffer(0)); 
	m_editId.ReleaseBuffer(); 
	std::string idStr (strId.GetBuffer(0)); 
	strId.ReleaseBuffer();
	//
	if (idStr.empty())
	{
		 MessageBox(_T("δ����id��"),_T("��ʾ"),MB_OK);
		 return;
	}
	std::string idFormat = "(\\d{17})([xX0-9])";
	std::regex r(idFormat);
	if (!std::regex_match(idStr,r))
	{
		MessageBox(_T("�������֤id��ʽ����ȷ"),_T("��ʾ"),MB_OK);
		return;
	}
	int index = m_sql->MysqlFindMethod("id","model_table",idStr);//��Ӧ�����±�
    if (index == m_sql->MysqlQueryWrap("select * from model_table"))
	{
		MessageBox(_T("�����ڴ�id��"),_T("��ʾ"),MB_OK);
		return;
	}

    m_editImagePath = CString((*m_sql)[index]["faceImagePath"].c_str());
	m_editId = CString((*m_sql)[index]["id"].c_str());
	m_editName = CString((*m_sql)[index]["name"].c_str());
	m_editModelVec = CString((*m_sql)[index]["modelVec"].c_str());
	CImage image;
	if(E_FAIL == image.Load(m_editImagePath))
	{
	   MessageBox(_T("ͼƬ·����Ч"),_T("��ʾ"),MB_OK);
	   return;
	}
	CRect rect;
	CRect rect1;
	int cx = image.GetWidth();
	int cy = image.GetHeight();

	GetDlgItem(IDC_PICTURE_STATIC)->GetClientRect(&rect);
	CDC * pDc = GetDlgItem(IDC_PICTURE_STATIC)->GetDC();
	SetStretchBltMode(pDc->m_hDC,STRETCH_HALFTONE);

	float xScale=(float)rect.Width()/(float)cx;
	float yScale=(float)rect.Height()/(float)cy;
	rect1 = CRect(rect.TopLeft(), CSize((int)cx*xScale,(int)cy*yScale));
    image.StretchBlt(pDc->m_hDC,rect1,SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������
	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc

	int startFlag = (*m_sql)[index]["faceImagePath"].rfind('\\');
	int endFlag = (*m_sql)[index]["faceImagePath"].rfind('.');
	std::string path = (*m_sql)[index]["faceImagePath"].substr(startFlag+1,endFlag-startFlag-1);
	m_editImagePath = CString(path.c_str());
    
	startFlag = (*m_sql)[index]["modelVec"].rfind('\\');
	endFlag = (*m_sql)[index]["modelVec"].rfind('.');
	path = (*m_sql)[index]["modelVec"].substr(startFlag+1,endFlag-startFlag-1);
	m_editModelVec = CString(path.c_str());
	UpdateData(FALSE);
}
