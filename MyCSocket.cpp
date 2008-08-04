
#include "MyCSocket.h"

/**
 * @param iPort local listenning port
 * @throws MyCSocketException is server socket could not be created
 */
MyCSocket::MyCSocket( int iPort)
{
	Reset( iPort);

	if( bind( m_Socket, ( SOCKADDR *)&m_sockaddr, sizeof( sockaddr)) != 0)
		throw MyCSocketException( "bind() failed");

	if( listen( m_Socket, 0) != 0)
		throw MyCSocketException( "accept() failed");
}

/**
 * @param szRemoteAddr Remote Machine Address
 * @param iRemotePort Server Listenning Port
 * @throws MyCSocketException if client socket could not be created
 */
MyCSocket::MyCSocket( char *szRemoteAddr, int iPort)
{
	if( !szRemoteAddr)
		throw MyCSocketException( "Invalid parameters");

	Reset( iPort);

	// first guess => try to resolve it as IP@
	m_sockaddr.sin_addr.s_addr = inet_addr( szRemoteAddr);
	if( m_sockaddr.sin_addr.s_addr == INADDR_NONE)
	{	// screwed => try to resolve it as name
	LPHOSTENT lpHost = gethostbyname( szRemoteAddr);
		if( !lpHost)
			throw MyCSocketException( "Unable to solve this address");
		m_sockaddr.sin_addr.s_addr = **(int**)(lpHost->h_addr_list);
	}

	// actually performs connection
	if( connect( m_Socket, ( SOCKADDR*)&m_sockaddr, sizeof( sockaddr)) != 0)
		throw MyCSocketException( "connect() failed");
}

/**
 * Create a socket for data transfer (typically after Accept)
 * @param Socket the socket descriptor for this new object
 */
MyCSocket::MyCSocket( SOCKET Socket)
{
	m_Socket = Socket;
}

/**
 * Destructor
 */
MyCSocket::~MyCSocket()
{
	Close();
}

/**
 * Wait for incomming connections on server socket
 * @return MyCSocket new data socket for this incomming client. Can be NULL is anything went wrong
 */
MyCSocket * MyCSocket::Accept()
{
int nlen = sizeof( sockaddr);
SOCKET Socket = accept( m_Socket, ( SOCKADDR *)&m_sockaddr, &nlen);

	if( Socket == -1)
		return( NULL);

	return( new MyCSocket( Socket));
}


/**
 * Close current socket
 */
void MyCSocket::Close()
{
	if( m_Socket != INVALID_SOCKET)
		closesocket( m_Socket);
}

/**
 * Read data available in socket or waits for incomming informations
 * @param pData Buffer where informations will be stored
 * @param iLen Max length of incomming data
 * @读取指定长度的数据包
 * @return Number of bytes read or -1 if anything went wrong
 */
int MyCSocket::Read( void * pData, unsigned int iLen)
{
	if( !pData || !iLen)
		return( -1);

	return( recv( m_Socket, ( char *)pData, iLen, 0));
}

/**
 * Initialisation common to all constructors
 */
void MyCSocket::Reset( unsigned int iPort)
{
	// Initialize winsock
	if( WSAStartup( MAKEWORD(2,0), &m_WSAData) != 0)
		throw MyCSocketException( "WSAStartup() failed");

	// Actually create the socket
	m_Socket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( m_Socket == INVALID_SOCKET)
		throw MyCSocketException( "socket() failed");

	// sockaddr initialisation
	memset( &m_sockaddr, 0, sizeof( sockaddr));

	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons( iPort);
	m_sockaddr.sin_addr.s_addr = INADDR_ANY;
}

/**
 * @param pData Buffer to be sent
 * @param iSize Number of bytes to be sent from buffer
 * @return the number of sent bytes or -1 if anything went wrong
 */
int MyCSocket::Write( void * pData, unsigned int iSize)
{
	if( !pData || !iSize)
		return( -1);

	return( ( int)send( m_Socket, ( LPCSTR)pData, iSize, 0));
}