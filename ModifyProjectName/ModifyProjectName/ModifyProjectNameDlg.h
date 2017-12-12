
// ModifyProjectNameDlg.h : 头文件
//

#pragma once


// CModifyProjectNameDlg 对话框
class CModifyProjectNameDlg : public CDialogEx
{
// 构造
public:
	CModifyProjectNameDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MODIFYPROJECTNAME_DIALOG };

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
