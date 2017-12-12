
// TeamClientDlg.cpp : ʵ���ļ�
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


// CTeamClientDlg �Ի���


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


// CTeamClientDlg ��Ϣ�������

BOOL CTeamClientDlg::OnInitDialog()
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTeamClientDlg::OnPaint()
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
HCURSOR CTeamClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTeamClientDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
// 	sendto(sockClient, "�����ַ�����", strlen("�����ַ�����"), 0,
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
	//CReadTransactionInfo::GetInstance().Initialize(_T("C:\\Program Files (x86)\\p-an.com\\ƽ�����������Ŷӹ���ϵͳ\\Data\\Runtime\\QTransactionsFinished.Xml"));
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
	//CReadTransactionInfo::GetInstance().Initialize(_T("C:\\Program Files (x86)\\p-an.com\\ƽ�����������Ŷӹ���ϵͳ\\Data\\Runtime\\QTransactionsRunning.Xml"));
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		SendDataToServer();
	}
	CDialogEx::OnTimer(nIDEvent);
}
