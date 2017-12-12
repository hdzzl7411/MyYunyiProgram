
// TeamClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeamClient.h"
#include "TeamClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <WinSock2.h>
#include "ReadTransactionInfo.h"
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTeamClientDlg 对话框


std::string GetAppFullPath()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);

	return szPath;
}

std::string GetAppPath()
{
	std::string strFullPath = GetAppFullPath();

	return strFullPath.substr(0, strFullPath.rfind('\\') + 1);
}

CTeamClientDlg::CTeamClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeamClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTeamClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTeamClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTeamClientDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTeamClientDlg 消息处理程序

BOOL CTeamClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_SendData = new CSSendData;
	m_SendData->nEvaCount = m_SendData->nRunInfoCount = 0;
	memset(m_SendData->strEvaBuf, 0, MAX_EVABUF);
	memset(m_SendData->strRunInfoBuf, 0, MAX_RUNBUF);
	//m_strServerIP = "127.0.0.1";
	m_nPort = 7878;
	m_strPath = "";
	ReadConfig();
	InitSocket();
	SetTimer(1, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTeamClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTeamClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTeamClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTeamClientDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	//SendDataToServer();
}

void CTeamClientDlg::InitSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return;
	}
	m_sockClient = socket(AF_INET, SOCK_DGRAM, 0);

}

void CTeamClientDlg::SendDataToServer()
{
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_strServerIP.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_nPort);
	MakeSendData();
	sendto(m_sockClient, (char*)m_SendData, sizeof CSSendData + 1, 0,
		(SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	CReadTransactionInfo::GetInstance().ClearListInfo();
// 	sendto(sockClient, "测试字符长度", strlen("测试字符长度"), 0,
// 		(SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
}

void CTeamClientDlg::MakeSendData()
{
	string strPathFileName;
	if (m_strPath.empty())
	{
		strPathFileName = "..\\Data\\Runtime\\QTransactionsFinished.Xml";
	}
	else
	{
		strPathFileName = m_strPath + "\\Data\\Runtime\\QTransactionsFinished.Xml";
	}
	//CReadTransactionInfo::GetInstance().Initialize(_T("C:\\Program Files (x86)\\p-an.com\\平安力合智能排队管理系统\\Data\\Runtime\\QTransactionsFinished.Xml"));
	CReadTransactionInfo::GetInstance().Initialize(strPathFileName);
	CReadTransactionInfo::GetInstance().InitEvaluation();
	list<CSEvaluation> lEvaTmp = CReadTransactionInfo::GetInstance().GetEvaluationList();
	int offset = 0;
	m_SendData->nEvaCount = lEvaTmp.size();
	for (list<CSEvaluation>::iterator EvaIt = lEvaTmp.begin(); EvaIt != lEvaTmp.end(); EvaIt++)
	{
		CSEvaluation evaTemp = *EvaIt;
		memcpy(m_SendData->strEvaBuf + offset, (char *)&evaTemp, sizeof(CSEvaluation));
		offset += sizeof CSEvaluation;
	}
	if (m_strPath.empty())
	{
		strPathFileName = "..\\Data\\Runtime\\QTransactionsRunning.Xml";
	}
	else
	{
		strPathFileName = m_strPath + "\\Data\\Runtime\\QTransactionsRunning.Xml";
	}
	//CReadTransactionInfo::GetInstance().Initialize(_T("C:\\Program Files (x86)\\p-an.com\\平安力合智能排队管理系统\\Data\\Runtime\\QTransactionsRunning.Xml"));
	CReadTransactionInfo::GetInstance().Initialize(strPathFileName);
	CReadTransactionInfo::GetInstance().InitRuningInfo();
	list<CSRuningInfo> lRunTmp = CReadTransactionInfo::GetInstance().GetRunInfoList();
	offset = 0;
	m_SendData->nRunInfoCount = lRunTmp.size();
	for (list<CSRuningInfo>::iterator RunIt = lRunTmp.begin(); RunIt != lRunTmp.end(); RunIt++)
	{
		CSRuningInfo runTemp = *RunIt;
		memcpy(m_SendData->strRunInfoBuf + offset, (char *)&runTemp, sizeof(CSRuningInfo));
		offset += sizeof CSRuningInfo;
	}

}


void CTeamClientDlg::ReadConfig()
{
	CString  PathFileName;
	PathFileName.Format("%sClientConfig.ini", GetAppPath().c_str());
	char strBuf[100] = { 0 };
	::GetPrivateProfileString("Setting", "ServerIP", 0, strBuf, 100, PathFileName);
	m_strServerIP = strBuf;
	::GetPrivateProfileString("Setting", "Port", 0, strBuf, 100, PathFileName);
	m_nPort = atoi(strBuf);
	::GetPrivateProfileString("Setting", "AppPath", "", strBuf, 100, PathFileName);
	m_strPath = strBuf;
}

void CTeamClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		SendDataToServer();
	}
	CDialogEx::OnTimer(nIDEvent);
}
