
// ModifyProjectNameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ModifyProjectName.h"
#include "ModifyProjectNameDlg.h"
#include "afxdialogex.h"

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


// CModifyProjectNameDlg �Ի���



CModifyProjectNameDlg::CModifyProjectNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyProjectNameDlg::IDD, pParent)
	, m_strProDir(_T(""))
	, m_strProName(_T(""))
	, m_strNewProName(_T(""))
	, m_strItemName(_T(""))
	, m_strNewItemName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModifyProjectNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRO_DIR, m_strProDir);
	DDX_Text(pDX, IDC_PRO_NAME, m_strProName);
	DDX_Text(pDX, IDC_NEW_PRO_NAME, m_strNewProName);
	DDX_Text(pDX, IDC_ITEM_NAME, m_strItemName);
	DDX_Text(pDX, IDC_NEW_ITEM_NAME, m_strNewItemName);
}

BEGIN_MESSAGE_MAP(CModifyProjectNameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MODIFY, &CModifyProjectNameDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_BTN_SELECTDIR, &CModifyProjectNameDlg::OnBnClickedBtnSelectdir)
	ON_BN_CLICKED(IDC_BTN_SELECTPRONAME, &CModifyProjectNameDlg::OnBnClickedBtnSelectproname)
	ON_BN_CLICKED(IDC_BTN_SELECTITEMNAME, &CModifyProjectNameDlg::OnBnClickedBtnSelectitemname)
END_MESSAGE_MAP()


// CModifyProjectNameDlg ��Ϣ�������

BOOL CModifyProjectNameDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CModifyProjectNameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CModifyProjectNameDlg::OnPaint()
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
HCURSOR CModifyProjectNameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include <windows.h>
#include <io.h>
#include <iostream>
#include <string>
#include <fstream>


using namespace std;
/////////////////////////////////////
//ReNameFolder
//������lpszFromPath Դ�ļ���·�� ��lpszToPath Ŀ���ļ���·��
//���ã��޸�ԭ�ļ��е����֡�
//
/////////////////////////////////////
BOOL ReNameFolder(LPCTSTR lpszFromPath, LPCTSTR lpszToPath)
{
	int nLengthFrm = strlen(lpszFromPath);
	char *NewPathFrm = new char[nLengthFrm + 2];
	strcpy_s(NewPathFrm,nLengthFrm + 2,lpszFromPath);
	NewPathFrm[nLengthFrm] = '\0';
	NewPathFrm[nLengthFrm + 1] = '\0';

	SHFILEOPSTRUCT FileOp;
	ZeroMemory((void*)&FileOp, sizeof(SHFILEOPSTRUCT));

	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = NewPathFrm;
	FileOp.pTo = lpszToPath;
	FileOp.wFunc = FO_RENAME;

	return SHFileOperation(&FileOp) == 0;
}

string&   replace_all_distinct(string&   str, const  string&  old_value, const   string&   new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else 
		{
			break; 
		}
	}
	return   str;
}
bool ReplaceInProFile(const string& strSourceFile,const string& strTarFile,char* strReplace, char* strNew)
{
// 	string file_path = "G:\\1234.txt";//�ļ�·��
// 	string out_path = "G:\\12345.txt";//���·��
	string str;
	string::size_type pos = 0;
	ifstream instream;
	ofstream outstream;
	instream.open(strSourceFile);
	if (!instream)
	{
		cout << "error" << endl;
		return false;
	}
	outstream.open(strTarFile);
	while (getline(instream, str)){
		pos = str.find(strReplace);//�����ַ���string�е�һ�γ��ֵ�λ��
		if (pos == string::npos)//�ж��Ƿ���ڡ�hyu������ַ�
		{
			//cout << "Can not find this string !" << endl;
			outstream << str << endl;
		}
		else{
			replace_all_distinct(str, strReplace, strNew);
			//str.replace(pos, strlen(strReplace), strNew);
			outstream << str << endl;
		}

	}
	instream.close();
	outstream.close();
	DeleteFile(strSourceFile.c_str());
	return true;
}
bool ReplaceInItemFile(const string& strSourceFile, const string& strTarFile, char* strReplace, char* strNew)
{
	string str;
	string::size_type pos = 0;
	ifstream instream;
	ofstream outstream;
	instream.open(strSourceFile);
	if (!instream)
	{
		cout << "error" << endl;
		return false;
	}
	outstream.open(strTarFile);
	bool bReplace = false;
	while (getline(instream, str)){
		pos = str.find(strReplace);//�����ַ���string�е�һ�γ��ֵ�λ��
		if (pos == string::npos)
		{
			cout << "Can not find this string !" << endl;
			outstream << str << endl;
		}
		else{
			//replace_all_distinct(str, strReplace, strNew);
			if (!bReplace)
			{
				str.replace(pos, strlen(strReplace), strNew);
				bReplace = true;
			}
			outstream << str << endl;
		}

	}
	instream.close();
	outstream.close();
	DeleteFile(strSourceFile.c_str());
	return true;
}
void CModifyProjectNameDlg::OnBnClickedModify()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strNewProFileName, strNewItemFileName, strOldProFileName, strOldItemFileName,strItem,strNewItem;
	if (m_strItemName.IsEmpty() || m_strNewItemName.IsEmpty() || m_strNewProName.IsEmpty() || m_strProName.IsEmpty() || m_strProDir.IsEmpty())
	{
		MessageBox(_T("����������Ŀ������Ϊ�գ�"), _T("��ʾ"), MB_OK);
		return;
	}
	int nPos = m_strNewProName.ReverseFind('.');
	if (nPos == -1)
	{
		m_strNewProName += ".sln";
	}
	strOldProFileName = m_strProDir + "\\" + m_strProName;
	strNewProFileName = m_strProDir + "\\" + m_strNewProName;
	ReplaceInProFile(strOldProFileName.GetBuffer(), strNewProFileName.GetBuffer(), strItem.GetBuffer(), strNewItem.GetBuffer());

	nPos = m_strItemName.ReverseFind('.');
	if (nPos == -1)
	{
		strItem = m_strItemName;
		m_strItemName += ".vcxproj";
	}
	else
	{
		strItem = m_strItemName.Left(nPos);
	}
	
	nPos = m_strNewItemName.ReverseFind('.');
	if (nPos == -1)
	{
		strNewItem = m_strNewItemName;
		m_strNewItemName += ".vcxproj";
	}
	else
	{
		strNewItem = m_strNewItemName.Left(nPos);
	}
	


	strOldItemFileName = m_strProDir + "\\" + strItem + "\\" + m_strItemName;
	strNewItemFileName = m_strProDir + "\\" + strItem + "\\" + m_strNewItemName;
	ReplaceInItemFile(strOldItemFileName.GetBuffer(), strNewItemFileName.GetBuffer(), strItem.GetBuffer(), strNewItem.GetBuffer());


	strOldItemFileName = m_strProDir + "\\" + strItem;
	strNewItemFileName = m_strProDir + "\\" + strNewItem;
	CString strFilter = strOldItemFileName + "\\" + strItem + ".vcxproj.filters";
	CString strNewFilter = strOldItemFileName + "\\" + strNewItem + ".vcxproj.filters";
	if (!_access(strFilter, 0))
	{
		rename(strFilter, strNewFilter);
	}
	CString strUser = strOldItemFileName + "\\" + strItem + ".vcxproj.user";
	CString strNewUser = strOldItemFileName + "\\" + strNewItem + ".vcxproj.user";
	if (!_access(strUser, 0))
	{
		rename(strUser, strNewUser);
	}

	ReNameFolder(strOldItemFileName, strNewItemFileName);

	CString strSdf = m_strProDir + "\\" + strItem + ".sdf";
	DeleteFile(strSdf);
	CString strSuo = m_strProDir + "\\" + strItem + ".v12.suo";
	DeleteFile(strSuo);

	nPos = m_strProDir.ReverseFind('\\');
	int nExtPos = m_strNewProName.ReverseFind('.');
	CString strNewDir = m_strProDir.Left(nPos) + "\\" + m_strNewProName.Left(nExtPos);
	ReNameFolder(m_strProDir, strNewDir);
	MessageBox(_T("�޸ĳɹ�"), _T("��ʾ"), MB_OK);

}


void CModifyProjectNameDlg::OnBnClickedBtnSelectdir()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("ѡ��Ҫ�޸ĵĹ���Ŀ¼:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;// BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	m_strProDir.Format(_T("%s"), szBuffer);
	UpdateData(FALSE);
}


void CModifyProjectNameDlg::OnBnClickedBtnSelectproname()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("�����ļ�(*.sln)\0*.sln\0�����ļ�(*.*)\0*.*\0");//Ҫѡ����ļ���׺   
	ofn.lpstrInitialDir = m_strProDir;//Ĭ�ϵ��ļ�·��   
	ofn.lpstrFile = szBuffer;//����ļ��Ļ�����   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
	BOOL bSel = GetOpenFileName(&ofn);
	
	
	m_strProName.Format(_T("%s"), szBuffer);
	m_strProName = m_strProName.Mid(m_strProDir.GetLength()+1);

	UpdateData(FALSE);
}


void CModifyProjectNameDlg::OnBnClickedBtnSelectitemname()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("��Ŀ�ļ�(*.vcxproj)\0*.vcxproj\0�����ļ�(*.*)\0*.*\0");//Ҫѡ����ļ���׺   
	ofn.lpstrInitialDir = m_strProDir;//Ĭ�ϵ��ļ�·��   
	ofn.lpstrFile = szBuffer;//����ļ��Ļ�����   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
	BOOL bSel = GetOpenFileName(&ofn);

	m_strItemName.Format(_T("%s"), szBuffer);
	int nPos = m_strItemName.ReverseFind('\\');
	m_strItemName = m_strItemName.Mid(nPos+1);
	UpdateData(FALSE);
}
