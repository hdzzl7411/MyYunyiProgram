
// TeamExpandDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ReadTransactionInfo.h"

#define			PADDING 5
#define			WINMINIVALUE   128
#define           SC_DRAGMOVE  0xF012                               //窗口拖拽移动的常量定义
#define			TITLEHEIGHT		30										//窗口标题的高度，只有在此高度内才可以实现窗口的拖拽移动
// CTeamExpandDlg 对话框
class CTeamExpandDlg : public CDialogEx
{
// 构造
public:
	CTeamExpandDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEAMEXPAND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Transaction1;
	afx_msg void OnSysCommand ( UINT nID, LPARAM lParam );
	afx_msg void OnPaint ( );
	afx_msg HBRUSH OnCtlColor ( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg LRESULT OnNcHitTest ( CPoint point );
	afx_msg void OnNcLButtonDown ( UINT nHitTest, CPoint point );
	afx_msg BOOL OnSetCursor ( CWnd* pWnd, UINT nHitTest, UINT message );
	afx_msg void OnLButtonDown ( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp ( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove ( UINT nFlags, CPoint point );
	afx_msg void OnSize ( UINT nType, int cx, int cy );
	afx_msg void OnDestroy ( );
	afx_msg void OnTimer ( UINT_PTR nIDEvent );
protected:
	CBrush   m_brush;
	RECT		m_rtMainPos;
	//BOOL		m_bIsChangSize;
	//BOOL		m_bIsMove;
	string		m_strWinText[10];
	string		m_strFontText;
	int			m_nFontSize;
	//CPoint		m_ptOrigin;				//鼠标按下起始位置，用来计算鼠标移动的高度和宽度
	//int			nMoveX;
	//int			nMoveY;
	int			nWinWidth;
	int			nWinHeight;
private:
	void UpdateEvaluation ( );
	void UpdateRunInfo ( );
	void UpdateAllControlPos ( int cx, int cy );
	void mySetFont ( int height, CFont *font, char *name );
	void ReadConfig ( );
	void		AdjustControl ( CWnd * pWnd ,int cx,int cy);
public:
	CListCtrl m_ctList;
};
