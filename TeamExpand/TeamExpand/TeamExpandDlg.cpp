
// TeamExpandDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeamExpand.h"
#include "TeamExpandDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFont * my_font = new CFont ( );
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
string GetAppFullPath ( )
{
	TCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName ( NULL, szPath, MAX_PATH );

	return szPath;
}

string GetAppPath ( )
{
	string strFullPath = GetAppFullPath ( );

	return strFullPath.substr ( 0, strFullPath.rfind ( '\\' ) + 1 );
}
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


// CTeamExpandDlg 对话框

CTeamExpandDlg::CTeamExpandDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeamExpandDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTeamExpandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Control ( pDX, IDC_TRANSACTION1, m_Transaction1 );
	DDX_Control ( pDX, IDC_LIST2, m_ctList );
}

BEGIN_MESSAGE_MAP(CTeamExpandDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR ( )
	ON_WM_TIMER ( )
	ON_WM_LBUTTONDOWN ( )
	ON_WM_LBUTTONUP ( )
	ON_WM_MOUSEMOVE ( )
	ON_WM_SIZE ( )
	ON_WM_DESTROY ( )
	ON_WM_NCHITTEST ( )
	ON_WM_NCLBUTTONDOWN ( )
	ON_WM_SETCURSOR ( )
END_MESSAGE_MAP ( )


// CTeamExpandDlg 消息处理程序

BOOL CTeamExpandDlg::OnInitDialog()
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
	for ( int i = 0; i < 10; i++ )
	{
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetWindowText ( _T("") );
		if ( i < 8 )
		{
			GetDlgItem ( IDC_CALLINFO1 + i )->SetWindowText ( _T ( "" ) );
		}
	}

	GetClientRect ( &m_rtMainPos );
	nWinWidth = m_rtMainPos.right - m_rtMainPos.left;
	nWinHeight = m_rtMainPos.bottom - m_rtMainPos.top;
	
	m_brush.CreateSolidBrush ( RGB ( 0, 0, 0 ) );   //   生成一黑色刷子
	UpdateEvaluation ( );
	UpdateRunInfo ( );
	SetTimer ( 1, 1000, NULL );
// 	m_bIsChangSize = FALSE;
// 	m_bIsMove = FALSE;
// 	nMoveX = nMoveY = 0;
// 	ReadConfig ( );
// 	mySetFont ( 32, my_font, m_strFontText.GetBuffer ( ) );
// 	DWORD dwStyle = m_ctList.GetExtendedStyle ( );
// 	dwStyle |= LVS_EX_FULLROWSELECT;    //使整行高亮  
// 	dwStyle |= LVS_EX_GRIDLINES;//网格线  
// 	m_ctList.SetExtendedStyle ( dwStyle );
// 
// 	m_ctList.InsertColumn ( 0, "ID号", LVCFMT_LEFT, 400 );
// 	m_ctList.SetBkColor ( RGB ( 0, 0, 0 ) );
// 	m_ctList.SetTextColor ( RGB ( 255, 0, 0 ) );
// 	m_ctList.SetTextBkColor ( RGB ( 0, 0, 0 ) );
// 	m_ctList.SetFont ( my_font );
// 	m_ctList.SetOutlineColor ( RGB ( 255,0,0) );
// 	CString str;
// 	for ( int i = 0; i < 10; i++ )
// 	{
// 		str.Format ( "%d号窗口  办理%s业务%d个  不允许%d", i, m_strWinText[i], 10, 3 );
// 		m_ctList.InsertItem ( i, str.GetBuffer ( ) );
// 
// 	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTeamExpandDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTeamExpandDlg::OnPaint()
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
// 		CPaintDC dc ( this );
// 		CBitmap m_bmpBK;
// 		m_bmpBK.LoadBitmap ( IDB_BITMAP1 );
// 		CRect rect;  GetClientRect ( &rect );//获得目标尺寸，即窗口客户区的坐标  
// 		BITMAP bitMap;//位图结构体  
// 		m_bmpBK.GetBitmap ( &bitMap );//获得原图片尺寸    
// 		CDC dcMem; //目标DC  
// 		dcMem.CreateCompatibleDC ( &dc ); //创建与dc兼容的内存DC  
// 		dcMem.SelectObject ( &m_bmpBK );//将位图对象m_bmpBK选入内存DC  
// 		dc.StretchBlt ( 0, 0, rect.Width ( ), rect.Height ( ), &dcMem, 0, 0, bitMap.bmWidth, bitMap.bmHeight, SRCCOPY );
// 		static HPEN hpenBlue;//申明一支彩色笔；
// 		hpenBlue = CreatePen ( PS_SOLID, 3, RGB ( 50, 105, 105 ) );
// 		CClientDC dc ( this );//this为当前窗口指针
// 		SelectObject ( dc, hpenBlue );//换上彩色笔
// 		RECT	rect = { 30, 38, 770, 375 };
// 		dc.Rectangle ( &rect);
// 		CPoint pt1, pt2;
// 		pt1.x = m_rtMainPos.Width ( ) * 3 / 80;
// 		pt1.y = 36;
// 		pt2.x = m_rtMainPos.Width ( ) - 30;
// 		pt2.y =  36 ;
// 		dc.MoveTo ( pt1 );
// 		dc.LineTo ( pt2 );
// 		for ( int i = 0; i < 10; i++ )
// 		{
// 			pt1.y = pt1.y + m_rtMainPos.Height ( ) * 34 / 600;
// 			pt2.y = pt2.y + m_rtMainPos.Height ( ) * 34 / 600;
// 			dc.MoveTo ( pt1 );
// 			dc.LineTo ( pt2 );
// 		}
// 		hpenBlue = CreatePen ( PS_SOLID, 3, RGB ( 0, 0, 255 ) );
// 		CClientDC dc1 ( this );
// 		SelectObject ( dc1, hpenBlue );
// 		pt1.x = 0;
// 		pt1.y =400;
// 		pt2.x = 800;
// 		pt2.y = 400;
// 		dc1.MoveTo ( pt1 );
// 		dc1.LineTo ( pt2 );
		
		CDialogEx::OnPaint();
	}
}

HBRUSH CTeamExpandDlg::OnCtlColor ( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	HBRUSH hbr = CDialogEx::OnCtlColor ( pDC, pWnd, nCtlColor );

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


void CTeamExpandDlg::OnTimer ( UINT_PTR nIDEvent )
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	switch ( nIDEvent )
	{
	case 1:
		UpdateEvaluation ( );
		UpdateRunInfo ( );
		break;
	default:
		UpdateEvaluation ( );
		UpdateRunInfo ( );
		break;
	}
	CDialogEx::OnTimer ( nIDEvent );
}


void CTeamExpandDlg::UpdateEvaluation ( )
{
	int i = 0;
	CReadTransactionInfo::GetInstance ( ).Initialize ( _T("C:\\Program Files (x86)\\p-an.com\\平安力合智能排队管理系统\\Data\\Runtime\\QTransactionsFinished.Xml" ));
	CReadTransactionInfo::GetInstance ( ).InitEvaluation ( );
	list<CSEvaluation> lEvaTmp = CReadTransactionInfo::GetInstance ( ).GetEvaluationList ( );
	ReadConfig ( );
	mySetFont ( m_nFontSize, my_font, (char *)m_strFontText.c_str( ) );
	for ( int i = 0; i < 10; i++ )
	{
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetFont ( my_font );
	}
	for ( list<CSEvaluation>::iterator EvaIt = lEvaTmp.begin ( ); EvaIt != lEvaTmp.end ( ); EvaIt++ )
	{
		CString  strShowText;
		strShowText.Format ( "%d号窗口  %s%d个  不允许%d", atoi(EvaIt->nCounterIndex), m_strWinText[atoi(EvaIt->nCounterIndex) - 1], EvaIt->nCount, EvaIt->nDiscontentCount );
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetFont ( my_font );
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetWindowText ( strShowText );
		i++;
	}
}


void CTeamExpandDlg::UpdateRunInfo ( )
{
	int i = 0;
	CReadTransactionInfo::GetInstance ( ).Initialize ( _T("C:\\Program Files (x86)\\p-an.com\\平安力合智能排队管理系统\\Data\\Runtime\\QTransactionsRunning.Xml") );
	CReadTransactionInfo::GetInstance ( ).InitRuningInfo ( );
	list<CSRuningInfo> lRunTmp = CReadTransactionInfo::GetInstance ( ).GetRunInfoList ( );
	
	for ( list<CSRuningInfo>::iterator RunIt = lRunTmp.begin ( ); RunIt != lRunTmp.end ( ); RunIt++ )
	{
		if ( atoi(RunIt->nCounterIndex) == 0 ) continue;
		CString  strShowText;
		strShowText.Format ( "请%s号到%02d号窗口", RunIt->nTicketNumber, atoi(RunIt->nCounterIndex) );
		GetDlgItem ( IDC_CALLINFO1 + i )->SetWindowText ( strShowText );
		GetDlgItem ( IDC_CALLINFO1 + i )->SetFont ( my_font );
		i++;
	}
}


void CTeamExpandDlg::UpdateAllControlPos ( int cx, int cy )
{
	CWnd *pWnd;
// 	pWnd = GetDlgItem ( IDC_STCLINE );
// 	if ( pWnd )
// 	{
// 		AdjustControl ( pWnd,cx,cy );
// 	}
	for ( int i = 0; i < 10; i++ )
	{
		pWnd = GetDlgItem ( IDC_TRANSACTION1 + i );     //获取控件句柄
		if ( pWnd )//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		{
			AdjustControl ( pWnd, cx, cy );
		}
		if ( i < 8 )
		{
			pWnd = GetDlgItem ( IDC_CALLINFO1 + i );
			if ( pWnd )
			{
				AdjustControl ( pWnd, cx, cy );
			}
		}
	}
	GetClientRect ( &m_rtMainPos );//将变化后的对话框大小设为旧大小
	nWinWidth = m_rtMainPos.right - m_rtMainPos.left;
	nWinHeight = m_rtMainPos.bottom - m_rtMainPos.top;
	Invalidate ( );
	
}

void CTeamExpandDlg::mySetFont ( int height, CFont *font, char *name )
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

void CTeamExpandDlg::OnLButtonDown ( UINT nFlags, CPoint point )
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
// 	m_ptOrigin = point;
// 	SetCapture ( );
// 	m_bIsMove = TRUE;
	CDialogEx::OnLButtonDown ( nFlags, point );
}


void CTeamExpandDlg::OnLButtonUp ( UINT nFlags, CPoint point )
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
// 	if ( m_bIsMove && (nMoveX != 0 || nMoveY != 0) )
// 	{
// 		GetClientRect ( &m_rtMainPos );
// 		ClientToScreen ( &m_rtMainPos );
// 		m_rtMainPos.left = ((m_rtMainPos.left + nMoveX) < 0) ? 0 : (m_rtMainPos.left + nMoveX);
// 		m_rtMainPos.right = m_rtMainPos.left + nWinWidth;
// 		m_rtMainPos.top = ((m_rtMainPos.top + nMoveY) < 0) ? 0 : (m_rtMainPos.top + nMoveY);
// 		m_rtMainPos.bottom = m_rtMainPos.top + nWinHeight;
// 		MoveWindow ( &m_rtMainPos );
// 		nMoveX = nMoveY = 0;
// 		m_bIsMove = FALSE;
// 	}
// 	ReleaseCapture ( );
	CDialogEx::OnLButtonUp ( nFlags, point );
}


void CTeamExpandDlg::OnMouseMove ( UINT nFlags, CPoint point )
{
	CDialogEx::OnMouseMove ( nFlags, point );
	// TODO:  在此添加消息处理程序代码和/或调用默认值
// 	if ( m_bIsMove )
// 	{
// 		nMoveX = point.x - m_ptOrigin.x;
// 		nMoveY = point.y - m_ptOrigin.y;
// 	}
}


void CTeamExpandDlg::OnSize ( UINT nType, int cx, int cy )
{
	CDialogEx::OnSize ( nType, cx, cy );
	// TODO:  在此处添加消息处理程序代码
	UpdateAllControlPos ( cx, cy );
}


void CTeamExpandDlg::OnDestroy ( )
{
	if ( my_font != NULL ) my_font->DeleteObject ( );
	CDialogEx::OnDestroy ( );

	// TODO:  在此处添加消息处理程序代码
}

void CTeamExpandDlg::ReadConfig ( )
{
	CString  PathFileName;
	PathFileName.Format ( "%sShowConfig.ini", GetAppPath ( ).c_str());
	char strBuf[100] = { 0 };
	::GetPrivateProfileString ( "Setting", "Font", 0, strBuf, 100, PathFileName );
	m_strFontText = strBuf;
	::GetPrivateProfileString ( "Setting", "FontSize", 0, strBuf, 100, PathFileName );
	m_nFontSize = atoi ( strBuf );
	for ( int i = 0; i < 10; i++ )
	{
		char strName[100] = { 0 };
		sprintf ( strName, "Win%d", i + 1 );
		::GetPrivateProfileString ( "Transaction", strName, "", strBuf, 100, PathFileName );
		m_strWinText[i] = strBuf ;
	}
}

void CTeamExpandDlg::AdjustControl ( CWnd * pWnd, int cx, int cy )
{
	int nHeight = m_rtMainPos.bottom - m_rtMainPos.top;
	int nWidth = m_rtMainPos.right - m_rtMainPos.left;
	if ( pWnd )//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;																	//获取控件变化前大小
		pWnd->GetWindowRect ( &rect );
		ScreenToClient ( &rect );											//将控件大小转换为在对话框中的区域坐标
		//　cx/m_rect.Width()为对话框在横向的变化比例
		rect.left = rect.left*cx / nWidth;			//调整控件大小
		rect.right = rect.right*cx / nWidth;
		rect.top = rect.top*cy / nHeight;
		rect.bottom = rect.bottom*cy / nHeight;
		pWnd->MoveWindow ( rect );									//设置控件大小
	}
}

LRESULT CTeamExpandDlg::OnNcHitTest ( CPoint point )
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	return CDialogEx::OnNcHitTest ( point );
}


void CTeamExpandDlg::OnNcLButtonDown ( UINT nHitTest, CPoint point )
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//此函数功能模拟窗口发送消息给系统
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
	CDialogEx::OnNcLButtonDown ( nHitTest, point );
}


BOOL CTeamExpandDlg::OnSetCursor ( CWnd* pWnd, UINT nHitTest, UINT message )
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	return CDialogEx::OnSetCursor ( pWnd, nHitTest, message );
}
