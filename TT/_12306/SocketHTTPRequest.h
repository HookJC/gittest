
/************************************************************************
* base http request include socket,send,recv function
* for get and post http method
* 1. 
* 2. 
* 3. 
* 4. 
* 5. 
* 6. 
* 7. 
************************************************************************/
#ifndef __SOCKET_HTTP_REQUEST_H__
#define __SOCKET_HTTP_REQUEST_H__

#include <string>
#ifdef WIN32
#include <winsock.h>
#else if LINUX
#include <sys/socket.h>
#endif

using std::string;

#ifndef MAX_BUFFER_BLOCK
#define MAX_BUFFER_BLOCK 4096
#endif
/************************************************************************/
// 短连接，需要支持多线程

/************************************************************************/
class CSocketHTTPRequest
{
public:
	CSocketHTTPRequest();
	virtual ~CSocketHTTPRequest();

	bool Init(const char* pszip, unsigned short usport);
	int reconn();

	static string gethead(const string& strsrc);
	static string getbody(const string& strsrc);
	static int get_chunked_data(const char* pszIn, unsigned int iIn, char* pszOut, unsigned int iOut);
	
	/************************************************************************/
	// 短连接，支持多线程
	SOCKET open_socket(const string& strip, unsigned short usport);
	int send_socket_data(const char* pszdata, unsigned long ulsize, SOCKET so);
	int recv_socket_data(char* charreads, int iread, SOCKET so);
	/************************************************************************/

protected:	
	int sendandrecv(const string& strip, unsigned short usport, const string& strdata, string& strrecv);

	int open(const string& strip, unsigned short usport);
	int senddata(const char* pszdata, unsigned long ulsize);

	int recvdata(string& strrecv);
	int recvchar(char* charreads, int iread);

	


	void close();

protected:
	SOCKET m_so;

private:	
	struct sockaddr_in m_addr;
};

#endif


