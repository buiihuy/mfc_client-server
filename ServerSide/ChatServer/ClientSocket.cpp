#include "pch.h"
#include "ClientSocket.h"
// ClientSocket.cpp : implementation file
//

//#include "stdafx.h"
#include "ChatServer.h"
#include "ClientSocket.h"

//#include "Userdata.h"

#include "ChatServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}



// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode)
{

	char buff[1000];

	int ibuf = Receive(buff, sizeof(buff));
	buff[ibuf] = '\0';
	if (buff[0] != ':' && buff[0] != '/') {
		strcpy_s((this->udata).cUserName, sizeof((this->udata).cUserName), buff);
		(this->udata).cUserName[strlen((this->udata).cUserName)] = '\0';
	}
	((CChatServerDlg*)pDlg)->AddMsg(buff, this);
	
	CSocket::OnReceive(nErrorCode);
}
