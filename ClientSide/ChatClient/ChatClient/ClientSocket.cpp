#include "pch.h"
#include "ClientSocket.h"
#include "Data.h"
#include "ChatClientDlg.h"
CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class

	char buff[1000];
	int br = Receive(buff, sizeof(buff));
	buff[br] = '\0';


	/*userinfo* udata;
	udata = (userinfo*)buff;*/
	((CChatClientDlg*)pDlg)->AddMsg(buff);

	CSocket::OnReceive(nErrorCode);
}