
// LedShowForMFCDlg.h : ͷ�ļ�
//

#pragma once
#include "QueueOperate.h"
#include <list>
#include <string>

using namespace std;
#define			PADDING 5
#define			WINMINIVALUE   128
#define           SC_DRAGMOVE  0xF012                               //������ק�ƶ��ĳ�������
#define			TITLEHEIGHT		30		
const int MAX_BUFFER_LEN = 255;
// CLedShowForMFCDlg �Ի���
class CLedShowForMFCDlg : public CDialogEx
{
// ����
public:
	CLedShowForMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_LEDSHOWFORMFC_DIALOG };

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
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
private:
	CStatic *m_stcShowText;
	CFont *m_font;
	int m_nWidth; // ���
	int m_nHeight; // �߶�
	std::list<string> m_listShow;

	CBrush		m_brush;
	CRect		m_rectScreenPos;
	CQueueOperate *m_queue;
	char		m_szComputerName[MAX_BUFFER_LEN];
	CString		m_strFontText;
	int			m_nFontSize;
	int			m_nMoveSpeed;
	int			m_nMoveCount;
	int			m_nCurrentCount;
	int			m_nMsgNum;
	bool		m_bLeftShow;
	COLORREF	m_backColor;
	COLORREF	m_textColor;
public:
	void SetShowTextFont(int height, CFont *font, char *name);
	void InitSize();
	void ReadConfigFile();
	void WriteConfigFile();
	void SetBackColor(int nColor);
	void SetTextColor(int nColor);
	void ReadQueueMsg();
	void UpdateShowText();
	static void ReadQueueThread(void *pParam);
	std::list<CString> GetStringRows(CString text, int width);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
