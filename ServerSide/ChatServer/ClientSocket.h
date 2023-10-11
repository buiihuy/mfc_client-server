#pragma once
#include "Afxsock.h" 
#include "Data.h"
#include <string>
#include<iostream>

class CClientSocket : public CSocket
{
	// Attributes
public:
	userinfo udata;

public:
	CClientSocket();
	~CClientSocket();
	CClientSocket::CClientSocket(const CClientSocket&) = delete;
	CClientSocket& operator=(const CClientSocket&) = delete;

	// Overrides
public:
	CDialog* pDlg;

public:
	virtual void OnReceive(int nErrorCode);

// Implementation
protected:
};
