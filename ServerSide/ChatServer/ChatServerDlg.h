
// ChatServerDlg.h : header file
//

#pragma once
#include "Data.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Channel.h"
#include<iostream>
#include<queue>
#include<vector>


// CChatServerDlg dialog
class CChatServerDlg : public CDialogEx
{
// Construction
public:
	void AddMsg(char* str, CClientSocket* m_sClient);
	// init server socket
	CServerSocket m_sServer;

	// manage channels in server
	std::vector<channel*> channels;

	CChatServerDlg(CWnd* pParent = nullptr);	// standard constructor
	//void Accept();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
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
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	DECLARE_MESSAGE_MAP()
public:
	
	CListBox m_Display;
	//CString m_Msg;
	afx_msg void OnLbnSelchangeList1();
public:
	void NICK(char* str, CClientSocket* m_sClient);
	void QUIT(char* str, CClientSocket* m_sClient);
	void JOIN(char* str, CClientSocket* m_sClient);
	void PART(CClientSocket* m_sClient);
	void LIST(char* str, CClientSocket* m_sClient);
	void PRIVMSG(char* str, CClientSocket* m_sClient);
	void WHO(char* str, CClientSocket* m_sClient);
};
