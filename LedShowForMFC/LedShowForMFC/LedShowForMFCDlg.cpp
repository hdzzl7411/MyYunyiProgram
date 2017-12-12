
// LedShowForMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LedShowForMFC.h"
#include "LedShowForMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CLedShowForMFCDlg �Ի���




CLedShowForMFCDlg::CLedShowForMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLedShowForMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLedShowForMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLedShowForMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLedShowForMFCDlg ��Ϣ�������

BOOL CLedShowForMFCDlg::OnInitDialog()
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
	ReadConfigFile();
	m_brush.CreateSolidBrush (m_backColor);   //   ����һ��ɫˢ��
	this->SetWindowPos(&wndTopMost,m_rectScreenPos.left,m_rectScreenPos.top,m_rectScreenPos.Width(),m_rectScreenPos.Height() ,SWP_SHOWWINDOW);

	m_font = new CFont();  // �������
	m_stcShowText = new CStatic();
	m_nCurrentCount = 1;
	SetShowTextFont (m_nFontSize, m_font, m_strFontText.GetBuffer());
	InitSize();
	m_stcShowText->Create("���ǳ�ʼ�Ĳ����ı�", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(m_nWidth - 30, 0, m_nWidth - 400 + 30, m_nHeight), this, IDC_SHOWTEXT);
	m_stcShowText->SetFont(m_font, FALSE);
	m_stcShowText->ModifyStyle(SS_CENTER|SS_RIGHT,SS_LEFT,TRUE);
	DWORD dwNameLen = MAX_BUFFER_LEN;  
	if (!GetComputerName(m_szComputerName, &dwNameLen))   
		TRACE("Error  %d\n", GetLastError());  
	else  
		TRACE("�������Ϊ: %s\n", m_szComputerName);
	m_queue = new CQueueOperate();

	wchar_t name[]=L".\\PRIVATE$\\CdmLedC";

	DWORD bufferLength = 256;

	wchar_t formattedQueueName[256]; 
	HRESULT returnValue = m_queue->CreateMSMQQueue(name,NULL,formattedQueueName,&bufferLength);

	if(returnValue != MQ_OK)
		wprintf(L"Creating a Queue failed\n");
	else
	{
		wprintf(L"Queue was successfully created..Formatted QueueName =%s\n",formattedQueueName);
		wprintf(L"LEn returned is %d\n", bufferLength);
	}

	//SetTimer(1, 550 - 50*m_nMoveSpeed, NULL); 
	_beginthread(ReadQueueThread,0,this);
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CLedShowForMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLedShowForMFCDlg::OnPaint()
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
// 		CPaintDC dcSta(GetDlgItem(IDC_SHOWTEXT));
// 		RECT rect;
// 		GetDlgItem(IDC_SHOWTEXT)->GetClientRect(&rect);
// 		dcSta.DrawText("��˹�ٻ��ֿ��ϴ�ɻ�",100,&rect,DT_SINGLELINE | DT_LEFT);
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLedShowForMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLedShowForMFCDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetWindowRect(&m_rectScreenPos);
	WriteConfigFile();
	if(m_stcShowText != NULL)
	{
		delete m_stcShowText;
		m_stcShowText = NULL;
	}
	if(m_font != NULL)
	{
		delete m_font;
		m_font = NULL;
	}
	if(m_queue != NULL)
	{
		delete m_queue;
		m_queue = NULL;
	}
	CDialogEx::OnClose();
}


HBRUSH CLedShowForMFCDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	if ( nCtlColor == CTLCOLOR_DLG )      //�Ի�����ɫ  
		return   m_brush;   
	if(nCtlColor == CTLCOLOR_STATIC)  
	{  
		pDC->SetBkMode(TRANSPARENT);   //����͸��  
		pDC->SetTextColor(m_textColor);  // �����ı���ɫ   
		return (HBRUSH)::GetStockObject(NULL_BRUSH);     
	}
	return hbr;
}


void CLedShowForMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		UpdateShowText();
		if(m_nCurrentCount > m_nMoveCount)
		{
			m_nCurrentCount = 1;
			KillTimer(1);
			return;
		}
		CString strShowText;
		m_stcShowText->GetWindowText(strShowText);
		if(m_bLeftShow)
		{
			m_stcShowText->SetWindowPos(NULL, m_nWidth - 30, 0, strShowText.GetLength()*30, m_nHeight, SWP_NOZORDER);
			m_nWidth = m_nWidth - 30;
			if (m_nWidth < -strShowText.GetLength()*30) {
				KillTimer(1);
				InitSize();
				m_nCurrentCount++;
				m_stcShowText->SetWindowPos(NULL, m_nWidth - 30, 0, strShowText.GetLength()*30, m_nHeight, SWP_NOZORDER);
				SetTimer(1, 550 - 50*m_nMoveSpeed, NULL);
			}
		}
		else
		{
			m_stcShowText->SetWindowPos(NULL, 0, m_nHeight - 20, m_nWidth, m_nMsgNum*150, SWP_NOZORDER);
			m_nHeight = m_nHeight - 20;
			if (m_nHeight < -m_nMsgNum*150) {
				KillTimer(1);
				InitSize();
				m_nCurrentCount++;
				m_stcShowText->SetWindowPos(NULL, 0, m_nHeight, m_nWidth, m_nMsgNum*150, SWP_NOZORDER);
				SetTimer(1, 550 - 50*m_nMoveSpeed, NULL);
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


LRESULT CLedShowForMFCDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	return CDialogEx::OnNcHitTest(point);
}


void CLedShowForMFCDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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


BOOL CLedShowForMFCDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CLedShowForMFCDlg::SetShowTextFont(int height, CFont *font, char *name)
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

void CLedShowForMFCDlg::InitSize()
{
	CRect cRect;
	GetClientRect(cRect); // ��ȡ���ڵ�λ�úʹ�С
	m_nWidth = cRect.Width();
	m_nHeight = cRect.Height();
}

void CLedShowForMFCDlg::ReadConfigFile()
{
	CString  PathFileName;
	PathFileName.Format ( "%sLEDConfig.ini", GetAppPath ( ).c_str());
	char strBuf[100] = { 0 };
	::GetPrivateProfileString ( "Setting", "Font", 0, strBuf, 100, PathFileName );
	m_strFontText.Format("%s",strBuf);
	::GetPrivateProfileString ( "Setting", "FontSize", 0, strBuf, 100, PathFileName );
	m_nFontSize = atoi ( strBuf );
	::GetPrivateProfileString ( "Setting", "ShowInfoNum", 0, strBuf, 100, PathFileName );
	m_nMsgNum = atoi ( strBuf );

	::GetPrivateProfileString ( "WinPos", "Left", 0, strBuf, 100, PathFileName );
	m_rectScreenPos.left = atoi ( strBuf );
	::GetPrivateProfileString ( "WinPos", "Top", 0, strBuf, 100, PathFileName );
	m_rectScreenPos.top = atoi ( strBuf );
	::GetPrivateProfileString ( "WinPos", "Right", 0, strBuf, 100, PathFileName );
	m_rectScreenPos.right = atoi ( strBuf );
	::GetPrivateProfileString ( "WinPos", "Bottom", 0, strBuf, 100, PathFileName );
	m_rectScreenPos.bottom = atoi ( strBuf );

	::GetPrivateProfileString ( "Move", "Speed", 0, strBuf, 100, PathFileName );
	m_nMoveSpeed = atoi ( strBuf );
	if(m_nMoveSpeed > 10) m_nMoveSpeed = 10;
	if(m_nMoveSpeed < 1) m_nMoveSpeed = 1;
	::GetPrivateProfileString ( "Move", "Time", 0, strBuf, 100, PathFileName );
	m_nMoveCount = atoi ( strBuf );
	::GetPrivateProfileString ( "Move", "Mode", 0, strBuf, 100, PathFileName );
	if(atoi ( strBuf ) == 2)
		m_bLeftShow = false;
	else
		m_bLeftShow = true;
	::GetPrivateProfileString ( "Color", "Background", 0, strBuf, 100, PathFileName );
	SetBackColor(atoi(strBuf));
	::GetPrivateProfileString ( "Color", "Text", 0, strBuf, 100, PathFileName );
	SetTextColor(atoi(strBuf));
}

void CLedShowForMFCDlg::WriteConfigFile()
{
	CString  PathFileName;
	PathFileName.Format ( "%sLEDConfig.ini", GetAppPath ( ).c_str());
	CString strValue;
	strValue.Format("%d",m_rectScreenPos.left);
	::WritePrivateProfileString("WinPos", "Left",strValue.GetBuffer(),PathFileName);
	strValue.Format("%d",m_rectScreenPos.top);
	::WritePrivateProfileString("WinPos", "Top",strValue.GetBuffer(),PathFileName);
	strValue.Format("%d",m_rectScreenPos.right);
	::WritePrivateProfileString("WinPos", "Right",strValue.GetBuffer(),PathFileName);
	strValue.Format("%d",m_rectScreenPos.bottom);
	::WritePrivateProfileString("WinPos", "Bottom",strValue.GetBuffer(),PathFileName);
}

void CLedShowForMFCDlg::SetBackColor(int nColor)
{
	switch(nColor)
	{
		case 0:
			m_backColor = RGB(0,0,0);
			break;
		case 1:
			m_backColor = RGB(255,255,255);
			break;
		case 2:
			m_backColor = RGB(255,0,0);
			break;
		case 3:
			m_backColor = RGB(0,255,0);
			break;
		case 4:
			m_backColor = RGB(0,0,255);
			break;
		case 5:
			m_backColor = RGB(255,255,0);
			break;
		case 6:
			m_backColor = RGB(0,255,255);
			break;
		case 7:
			m_backColor = RGB(255,97,0);
			break;
		case 8:
			m_backColor = RGB(160,32,240);
			break;
		default:
			m_backColor = RGB(0,0,0);
			break;
	}
}

void CLedShowForMFCDlg::SetTextColor(int nColor)
{
	switch(nColor)
	{
		case 0:
			m_textColor = RGB(0,0,0);
			break;
		case 1:
			m_textColor = RGB(255,255,255);
			break;
		case 2:
			m_textColor = RGB(255,0,0);
			break;
		case 3:
			m_textColor = RGB(0,255,0);
			break;
		case 4:
			m_textColor = RGB(0,0,255);
			break;
		case 5:
			m_textColor = RGB(255,255,0);
			break;
		case 6:
			m_textColor = RGB(0,255,255);
			break;
		case 7:
			m_textColor = RGB(255,97,0);
			break;
		case 8:
			m_textColor = RGB(160,32,240);
			break;
		default:
			m_textColor = RGB(255,0,0);
			break;
	}
}


void CLedShowForMFCDlg::ReadQueueMsg()
{
	char strName[] = "PRIVATE$\\CdmLedC";
	char strRetMsg[1024] = "";
	HRESULT returnValue = m_queue->ReadingDestQueue(strName,m_szComputerName,strRetMsg);
	if(!(strcmp(strRetMsg,"") == 0))
	{
		string strTemp = strRetMsg;
		if(m_listShow.size() > m_nMsgNum - 1)
		{
			m_listShow.pop_front();
		}
		m_listShow.push_back(strTemp);
		SetTimer(1,550 - 50 * m_nMoveSpeed,NULL);
	}
}
std::list<CString> CLedShowForMFCDlg::GetStringRows(CString text, int width)
{
	int RowBeginIndex = 0;
	int rowEndIndex = 0;
	int textLength = text.GetLength();
	std::list<CString> textRows;
	//float fRate = m_nFontSize;
	for (int index = 0; index < textLength; index++)
	{
		rowEndIndex = index;
		//float nWidth = graphic.MeasureString(text.substr(RowBeginIndex, rowEndIndex - RowBeginIndex + 1), font).Width;
		if (index == textLength - 1)
		{
			textRows.push_back(text.Mid(RowBeginIndex));
		}
		else if (rowEndIndex + 1 < text.GetLength() && text.Mid(rowEndIndex, 2) == "\r\n")
		{
			textRows.push_back(text.Mid(RowBeginIndex, rowEndIndex - RowBeginIndex));
			rowEndIndex = index += 2;
			RowBeginIndex = rowEndIndex;
		}
		else if (text.Mid(RowBeginIndex, rowEndIndex - RowBeginIndex + 1).GetLength()*m_nFontSize> width)
		{
			textRows.push_back(text.Mid(RowBeginIndex, rowEndIndex - RowBeginIndex));
			RowBeginIndex = rowEndIndex;
		}
	}
	return textRows;
}
void CLedShowForMFCDlg::UpdateShowText()
{
	CString strShowText = "";

	for(list<string>::iterator it = m_listShow.begin();it != m_listShow.end();++it)
	{
		strShowText += it->c_str();
// 		list<CString> textRows = GetStringRows(it->c_str(),m_nWidth);
// 		for(list<CString>::iterator rowit = textRows.begin();rowit != textRows.end();++rowit)
// 		{
// 			strShowText += rowit->GetBuffer();
// 			strShowText += "\n";
// 		}
		if(m_bLeftShow)
			strShowText += "\t";
		else
			strShowText += "\r\n";
	}
	m_stcShowText->SetWindowText(strShowText);
	Invalidate();
}

void CLedShowForMFCDlg::ReadQueueThread(void *pParam)
{
	CLedShowForMFCDlg *handle = (CLedShowForMFCDlg*)pParam;
	while(1)
	{
		handle->ReadQueueMsg();
		Sleep(50);
	}
}

void CLedShowForMFCDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static int bh = 1000;
	char strName[] = "PRIVATE$\\CdmLedC";
	CString strMsg;
	strMsg.Format("�ŶӺ�%dҵ����������ɣ��뵽�ɷѴ��ڡ�",bh);
	//char strMsg[] = "���Ƕ���ʹ�õĲ�����Ϣ�������ɲ�����ȡ����Ϣ��";
	HRESULT returnValue = m_queue->QueueSendMessage(strName,m_szComputerName,strMsg.GetBuffer());
	bh++;
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CLedShowForMFCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	InitSize();
	// TODO: �ڴ˴������Ϣ����������
}
