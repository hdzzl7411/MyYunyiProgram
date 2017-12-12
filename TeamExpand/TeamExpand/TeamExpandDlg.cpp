
// TeamExpandDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TeamExpand.h"
#include "TeamExpandDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFont * my_font = new CFont ( );
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
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


// CTeamExpandDlg �Ի���

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


// CTeamExpandDlg ��Ϣ�������

BOOL CTeamExpandDlg::OnInitDialog()
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
	
	m_brush.CreateSolidBrush ( RGB ( 0, 0, 0 ) );   //   ����һ��ɫˢ��
	UpdateEvaluation ( );
	UpdateRunInfo ( );
	SetTimer ( 1, 1000, NULL );
// 	m_bIsChangSize = FALSE;
// 	m_bIsMove = FALSE;
// 	nMoveX = nMoveY = 0;
// 	ReadConfig ( );
// 	mySetFont ( 32, my_font, m_strFontText.GetBuffer ( ) );
// 	DWORD dwStyle = m_ctList.GetExtendedStyle ( );
// 	dwStyle |= LVS_EX_FULLROWSELECT;    //ʹ���и���  
// 	dwStyle |= LVS_EX_GRIDLINES;//������  
// 	m_ctList.SetExtendedStyle ( dwStyle );
// 
// 	m_ctList.InsertColumn ( 0, "ID��", LVCFMT_LEFT, 400 );
// 	m_ctList.SetBkColor ( RGB ( 0, 0, 0 ) );
// 	m_ctList.SetTextColor ( RGB ( 255, 0, 0 ) );
// 	m_ctList.SetTextBkColor ( RGB ( 0, 0, 0 ) );
// 	m_ctList.SetFont ( my_font );
// 	m_ctList.SetOutlineColor ( RGB ( 255,0,0) );
// 	CString str;
// 	for ( int i = 0; i < 10; i++ )
// 	{
// 		str.Format ( "%d�Ŵ���  ����%sҵ��%d��  ������%d", i, m_strWinText[i], 10, 3 );
// 		m_ctList.InsertItem ( i, str.GetBuffer ( ) );
// 
// 	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTeamExpandDlg::OnPaint()
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
// 		CPaintDC dc ( this );
// 		CBitmap m_bmpBK;
// 		m_bmpBK.LoadBitmap ( IDB_BITMAP1 );
// 		CRect rect;  GetClientRect ( &rect );//���Ŀ��ߴ磬�����ڿͻ���������  
// 		BITMAP bitMap;//λͼ�ṹ��  
// 		m_bmpBK.GetBitmap ( &bitMap );//���ԭͼƬ�ߴ�    
// 		CDC dcMem; //Ŀ��DC  
// 		dcMem.CreateCompatibleDC ( &dc ); //������dc���ݵ��ڴ�DC  
// 		dcMem.SelectObject ( &m_bmpBK );//��λͼ����m_bmpBKѡ���ڴ�DC  
// 		dc.StretchBlt ( 0, 0, rect.Width ( ), rect.Height ( ), &dcMem, 0, 0, bitMap.bmWidth, bitMap.bmHeight, SRCCOPY );
// 		static HPEN hpenBlue;//����һ֧��ɫ�ʣ�
// 		hpenBlue = CreatePen ( PS_SOLID, 3, RGB ( 50, 105, 105 ) );
// 		CClientDC dc ( this );//thisΪ��ǰ����ָ��
// 		SelectObject ( dc, hpenBlue );//���ϲ�ɫ��
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

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
 	if ( nCtlColor == CTLCOLOR_DLG )      //�Ի�����ɫ  
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
		)  //��̬�ı���ɫ������ָ���ؼ�ID��  
	{
		pDC->SetTextColor ( RGB ( 255, 0, 0 ) );
		pDC->SetBkColor ( RGB ( 0, 0, 0 ) );     //��̬�ؼ��ı�����ɫ���ã����������ڱ�����ɫһ�£������������͸��
		pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  
		hbr = ( HBRUSH ) m_brush;
	}

	return hbr;
}


void CTeamExpandDlg::OnTimer ( UINT_PTR nIDEvent )
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	CReadTransactionInfo::GetInstance ( ).Initialize ( _T("C:\\Program Files (x86)\\p-an.com\\ƽ�����������Ŷӹ���ϵͳ\\Data\\Runtime\\QTransactionsFinished.Xml" ));
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
		strShowText.Format ( "%d�Ŵ���  %s%d��  ������%d", atoi(EvaIt->nCounterIndex), m_strWinText[atoi(EvaIt->nCounterIndex) - 1], EvaIt->nCount, EvaIt->nDiscontentCount );
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetFont ( my_font );
		GetDlgItem ( IDC_TRANSACTION1 + i )->SetWindowText ( strShowText );
		i++;
	}
}


void CTeamExpandDlg::UpdateRunInfo ( )
{
	int i = 0;
	CReadTransactionInfo::GetInstance ( ).Initialize ( _T("C:\\Program Files (x86)\\p-an.com\\ƽ�����������Ŷӹ���ϵͳ\\Data\\Runtime\\QTransactionsRunning.Xml") );
	CReadTransactionInfo::GetInstance ( ).InitRuningInfo ( );
	list<CSRuningInfo> lRunTmp = CReadTransactionInfo::GetInstance ( ).GetRunInfoList ( );
	
	for ( list<CSRuningInfo>::iterator RunIt = lRunTmp.begin ( ); RunIt != lRunTmp.end ( ); RunIt++ )
	{
		if ( atoi(RunIt->nCounterIndex) == 0 ) continue;
		CString  strShowText;
		strShowText.Format ( "��%s�ŵ�%02d�Ŵ���", RunIt->nTicketNumber, atoi(RunIt->nCounterIndex) );
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
		pWnd = GetDlgItem ( IDC_TRANSACTION1 + i );     //��ȡ�ؼ����
		if ( pWnd )//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
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
	GetClientRect ( &m_rtMainPos );//���仯��ĶԻ����С��Ϊ�ɴ�С
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
	lstrcpy ( lf.lfFaceName, "����" );
	
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	m_ptOrigin = point;
// 	SetCapture ( );
// 	m_bIsMove = TRUE;
	CDialogEx::OnLButtonDown ( nFlags, point );
}


void CTeamExpandDlg::OnLButtonUp ( UINT nFlags, CPoint point )
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	if ( m_bIsMove )
// 	{
// 		nMoveX = point.x - m_ptOrigin.x;
// 		nMoveY = point.y - m_ptOrigin.y;
// 	}
}


void CTeamExpandDlg::OnSize ( UINT nType, int cx, int cy )
{
	CDialogEx::OnSize ( nType, cx, cy );
	// TODO:  �ڴ˴������Ϣ����������
	UpdateAllControlPos ( cx, cy );
}


void CTeamExpandDlg::OnDestroy ( )
{
	if ( my_font != NULL ) my_font->DeleteObject ( );
	CDialogEx::OnDestroy ( );

	// TODO:  �ڴ˴������Ϣ����������
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
	if ( pWnd )//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;																	//��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect ( &rect );
		ScreenToClient ( &rect );											//���ؼ���Сת��Ϊ�ڶԻ����е���������
		//��cx/m_rect.Width()Ϊ�Ի����ں���ı仯����
		rect.left = rect.left*cx / nWidth;			//�����ؼ���С
		rect.right = rect.right*cx / nWidth;
		rect.top = rect.top*cy / nHeight;
		rect.bottom = rect.bottom*cy / nHeight;
		pWnd->MoveWindow ( rect );									//���ÿؼ���С
	}
}

LRESULT CTeamExpandDlg::OnNcHitTest ( CPoint point )
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint ptCur;
	CRect rect;
	GetCursorPos ( &ptCur );
	GetWindowRect ( &rect );
	//ģ�ⴰ�ڱ����������Ͻǡ����Ͻǡ����½ǡ����½ǡ���ߡ��ұߡ��ϱߡ��±ߵ�����
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�˺�������ģ�ⴰ�ڷ�����Ϣ��ϵͳ
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint ptCur;
	CRect rect;
	GetCursorPos ( &ptCur );
	GetWindowRect ( &rect );
	//����ϵͳ�����Ļ
	if ( rect.Width ( ) >= GetSystemMetrics ( SM_CXSCREEN ) - PADDING && rect.Height ( ) >= GetSystemMetrics ( SM_CYSCREEN ) - PADDING )
		return CDialog::OnSetCursor ( pWnd, nHitTest, message );
	//���ϽǺ����½�λ��
	if ( CRect ( rect.left, rect.top, rect.left + PADDING, rect.top + PADDING ).PtInRect ( ptCur ) || CRect ( rect.right - PADDING, rect.bottom - PADDING, rect.right, rect.bottom ).PtInRect ( ptCur ) )
	{
		SetCursor ( LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_SIZENWSE ) ) );
		return 0;
	}
	//���½Ǻ����Ͻǵ�λ��
	else if ( CRect ( rect.left, rect.bottom - PADDING, rect.left + PADDING, rect.bottom ).PtInRect ( ptCur ) || CRect ( rect.right - PADDING, rect.top , rect.right+ PADDING, rect.top + PADDING ).PtInRect ( ptCur ) )
	{
		SetCursor ( LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_SIZENESW ) ) );
		return 0;
	}
	//�ϱߺ��±ߵ�λ��
	else if ( CRect ( rect.left, rect.top, rect.right - WINMINIVALUE, rect.top + PADDING ).PtInRect ( ptCur ) || CRect ( rect.left, rect.bottom - PADDING, rect.right, rect.bottom ).PtInRect ( ptCur ) )
	{
		SetCursor ( LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_SIZENS ) ) );
		return 0;
	}
	//��ߺ��ұߵ�λ��
	else if ( CRect ( rect.left, rect.top, rect.left + PADDING, rect.bottom ).PtInRect ( ptCur ) || CRect ( rect.right - PADDING, rect.top, rect.right, rect.bottom ).PtInRect ( ptCur ) )
	{
		SetCursor ( LoadCursor ( NULL, MAKEINTRESOURCE ( IDC_SIZEWE ) ) );
		return 0;
	}
	return CDialogEx::OnSetCursor ( pWnd, nHitTest, message );
}
