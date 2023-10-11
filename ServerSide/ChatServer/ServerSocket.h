#pragma once
#include "ClientSocket.h"
#include "Data.h"
#include<vector>
class CServerSocket : public CSocket
{
public:
	CServerSocket();
	~CServerSocket();
public:

	CDialog* pDlg;

	//vector CClientSocket
	std::vector<CClientSocket*> m_sClients;
	
public:
	virtual void OnAccept(int nErrorCode);
};

