
// LedShowForMFCDlg.h : 头文件
//

#pragma once
#include "QueueOperate.h"
#include <list>
#include <string>

using namespace std;
#define			PADDING 5
#define			WINMINIVALUE   128
#define           SC_DRAGMOVE  0xF012                               //窗口拖拽移动的常量定义
#define			TITLEHEIGHT		30		
const int MAX_BUFFER_LEN = 255;
// CLedShowForMFCDlg 对话框
class CLedShowForMFCDlg : public CDialogEx
{
// 构造
public:
	CLedShowForMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LEDSHOWFORMFC_DIALOG };

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
	int m_nWidth; // 宽度
	int m_nHeight; // 高度
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
