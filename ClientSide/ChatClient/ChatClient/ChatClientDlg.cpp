
// ChatClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"
#include<iostream>
#include<string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatClientDlg dialog



CChatClientDlg::CChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATCLIENT_DIALOG, pParent)
	, m_sUserName(_T(""))
	, m_sMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Display);
	DDX_Text(pDX, IDC_EDIT1, m_sUserName);
	DDX_Text(pDX, IDC_EDIT2, m_sMsg);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CChatClientDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CChatClientDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatClientDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CChatClientDlg message handlers

BOOL CChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatClientDlg::OnBnClickedButtonConnect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_sUserName.GetLength() == 0) {
		MessageBox(_T("The username is empty"));
		return;
	}

	if (m_sUserName.GetLength() > 20) {
		MessageBox(_T("The username is too long!"));
		return;
	}

	if (m_sUserName[0] == ':' || m_sUserName[0] == '/') {
		MessageBox(_T("You can not set the first character of username is ':' or '/'!"));
		return;
	}

	if (AfxSocketInit() == FALSE)
	{
		AfxMessageBox(_T("Failed to Initialize Sockets"));
		return;
	}

	if (m_Client.Create() == FALSE)
	{
		MessageBox(_T("Failed to Create Socket"));
		return;

	}
	if (m_Client.Connect(_T("127.0.0.1"), 1001) == FALSE)
	{
		MessageBox(_T("Failed to Connect"));
		return;

	}
	m_Client.pDlg = this;
	
	CStringA tmp(m_sUserName);
	const char* msg = tmp;
	m_Client.Send(msg, strlen(msg));

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
}


void CChatClientDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	char msg[20] = "";
	snprintf(msg, sizeof(msg), "/STOP");
	m_Client.Send(msg, strlen(msg));

	m_Client.ShutDown(2);
	m_Client.Close();

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
}


void CChatClientDlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_sMsg == ":HELP") {
		m_Display.AddString(_T(">>NICK:  <:>NICK <SPACES> <NICKNAME>"));
		m_Display.AddString(_T(">>QUIT:  <:>QUIT"));
		m_Display.AddString(_T(">>JOIN:  <:>JOIN <SPACES> <CHANNEL_NAME>"));
		m_Display.AddString(_T(">>PART:  <:>PART"));
		m_Display.AddString(_T(">>LIST:  <:>LIST"));
		m_Display.AddString(_T(">>PRIVMSG:  <:>PRIVMSG <SPACES> @<USERNAME> <SPACES> <MESSAGES>"));
		m_Display.AddString(_T("            <:>PRIVMSG <SPACES> #<CHANNEL_NAME> <SPACES> <MESSAGES>"));
		m_Display.AddString(_T(">>WHO:  <:>WHO <SPACES> @<USERNAME>"));
		m_Display.AddString(_T("        <:>WHO <SPACES> #<CHANNEL_NAME>"));
		m_Display.AddString(_T(">>HELP:  <:>HELP"));
	}
	else {
		if (m_sMsg[0] != ':') {
			MessageBox(_T("Error Syntax"));
			return;
		}
		CStringA tmp(m_sMsg);
		const char* msg = tmp;
		if (strlen(msg) > 1000) {
			MessageBox(_T("Too long message!!!"));
			return;
		}
		m_Client.Send(msg, strlen(msg));
	}
}

void CChatClientDlg::AddMsg(char* buff, CClientSocket* m_client)
{
	if (!strcmp(buff, "/QUIT")) {
		CChatClientDlg::OnBnClickedButtonStop();
	}
	else if (!strcmp(buff, "The username is unavailable")) {
		CString mes(buff);
		MessageBox(mes);
		m_sUserName.Empty();
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		m_client->Close();
	}
	else if (!strcmp(buff, "Syntax Error!")) {
		CString mes(buff);
		MessageBox(mes);
	}
	else {
		CString display(buff);
		m_Display.AddString(display);
	}
}
