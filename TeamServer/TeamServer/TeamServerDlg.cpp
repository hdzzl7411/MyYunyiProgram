
// TeamServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeamServer.h"
#include "TeamServerDlg.h"
//#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <winsock2.h>

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
extern BOOL	g_bSizeFirst;
extern BOOL g_bMoveFirst;
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


// CTeamServerDlg 对话框


std::string GetAppFullPath ( )
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName ( NULL, szPath, MAX_PATH );

	return szPath;
}

std::string GetAppPath ( )
{
	std::string strFullPath = GetAppFullPath ( );

	return strFullPath.substr ( 0, strFullPath.rfind ( '\\' ) + 1 );
}


CTeamServerDlg::CTeamServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeamServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTeamServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTeamServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CTeamServerDlg 消息处理程序

BOOL CTeamServerDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_brush.CreateSolidBrush ( RGB ( 0, 0, 0 ) );   //   生成一黑色刷子
	my_font = new CFont ( );
	m_strIP = "127.0.0.1";
	m_nPort = 7878;
	//GetClientRect ( &m_rtMainPos );
	ReadConfig ( );
	//ScreenToClient(m_rtMainPos);
	mySetFont ( m_nFontSize, my_font, (char *)m_strFontText.c_str ( ) );
	//this->MoveWindow(&m_rtMainPos);
// 	int   cx   =   GetSystemMetrics(   SM_CXSCREEN   );  //获得屏幕的分辨率
// 	int   cy   =   GetSystemMetrics(   SM_CYSCREEN   );
	g_bMoveFirst = TRUE;
	g_bSizeFirst = TRUE;
	this->SetWindowPos(&wndTopMost,m_rtMainPos.left,m_rtMainPos.top,m_rtMainPos.Width(),m_rtMainPos.Height(),SWP_SHOWWINDOW);
	//SetWindowPos(this,0,0,400,600,SWP_SHOWWINDOW);
	for ( int i = 0; i < 10; i++ )
	{
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetWindowText ( _T("此文字为评价窗口测试用的文字") );
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetWindowText ( _T("") );
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetFont(my_font);
		if ( i < 8 )
		{
			GetDlgItem ( IDC_CALLINFO1 + i )->SetWindowText ( _T ( "此文字为叫号信息测试用的文字" ) );
			GetDlgItem ( IDC_CALLINFO1 + i )->SetWindowText ( _T ( "" ) );
			GetDlgItem ( IDC_CALLINFO1 + i )->SetFont(my_font);
		}
	}



	m_lpRecvData = new CSSendData();
	InitServerSocket();
	SetTimer(1,1000,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTeamServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTeamServerDlg::OnPaint()
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
HCURSOR CTeamServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTeamServerDlg::OnDestroy()
{
	if ( my_font != NULL ) 
	{
		SAFE_DELETE(my_font);
	}
	if(m_lpRecvData != NULL)
	{
		m_lpRecvData = NULL;
		//SAFE_DELETE(m_lpRecvData);
	}
	closesocket(m_sockSrv);  
	WSACleanup(); 
	CDialogEx::OnDestroy();

}

HBRUSH CTeamServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if ( nCtlColor == CTLCOLOR_DLG )      //对话框颜色  
		return   m_brush;           
	if ( nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION1 || pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION2 \
		|| pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION3 || pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION4 \
		|| pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION5 || pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION6 \
		|| pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION7 || pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION8\
		|| pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION9 || pWnd->GetDlgCtrlID ( ) == IDC_TRANSACTION10 \
		|| pWnd->GetDlgCtrlID ( ) == IDC_CALLINFO1 || pWnd->GetDlgCtrlID ( ) == IDC_CALLINFO2 \
		|| pWnd->GetDlgCtrlID ( ) == IDC_CALLINFO3 || pWnd->GetDlgCtrlID ( ) == IDC_CALLINFO4 \
		|| pWnd->GetDlgCtrlID ( ) == IDC_CALLINFO5 || pWnd->GetDlgCtrlID ( ) == IDC_CALLINFO6 \
		|| pWnd->GetDlgCtrlID ( ) == IDC_CALLINFO7 || pWnd->GetDlgCtrlID ( ) == IDC_CALLINFO8 
		)  //静态文本颜色，并且指定控件ID号  
	{
		pDC->SetTextColor ( RGB ( 255, 0, 0 ) );
		pDC->SetBkColor ( RGB ( 0, 0, 0 ) );     //静态控件的背景颜色设置，这里设置于背景颜色一致，则可以免设置透明
		pDC->SetBkMode(TRANSPARENT);    //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合  
		hbr = ( HBRUSH ) m_brush;
	}
	return hbr;
}

void CTeamServerDlg::mySetFont(int height, CFont *font, char *name)
{
	LOGFONT lf;
	::ZeroMemory ( &lf, sizeof ( lf ) );
	lf.lfHeight = 20;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 760;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = GB2312_CHARSET;
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	lf.lfClipPrecision = CLIP_STROKE_PRECIS;
	lf.lfQuality = DRAFT_QUALITY;
	lf.lfPitchAndFamily = VARIABLE_PITCH;
	lstrcpy ( lf.lfFaceName, "宋体" );

	lf.lfHeight = height;
	lstrcpy ( lf.lfFaceName, name);
	if ( font != NULL )
	{
		font->DeleteObject ( );
		font->CreateFontIndirect ( &lf );
	}
}


void CTeamServerDlg::ReadConfig()
{
	CString  PathFileName;
	PathFileName.Format ( "%sShowConfig.ini", GetAppPath ( ).c_str());
	char strBuf[100] = { 0 };
	::GetPrivateProfileString ( "Setting", "ServerIP", 0, strBuf, 100, PathFileName );
	m_strIP = strBuf;
	::GetPrivateProfileString ( "Setting", "Port", 0, strBuf, 100, PathFileName );
	m_nPort = atoi(strBuf);
	::GetPrivateProfileString ( "Setting", "Font", 0, strBuf, 100, PathFileName );
	m_strFontText = strBuf;
	::GetPrivateProfileString ( "Setting", "FontSize", 0, strBuf, 100, PathFileName );
	m_nFontSize = atoi ( strBuf );

	::GetPrivateProfileString ( "WinPos", "Left", 0, strBuf, 100, PathFileName );
	m_rtMainPos.left = atoi ( strBuf );
	::GetPrivateProfileString ( "WinPos", "Top", 0, strBuf, 100, PathFileName );
	m_rtMainPos.top = atoi ( strBuf );
	::GetPrivateProfileString ( "WinPos", "Right", 0, strBuf, 100, PathFileName );
	m_rtMainPos.right = atoi ( strBuf );
	::GetPrivateProfileString ( "WinPos", "Bottom", 0, strBuf, 100, PathFileName );
	m_rtMainPos.bottom = atoi ( strBuf );
	for ( int i = 0; i < MAXCOUNTER_NUM; i++ )
	{
		char strName[100] = { 0 };
		sprintf ( strName, "Win%d", i + 1 );
		::GetPrivateProfileString ( "Transaction", strName, "", strBuf, 100, PathFileName );
		m_strWinText[i] = strBuf ;
	}
}
void CTeamServerDlg::WriteConfig()
{
	CString  PathFileName;
	PathFileName.Format ( "%sShowConfig.ini", GetAppPath ( ).c_str());
	CString strValue;
	strValue.Format("%d",m_rtMainPos.left);
	::WritePrivateProfileString("WinPos", "Left",strValue.GetBuffer(),PathFileName);
	strValue.Format("%d",m_rtMainPos.top);
	::WritePrivateProfileString("WinPos", "Top",strValue.GetBuffer(),PathFileName);
	strValue.Format("%d",m_rtMainPos.right);
	::WritePrivateProfileString("WinPos", "Right",strValue.GetBuffer(),PathFileName);
	strValue.Format("%d",m_rtMainPos.bottom);
	::WritePrivateProfileString("WinPos", "Bottom",strValue.GetBuffer(),PathFileName);

}

void CTeamServerDlg::UpdateEvaluation(LPSendData lpRecvData)
{
	ReadConfig ( );
	mySetFont ( m_nFontSize, my_font, (char *)m_strFontText.c_str() );
	
	int offset = 0;
	for(int i = 0; i <lpRecvData->nEvaCount ; i++)
	{
		LPCSEvaluation stEva = new CSEvaluation;;
		memcpy(stEva,lpRecvData->strEvaBuf + offset,sizeof CSEvaluation);
		offset += sizeof CSEvaluation;
		CString  strShowText;
		strShowText.Format ( "%d号窗口  %s%d个  不允许%d", atoi(stEva->nCounterIndex), m_strWinText[atoi(stEva->nCounterIndex) - 1].c_str(), stEva->nCount, stEva->nDiscontentCount );
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetFont ( my_font );
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetWindowText ( strShowText );
		SAFE_DELETE(stEva);
	}

}

void CTeamServerDlg::UpdateRunInfo(LPSendData lpRecvData)
{
	int offset = 0;
	for (int i = 0; i <lpRecvData->nRunInfoCount; i++)
	{
		LPCSRuningInfo stRunInfo = new CSRuningInfo;
		memcpy(stRunInfo,lpRecvData->strRunInfoBuf + offset,sizeof CSRuningInfo);
		offset += sizeof CSRuningInfo;
		CString  strShowText;
		strShowText.Format ( "请%s号到%02d号窗口", stRunInfo->nTicketNumber, atoi(stRunInfo->nCounterIndex) );
		GetDlgItem ( IDC_CALLINFO1 + i )->SetWindowText ( strShowText );
		GetDlgItem ( IDC_CALLINFO1 + i )->SetFont ( my_font );
		SAFE_DELETE(stRunInfo);
	}
}



void CTeamServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(nType == 1) return;   //如果是窗体最小化则什么都不做
	
	CWnd *pWnd = NULL;
	int iIncrementX = cx - m_rtMainPos.Width();
	int iIncrementY = cy - m_rtMainPos.Height();
	pWnd = GetDlgItem(IDC_TRANSACTION1);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	int ctHeight = rect.Height() + iIncrementY / 14;
	rect.right += iIncrementX;
	//int ctHeight = rect.Height() * (float)((float)1 + (float)iIncrementY / (float)16);
	if(ctHeight < 10) 
		ctHeight = 10;
// 	else if( ctHeight > 50)
// 		ctHeight = 50;
	rect.bottom = rect.top + ctHeight;
	pWnd->MoveWindow(rect);
	for(int i = 1; i < 10; i++)
	{
		rect.top = rect.bottom + 2;
		rect.bottom = rect.top + ctHeight;
		pWnd = GetDlgItem(IDC_TRANSACTION1 + i);
		pWnd->MoveWindow(rect);
	}
	int bottTemp = rect.bottom; 
	CRect runRect;
	runRect.left = rect.left - 2;
	runRect.right = runRect.left + rect.Width() / 2;
	runRect.top = bottTemp + 30;
	runRect.bottom = runRect.top + ctHeight;
	bottTemp = runRect.bottom;
	pWnd = GetDlgItem(IDC_CALLINFO1);
	pWnd->MoveWindow(runRect);
	CRect runRect1;
	runRect1.left = runRect.right + 10;
	runRect1.top = runRect.top;
	runRect1.right =  runRect1.left +  runRect.Width();
	runRect1.bottom = runRect1.top + ctHeight;
	pWnd = GetDlgItem(IDC_CALLINFO2);
	pWnd->MoveWindow(runRect1);
	for(int i = 1;i<4;i++)
	{
		runRect.top = runRect.bottom + 2;
		runRect.bottom = runRect.top + ctHeight;
		pWnd = GetDlgItem(IDC_CALLINFO1 + 2 * i);
		pWnd->MoveWindow(runRect);
		runRect1.top = runRect1.bottom + 2;
		runRect1.bottom = runRect1.top + ctHeight;
		pWnd = GetDlgItem(IDC_CALLINFO2 + 2 * i);
		pWnd->MoveWindow(runRect1);
	}

	GetWindowRect(&m_rtMainPos);//将变化后的对话框大小设为旧大小
	if(!g_bSizeFirst)
	{
		WriteConfig();
	}
	else
	{
		g_bSizeFirst = FALSE;
	}
	Invalidate();
}


LRESULT CTeamServerDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint ptCur;
	CRect rect;
	GetCursorPos ( &ptCur );
	GetWindowRect ( &rect );
	//模拟窗口标题栏、左上角、右上角、左下角、右下角、左边、右边、上边、下边等区域
	if ( CRect ( rect.left, rect.top, rect.right, rect.top + TITLEHEIGHT ).PtInRect ( ptCur ) )
		return HTCAPTION;
	else if ( CRect ( rect.left, rect.top, rect.left + PADDING, rect.top + PADDING ).PtInRect ( ptCur ) )
		return HTTOPLEFT;
	else if ( CRect ( rect.right - PADDING, rect.top, rect.right, rect.top + PADDING ).PtInRect ( ptCur ) )
		return HTTOPRIGHT;
	else if ( CRect ( rect.left, rect.bottom - PADDING, rect.left + PADDING, rect.bottom ).PtInRect ( ptCur ) )
		return HTBOTTOMLEFT;
	else if ( CRect ( rect.right - PADDING, rect.bottom - PADDING, rect.right, rect.bottom ).PtInRect ( ptCur ) )
		return HTBOTTOMRIGHT;
	else if ( CRect ( rect.left, rect.top, rect.left + PADDING, rect.bottom ).PtInRect ( ptCur ) )
		return HTLEFT;
	else if ( CRect ( rect.right - PADDING, rect.top, rect.right, rect.bottom ).PtInRect ( ptCur ) )
		return HTRIGHT;
	else if ( CRect ( rect.left, rect.top, rect.right - WINMINIVALUE, rect.top + PADDING ).PtInRect ( ptCur ) ) 
		return HTTOP;
	else if ( CRect ( rect.left, rect.bottom - PADDING, rect.right, rect.bottom ).PtInRect ( ptCur ) )
		return HTBOTTOM;
	return CDialogEx::OnNcHitTest(point);
}


void CTeamServerDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch ( nHitTest )
	{
	case HTCAPTION:
		SendMessage ( WM_SYSCOMMAND, SC_DRAGMOVE, MAKELPARAM ( point.x, point.y ) );
		return;
	case HTTOP:
		SendMessage ( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM ( point.x, point.y ) );
		return;
	case HTBOTTOM:
		SendMessage ( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM ( point.x, point.y ) );
		return;
	case HTLEFT:
		SendMessage ( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM ( point.x, point.y ) );
		return;
	case HTRIGHT:
		SendMessage ( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM ( point.x, point.y ) );
		return;
	case HTTOPLEFT:
		SendMessage ( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM ( point.x, point.y ) );
		return;
	case HTTOPRIGHT:
		SendMessage ( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM ( point.x, point.y ) );
		return;
	case HTBOTTOMLEFT:
		SendMessage ( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM ( point.x, point.y ) );
		return;
	case HTBOTTOMRIGHT:
		SendMessage ( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM ( point.x, point.y ) );
		return;
	default:
		break;
	}
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


BOOL CTeamServerDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint ptCur;
	CRect rect;
	GetCursorPos ( &ptCur );
	GetWindowRect ( &rect );
	//超过系统最大屏幕
	if ( rect.Width ( ) >= GetSystemMetrics ( SM_CXSCREEN ) - PADDING && rect.Height ( ) >= GetSystemMetrics ( SM_CYSCREEN ) - PADDING )
		return CDialog::OnSetCursor ( pWnd, nHitTest, message );
	//左上角和右下角位置
	if ( CRect ( rect.left, rect.top, rect.left + PADDING, rect.top + PADDING ).PtInRect ( ptCur ) || CRect ( rect.right - PADDING, rect.bottom - PADDING, rect.right, rect.bottom ).PtInRect ( ptCur ) )
	{
		SetCursor ( LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_SIZENWSE ) ) );
		return 0;
	}
	//左下角和右上角的位置
	else if ( CRect ( rect.left, rect.bottom - PADDING, rect.left + PADDING, rect.bottom ).PtInRect ( ptCur ) || CRect ( rect.right - PADDING, rect.top , rect.right+ PADDING, rect.top + PADDING ).PtInRect ( ptCur ) )
	{
		SetCursor ( LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_SIZENESW ) ) );
		return 0;
	}
	//上边和下边的位置
	else if ( CRect ( rect.left, rect.top, rect.right - WINMINIVALUE, rect.top + PADDING ).PtInRect ( ptCur ) || CRect ( rect.left, rect.bottom - PADDING, rect.right, rect.bottom ).PtInRect ( ptCur ) )
	{
		SetCursor ( LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_SIZENS ) ) );
		return 0;
	}
	//左边和右边的位置
	else if ( CRect ( rect.left, rect.top, rect.left + PADDING, rect.bottom ).PtInRect ( ptCur ) || CRect ( rect.right - PADDING, rect.top, rect.right, rect.bottom ).PtInRect ( ptCur ) )
	{
		SetCursor ( LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_SIZEWE ) ) );
		return 0;
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CTeamServerDlg::InitServerSocket()
{
	WORD wVersionRequested;   
	WSADATA wsaData;   
	int err;   
	wVersionRequested = MAKEWORD( 1, 1 );   
	err = WSAStartup( wVersionRequested, &wsaData );   
	if ( err != 0 ) {   
		return;   
	}   

	if ( LOBYTE( wsaData.wVersion ) != 1 ||   
		HIBYTE( wsaData.wVersion ) != 1 ) {   
			WSACleanup( );   
			return;    
	}   

	m_sockSrv=socket(AF_INET,SOCK_DGRAM,0);   
	SOCKADDR_IN addrSrv;   
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);   
	addrSrv.sin_family=AF_INET;   
	addrSrv.sin_port=htons(m_nPort);   

	bind(m_sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));   
}

void CTeamServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 1)
	{
		//SOCKADDR_IN addrClient;   
		int len = sizeof(SOCKADDR);   
		char recvBuf[sizeof CSSendData] = { 0 };   
		struct timeval tv;  
		int ret;  
		tv.tv_sec = 2;  
		tv.tv_usec = 0;  
		if (setsockopt(m_sockSrv, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0) {  
		    TRACE("socket option  SO_RCVTIMEO not support\n");  
		   return;  
		}  
		if ((ret = recvfrom(m_sockSrv, recvBuf, sizeof recvBuf, 0, NULL, NULL)) < 0) {  
		  if (ret == EWOULDBLOCK || ret == EAGAIN)  
		        TRACE("recvfrom timeout\n");  
	      else  
		        TRACE("recvfrom err:%d\n", ret);  
		}
		if(strcmp(recvBuf,"") == 0) return;
		
		m_lpRecvData->clear();
		m_lpRecvData = (LPSendData)recvBuf;

		UpdateEvaluation(m_lpRecvData);
		UpdateRunInfo(m_lpRecvData);
		//recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&len);   
		Invalidate();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CTeamServerDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
	int a = 0;
	if(!g_bMoveFirst)
	{
		int nWidth = m_rtMainPos.Width();
		int nHeight = m_rtMainPos.Height();
		m_rtMainPos.left = x;
		m_rtMainPos.top = y;
		m_rtMainPos.right = x + nWidth;
		m_rtMainPos.bottom = y + nHeight;
		WriteConfig();
	}
	else
	{
		g_bMoveFirst = FALSE;
	}
}
