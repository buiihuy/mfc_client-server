#include "pch.h"
#include "ServerSocket.h"
//#include "stdafx.h"
#include "ChatServer.h"
#include "ServerSocket.h"
#include "ChatServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions

void CServerSocket::OnAccept(int nErrorCode)
{
    if (nErrorCode == 0) {
        // Successfully accepted an incoming connection

        // Create a new socket for the accepted client
        CClientSocket* pClientSocket = new CClientSocket;

        if (Accept(*pClientSocket)) {
            pClientSocket->pDlg = this->pDlg;
            
            // Handle the new client connection here.
            //AfxMessageBox(_T("Client Request Connection"));
            // add pClientSocket to m_sClients to manage multiple clients
            m_sClients.push_back(pClientSocket);
            (pClientSocket->udata).in_channel = false;
            pClientSocket->pDlg->UpdateData(TRUE);
        }
        else {
            // Failed to accept the client connection, so clean up the socket.
            delete pClientSocket;
        }
    }
    CSocket::OnAccept(nErrorCode);

}
