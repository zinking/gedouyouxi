#ifndef __MyCSocket_H
#define __MyCSocket_H

#include "stdio.h"
#include <Winsock2.h>
//#include "Toolkit.h"

#pragma   comment(lib,"wsock32.lib")

class MyCSocket
{
public:
	MyCSocket( char *szRemoteAddr, int iPort);
	MyCSocket( int iPort);
	MyCSocket( SOCKET Socket);

	~MyCSocket();

	MyCSocket * Accept( void);
	void Close( void);
	int Read( void * pData, unsigned int iLen);
	int Write( void * pData, unsigned int iLen);

private:
	SOCKET m_Socket;
	WSADATA m_WSAData;
	SOCKADDR_IN m_sockaddr;

	void Reset( unsigned int iPort);
};

class MyCSocketException
{
public:
	MyCSocketException( char * szText)
	{
		strcpy( m_szText, szText);
	}

	~MyCSocketException(){};

	char * getText(){ return( m_szText);}

private:
	char m_szText[ 128];
};

#endif //__MyCSocket_H