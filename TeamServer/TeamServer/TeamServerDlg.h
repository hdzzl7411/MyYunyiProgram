
// TeamServerDlg.h : 头文件
//

#pragma once
#define			PADDING 5
#define			WINMINIVALUE   128
#define           SC_DRAGMOVE  0xF012                               //窗口拖拽移动的常量定义
#define			TITLEHEIGHT		30										//窗口标题的高度，只有在此高度内才可以实现窗口的拖拽移动
#include <string>
#include <list>
using namespace std;
#define  MAXSTAFFID_LEN		6
#define  MAXCOUNTER_LEN     3
#define  MAXCOUNTER_NUM		10
#define MAX_EVABUF  1024
#define MAX_RUNBUF	100
#define SAFE_DELETE(obj)		if(obj){delete obj; obj = NULL;}

#pragma pack(1)
typedef struct tagEvaluation
{
	char							nCounterIndex[MAXCOUNTER_LEN];                             //窗口号
	char							nStaffId[MAXSTAFFID_LEN];										//员工编号
	int							nCount;										//统计办理业务总数量
	int							nEvaType;									//评价类型
	int							nDiscontentCount;						//不满意统计数
	int							nDegree;										//满意统计数
	int							nVerysatisfy;								//非常满意统计数
	//unsigned char			strDate[11];									//业务发生日期
	tagEvaluation()
	{
		memcpy(nCounterIndex, "", MAXCOUNTER_LEN);
		memcpy(nStaffId, "", MAXSTAFFID_LEN);
		nCount = 0;
		nEvaType = -1;
		nDiscontentCount = 0;
		nDegree = 0;
		nVerysatisfy = 0;
	};
}CSEvaluation, *LPCSEvaluation;
typedef struct tagRuningInfo 
{
	char							nTicketNumber[MAXSTAFFID_LEN];							//票号
	char							nCounterIndex[MAXCOUNTER_LEN];                             //窗口号
	int							nTransactionStatus;						//业务状态
	tagRuningInfo()
	{
		memcpy(nCounterIndex, "", MAXCOUNTER_LEN);
		memcpy(nTicketNumber, "", MAXSTAFFID_LEN);
		nTransactionStatus = 0;
	};
}CSRuningInfo,*LPCSRuningInfo;

typedef struct TagSendData{
	int		nEvaCount;
	char	strEvaBuf[MAX_EVABUF];
	int		nRunInfoCount;
	char	strRunInfoBuf[MAX_RUNBUF];
	TagSendData()
	{
		nEvaCount = 0;
		nRunInfoCount = 0;
		memcpy(strEvaBuf,"",MAX_EVABUF);
		memcpy(strRunInfoBuf,"",MAX_RUNBUF);
	}
	void clear()
	{
		nEvaCount = 0;
		nRunInfoCount = 0;
		memcpy(strEvaBuf,"",MAX_EVABUF);
		memcpy(strRunInfoBuf,"",MAX_RUNBUF);
	}
	~TagSendData()
	{
// 		free(strRunInfoBuf);
// 		free(strEvaBuf);
	}
}CSSendData,*LPSendData;
#pragma pack()
// CTeamServerDlg 对话框
class CTeamServerDlg : public /*CChangeControlSzie*/CDialogEx
{
// 构造
public:
	CTeamServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEAMSERVER_DIALOG };

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

protected:
	CBrush   m_brush;
	CRect		m_rtMainPos;
	int			nWinWidth;
	int			nWinHeight;
	CFont *		my_font;
	string		m_strWinText[MAXCOUNTER_NUM];
	string		m_strFontText;
	int			m_nFontSize;
	unsigned int m_nPort;
	string		m_strIP;
	list<CSEvaluation> m_lEvaTmp;
	list<CSRuningInfo> m_lRunTmp;
	SOCKET			m_sockSrv;
	LPSendData		m_lpRecvData;

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	void mySetFont ( int height, CFont *font, char *name );
	void ReadConfig ( );
	void WriteConfig();
	void UpdateEvaluation (LPSendData lpRecvData);
	void UpdateRunInfo (LPSendData lpRecvData);
	void InitServerSocket();
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg void OnMove(int x, int y);
};
