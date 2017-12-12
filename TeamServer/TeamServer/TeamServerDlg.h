
// TeamServerDlg.h : ͷ�ļ�
//

#pragma once
#define			PADDING 5
#define			WINMINIVALUE   128
#define           SC_DRAGMOVE  0xF012                               //������ק�ƶ��ĳ�������
#define			TITLEHEIGHT		30										//���ڱ���ĸ߶ȣ�ֻ���ڴ˸߶��ڲſ���ʵ�ִ��ڵ���ק�ƶ�
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
	char							nCounterIndex[MAXCOUNTER_LEN];                             //���ں�
	char							nStaffId[MAXSTAFFID_LEN];										//Ա�����
	int							nCount;										//ͳ�ư���ҵ��������
	int							nEvaType;									//��������
	int							nDiscontentCount;						//������ͳ����
	int							nDegree;										//����ͳ����
	int							nVerysatisfy;								//�ǳ�����ͳ����
	//unsigned char			strDate[11];									//ҵ��������
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
	char							nTicketNumber[MAXSTAFFID_LEN];							//Ʊ��
	char							nCounterIndex[MAXCOUNTER_LEN];                             //���ں�
	int							nTransactionStatus;						//ҵ��״̬
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
// CTeamServerDlg �Ի���
class CTeamServerDlg : public /*CChangeControlSzie*/CDialogEx
{
// ����
public:
	CTeamServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEAMSERVER_DIALOG };

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
