
// ModifyProjectNameDlg.h : ͷ�ļ�
//

#pragma once


// CModifyProjectNameDlg �Ի���
class CModifyProjectNameDlg : public CDialogEx
{
// ����
public:
	CModifyProjectNameDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MODIFYPROJECTNAME_DIALOG };

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
	CString m_strProDir;
	CString m_strProName;
	CString m_strNewProName;
	CString m_strItemName;
	CString m_strNewItemName;
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedBtnSelectdir();
	afx_msg void OnBnClickedBtnSelectproname();
	afx_msg void OnBnClickedBtnSelectitemname();
};
