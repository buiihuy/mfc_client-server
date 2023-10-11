
// ChatClientDlg.h : header file
//

#pragma once
#include "ClientSocket.h"
#include "Data.h"


// CChatClientDlg dialog
class CChatClientDlg : public CDialogEx
{
// Construction
public:
	CClientSocket m_Client;
	void AddMsg(char* buff);
	CChatClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonSend();
	CListBox m_Display;
	CString m_sUserName;
	CString m_sMsg;
};
