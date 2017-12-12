
// TeamClientDlg.h : ͷ�ļ�
//

#pragma once
#define MAX_EVABUF  1024
#define MAX_RUNBUF	100

#include "ReadTransactionInfo.h"
#pragma pack(1)
typedef struct TagSendData{
	unsigned int		nEvaCount;
	unsigned char	strEvaBuf[MAX_EVABUF];
	unsigned int		nRunInfoCount;
	unsigned char	strRunInfoBuf[MAX_RUNBUF];
	TagSendData()
	{
		nEvaCount = 0;
		nRunInfoCount = 0;
		memcpy(strEvaBuf, "", MAX_EVABUF);
		memcpy(strRunInfoBuf, "", MAX_RUNBUF);
	}
}CSSendData,*LPSendData;
#include <list>
#include <string>
using namespace std;

#pragma pack()
// CTeamClientDlg �Ի���
class CTeamClientDlg : public CDialogEx
{
// ����
public:
	CTeamClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEAMCLIENT_DIALOG };

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
	afx_msg void OnBnClickedOk();
private:
	void InitSocket();
	void SendDataToServer();
	void MakeSendData();
	void ReadConfig();
private:
	LPSendData			m_SendData;
	std::string			m_strServerIP;
	unsigned int		m_nPort;
	SOCKET				m_sockClient;
	string				m_strPath;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
