// ChatServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "afxdialogex.h"
#include "Data.h"
#include<string>
#include<vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _CRT_SECURE_NO_WARNINGS

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


// CChatServerDlg dialog



CChatServerDlg::CChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Display);
	//DDX_Text(pDX, IDC_EDIT1, m_Msg);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatServerDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_START, &CChatServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CChatServerDlg::OnBnClickedButtonStop)
	ON_LBN_SELCHANGE(IDC_LIST1, &CChatServerDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CChatServerDlg message handlers

BOOL CChatServerDlg::OnInitDialog()
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

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatServerDlg::OnPaint()
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
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatServerDlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
}



void CChatServerDlg::AddMsg(char* str, CClientSocket* m_sClient)
{
	char msg[100] = "";
	if (str[0] == ':') {
		char tmp[20];
		int i = 0;
		while (str[i] != ' ' && str[i] != '\0') {
			//strcat_s(tmp, strlen(tmp), str + i);
			tmp[i] = str[i];
			i++;
		}
		tmp[i] = '\0';
		if (!strcmp(tmp, ":NICK")) NICK(str, m_sClient);
		else if (!strcmp(tmp, ":QUIT")) QUIT(str, m_sClient);
		else if (!strcmp(tmp, ":JOIN")) JOIN(str, m_sClient);
		else if (!strcmp(tmp, ":PART")) PART(m_sClient);
		else if (!strcmp(tmp, ":LIST")) LIST(str, m_sClient);
		else if (!strcmp(tmp, ":PRIVMSG")) PRIVMSG(str, m_sClient);
		else if (!strcmp(tmp, ":WHO")) WHO(str, m_sClient);
		else {
			snprintf(msg, sizeof(msg), ">>>ERROR_UNKNOWNCOMMAND");
			m_sClient->Send(msg, strlen(msg));
		}
	}
	else if (!strcmp(str, "/STOP")) {
		//m_sClient->Close();
		for (int i = 0; i < m_sServer.m_sClients.size(); i++) {
			if (m_sServer.m_sClients.at(i) == m_sClient) {
				m_sServer.m_sClients.erase(m_sServer.m_sClients.begin() + i);
			}
		}
	}
	else {
		for (int i = 0; i < m_sServer.m_sClients.size(); i++) {
			if (!strcmp(m_sServer.m_sClients.at(i)->udata.cUserName, str) && m_sServer.m_sClients.at(i) != m_sClient) {
				snprintf(msg, sizeof(msg), "The username is unavailable");
				m_sClient->Send(msg, strlen(msg));
				m_sClient->Close();
				return;
			}
		}
		CString str_display;
		str_display.Format(_T("%hs join server"), str);
		m_Display.AddString(str_display);
	}
}


void CChatServerDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	if (AfxSocketInit() == FALSE)
	{
		AfxMessageBox(_T("Failed to Initialize Sockets"));
		return;
	}

	//m_Server.m_Client.pDlg = this;
	m_sServer.pDlg = this;

	if (m_sServer.Create(1001) == FALSE)
	{
		MessageBox(_T("Failed to create"));
		return;
	}
	m_Display.AddString(_T("Initialize socket successfully"));
	m_Display.AddString(_T("Creat port 1001 successfully"));

	if (m_sServer.Listen(5) == FALSE)
	{
		MessageBox(_T("Failed"));
		return;
	}
	m_Display.AddString(_T("Server is listening"));


	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}


void CChatServerDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	size_t size = m_sServer.m_sClients.size();
	for (int i = 0; i < size; i++) {
		m_sServer.m_sClients.at(i)->ShutDown(2);
	}

	m_sServer.Close();

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
}


void CChatServerDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}


void CChatServerDlg::NICK(char* str, CClientSocket* m_sClient) {
	int i = 6;
	char msg[30] = "";
	char nickname[20];

	if (str[5] == '\0') {
		snprintf(msg, sizeof(msg), "Syntax Error!");
		m_sClient->Send(msg, strlen(msg));
		return;
	}

	while (str[i] != '\0') {
		nickname[i-6] = str[i];
		i++;
	}
	nickname[i - 6] = '\0';
	if (strcmp(nickname, "\0")) {
		for (int i = 0; i < m_sServer.m_sClients.size(); i++) {

			if (m_sServer.m_sClients[i] != m_sClient && !strcmp((m_sServer.m_sClients[i]->udata).cNickName, nickname)) {

				snprintf(msg, sizeof(msg), "This nickname is available!");
				m_sClient->Send(msg, strlen(msg));

				return;
			}
		}
		strcpy_s((m_sClient->udata).cNickName, sizeof((m_sClient->udata).cNickName), nickname);
		snprintf(msg, sizeof(msg), "You set nickname successfully!");
		m_sClient->Send(msg, strlen(msg));
	}
	else {
		snprintf(msg, sizeof(msg), "Server : Empty Nickname");
		m_sClient->Send(msg, strlen(msg));
	}
}


void CChatServerDlg::QUIT(char* str, CClientSocket* m_sClient) {

	char msg[50] = "";

	for (int i = 0; i < channels.size(); i++) {

		for (int j = 0; j < channels[i]->channelClient.size(); j++) {

			if (channels[i]->channelClient.at(j) == m_sClient) {

				channels[i]->channelClient.erase(channels[i]->channelClient.begin() + j);
				m_sClient->udata.in_channel = false;

				snprintf(msg, sizeof(msg), "You leave channel %s successfully", channels[i]->channelName);
				m_sClient->Send(msg, strlen(msg));

				snprintf(msg, sizeof(msg), "%s leave channel", m_sClient->udata.cUserName);
				for (int k = 0; k < channels[i]->channelClient.size(); k++) {
					channels[i]->channelClient.at(k)->Send(msg, strlen(msg));
				}
				break;
			}
		}
	}
	snprintf(msg, sizeof(msg), "/QUIT");
	m_sClient->Send(msg, strlen(msg));
}


void CChatServerDlg::JOIN(char* str, CClientSocket* m_sClient) {
	char cname[20];
	char msg[100] = "";
	int i = 6;

	if (str[5] == '\0') {
		snprintf(msg, sizeof(msg), "Syntax Error!");
		m_sClient->Send(msg, strlen(msg));
		return;
	}

	while (str[i] != '\0') {
		cname[i-6] = str[i];
		i++;
	}
	cname[i - 6] = '\0';

	if (strcmp(cname, "\0")) {
		if (m_sClient->udata.in_channel == false) {

			for (int i = 0; i < channels.size(); i++) {

				if (!strcmp(channels[i]->channelName, cname)) {

					snprintf(msg, sizeof(msg), "You join channel %s successfully!", cname);
					m_sClient->Send(msg, strlen(msg));

					snprintf(msg, sizeof(msg), "%s join channel %s", m_sClient->udata.cUserName, cname);

					for (int k = 0; k < channels[i]->channelClient.size(); k++) {
						if (strcmp(channels[i]->channelClient.at(k)->udata.cUserName, m_sClient->udata.cUserName)) {
							channels[i]->channelClient.at(k)->Send(msg, strlen(msg));
						}
					}

					channels[i]->channelClient.push_back(m_sClient);
					m_sClient->udata.in_channel = true;
					return;
				}
			}

			channel* newchannel = new channel;
			strcpy_s(newchannel->channelName, sizeof(newchannel->channelName), cname);
			newchannel->channelClient.push_back(m_sClient);
			channels.push_back(newchannel);
			m_sClient->udata.in_channel = true;
			//msg = "you join channel successfully";
			snprintf(msg, sizeof(msg), "You join channel %s successfully!", cname);
			m_sClient->Send(msg, strlen(msg));
		}
		else {
			snprintf(msg, sizeof(msg), "You join channel failed!");
			m_sClient->Send(msg, strlen(msg));
		}
	}
	else {
		snprintf(msg, sizeof(msg), "Empty channel name!");
		m_sClient->Send(msg, strlen(msg));
	}
}


void CChatServerDlg::PART(CClientSocket* m_sClient) {
	char msg[100] = "";

	for (int i = 0; i < channels.size(); i++) {

		for (int j = 0; j < channels[i]->channelClient.size(); j++) {

			if (channels[i]->channelClient.at(j) == m_sClient) {

				channels[i]->channelClient.erase(channels[i]->channelClient.begin() + j);
				m_sClient->udata.in_channel = false;

				snprintf(msg, sizeof(msg), "You leave channel %s successfully!", channels[i]->channelName);
				m_sClient->Send(msg, strlen(msg));

				snprintf(msg, sizeof(msg), "%s leave channel!", m_sClient->udata.cUserName);
				for (int k = 0; k < channels[i]->channelClient.size(); k++) {
					channels[i]->channelClient.at(k)->Send(msg, strlen(msg));
				}
				return;
			}
		}
	}
	snprintf(msg, sizeof(msg), "You are not in any channel!");
	m_sClient->Send(msg, strlen(msg));
}


void CChatServerDlg::LIST(char* str, CClientSocket* m_sClient) {
	CString msg;

	for (int i = 0; i < channels.size(); i++) {
		msg.Format(_T("%hs : %zu"), channels[i]->channelName, channels[i]->channelClient.size());
		CStringA tmp(msg);
		const char* new_msg = tmp;
		m_sClient->Send(new_msg, strlen(new_msg));
	}
}
 

void CChatServerDlg::PRIVMSG(char* str, CClientSocket* m_sClient) {

	char msg[1000] = "";

	if (str[8] == '\0') {
		snprintf(msg, sizeof(msg), "Syntax Error!");
		m_sClient->Send(msg, strlen(msg));
		return;
	}

	// inbox to an user
	if (str[9] == '@') {
		char receiver[20]; // store the name of the received client
		char text[1000]; // store the message
		int i = 10;
		int k = 0;

		while (str[i] != ' ' && str[i] != '\0') {
			receiver[i - 10] = str[i];
			i++;
		}
		receiver[i - 10] = '\0';

		if (str[i] != '\0') {
			i++;
			while (str[i] != '\0') {
				text[k] = str[i];
				i++;
				k++;
			}
			text[k] = '\0';
		}
		else text[k] = '\0';

		if (strcmp(text, "\0") && strcmp(receiver, "\0")) {
			for (int j = 0; j < m_sServer.m_sClients.size(); j++) {

				if (!strcmp((m_sServer.m_sClients.at(j)->udata).cUserName, receiver)) {

					snprintf(msg, sizeof(msg), "%s : %s", (m_sClient->udata).cUserName, text);
					m_sServer.m_sClients.at(j)->Send(msg, strlen(msg));

					return;
				}
			}
			snprintf(msg, sizeof(msg), "Server : User not found");
			m_sClient->Send(msg, strlen(msg));
		}
		else {
			snprintf(msg, sizeof(msg), "Server : Empty message or received user name!");
			m_sClient->Send(msg, strlen(msg));
		}
	}

	// inbox to channel
	else if (str[9] == '#') {

		if (!m_sClient->udata.in_channel) {
			snprintf(msg, sizeof(msg), "You are not in any channel!");
			m_sClient->Send(msg, strlen(msg));
			return;
		}

		char channelReceiver[20]; // store the channel name
		char text[1000]; // store the message
		int i = 10;
		int m = 0;

		while (str[i] != ' ' && str[i] != '\0') {
			channelReceiver[i-10] = str[i];
			i++;
		}
		channelReceiver[i - 10] = '\0';

		if (str[i] != '\0') {
			i++;
			while (str[i] != '\0') {
				text[m] = str[i];
				i++;
				m++;
			}
			text[m] = '\0';
		}
		else text[m] = '\0';

		if (strcmp(text, "\0") && strcmp(channelReceiver, "\0")) {
			for (int j = 0; j < channels.size(); j++) {
				if (!strcmp(channels[j]->channelName, channelReceiver)) {

					snprintf(msg, sizeof(msg), "%s > %s : %s", (m_sClient->udata).cUserName, channels[j]->channelName, text);

					for (int k = 0; k < channels[j]->channelClient.size(); k++) {

						if (strcmp((channels[j]->channelClient.at(k))->udata.cUserName, (m_sClient->udata).cUserName)) {
							(channels[j]->channelClient.at(k))->Send(msg, strlen(msg));
						}
					}
					return;
				}
			}
			snprintf(msg, sizeof(msg), "Server : Channel not found"); 
			m_sClient->Send(msg, strlen(msg));
		}
		else {
			snprintf(msg, sizeof(msg), "Server : Empty message or channel name!");
			m_sClient->Send(msg, strlen(msg));
		}
	}

	// other
	else {
		snprintf(msg, sizeof(msg), "Syntax Error!");
		m_sClient->Send(msg, strlen(msg));
	}
}


void CChatServerDlg::WHO(char* str, CClientSocket* m_sClient) {
	char msg[100] = "";

	if (str[4] == '\0') {
		snprintf(msg, sizeof(msg), "Syntax Error!");
		m_sClient->Send(msg, strlen(msg));
		return;
	}

	if (str[5] == '@') {
		//char msg[100] = "";
		char uname[20];
		int i = 6;

		while (str[i] != '\0' && (i - 6) < sizeof(uname) - 1) {
			uname[i - 6] = str[i];
			i++;
		}
		uname[i - 6] = '\0';

		if (strcmp(uname, "\0")) {
			for (int j = 0; j < m_sServer.m_sClients.size(); j++) {
				if (!strcmp((m_sServer.m_sClients.at(j)->udata).cUserName, uname)) {

					snprintf(msg, sizeof(msg), "User name : %s", (m_sServer.m_sClients.at(j)->udata).cUserName);
					m_sClient->Send(msg, strlen(msg));

					snprintf(msg, sizeof(msg), "   Nick name : %s", (m_sServer.m_sClients.at(j)->udata).cNickName);
					m_sClient->Send(msg, strlen(msg));

					break;
				}
			}
		}
		else {
			snprintf(msg, sizeof(msg), "Server : Empty user name");
			m_sClient->Send(msg, strlen(msg));
		}
	}

	else if (str[5] == '#') {
		//char msg[100] = "";
		char cname[20];
		int i = 6;

		while (str[i] != '\0') {
			cname[i-6] = str[i];
			i++;
		}
		cname[i - 6] = '\0';

		if (strcmp(cname, "\0")) {
			for (int j = 0; j < channels.size(); j++) {
				if (!strcmp(channels[j]->channelName, cname)) {

					snprintf(msg, sizeof(msg), "Channel : %s", channels[j]->channelName);
					m_sClient->Send(msg, strlen(msg));

					for (int k = 0; k < channels[j]->channelClient.size(); k++) {
						CClientSocket* pSocket = channels[j]->channelClient.at(k);

						snprintf(msg, sizeof(msg), "   >>> User name : %s", pSocket->udata.cUserName);
						m_sClient->Send(msg, strlen(msg));

						snprintf(msg, sizeof(msg), "   Nick name : %s", pSocket->udata.cNickName);
						m_sClient->Send(msg, strlen(msg));
					}
					break;
				}
			}
		}
		else {
			snprintf(msg, sizeof(msg), "Server : Empty channel name");
			m_sClient->Send(msg, strlen(msg));
		}
	}

	else {
		snprintf(msg, sizeof(msg), "Syntax Error!");
		m_sClient->Send(msg, strlen(msg));
	}
}