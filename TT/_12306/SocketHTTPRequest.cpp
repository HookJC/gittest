
#include <iostream>
#include "SocketHTTPRequest.h"
#include "typedef.h"

CSocketHTTPRequest::CSocketHTTPRequest()
{
	m_so = 0;
	memset(&m_addr, 0x0, sizeof(m_addr));
}

CSocketHTTPRequest::~CSocketHTTPRequest()
{
	close();
}

bool CSocketHTTPRequest::Init(const char* pszip, unsigned short usport)
{	
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = inet_addr(pszip);
	m_addr.sin_port = htons(usport);
	return true;
}

int CSocketHTTPRequest::reconn()
{
	m_so = socket(AF_INET, SOCK_STREAM, 0);
	int nNetTimeout = 10000; // 10秒，
	// 设置发送超时
	setsockopt(m_so, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	// 设置接收超时
	setsockopt(m_so, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
	
	if (m_so < 0)
	{
		return -1;
	}
	
	if (connect(m_so, (struct sockaddr* )&m_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		return -2;
	}
	
	return 0;
}

int CSocketHTTPRequest::open(const string& strip, unsigned short usport)
{		
	close();
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = inet_addr(strip.c_str());
	m_addr.sin_port = htons(usport);
	
	m_so = socket(AF_INET, SOCK_STREAM, 0);
	int nNetTimeout = 10000; // 10秒，
	// 设置发送超时
	//setsockopt(m_so, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	// 设置接收超时
	//setsockopt(m_so, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	if (m_so < 0)
	{
		return -1;
	}
	
	if (connect(m_so, (struct sockaddr* )&m_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		return -2;
	}
	
	return 0;
}

void CSocketHTTPRequest::close()
{
	if (m_so > 0)
	{
		//memset(&m_addr, 0x0, sizeof(m_addr));
#ifdef WIN32		
		closesocket(m_so);
#else if LINUX
		close(m_so);
#endif
		m_so = 0;
	}
}

int CSocketHTTPRequest::senddata(const char* pszdata, unsigned long ulsize)
{	
	if (m_so == 0 && reconn() != 0)
	{
		return 0;
	}
#if _DEBUG_
	std::cout << "Request:" << std::endl;
	std::cout << pszdata << std::endl;
#endif
	return send(m_so, pszdata, ulsize, 0);
}

int CSocketHTTPRequest::recvdata(string& strrecv)
{
	int iret = 0;
	char szrecv[4096 + 1] = {
		0
	};
	strrecv = "";

	while ((iret = recv(m_so, szrecv, 4096, 0)) > 0)
	{
		strrecv += szrecv;		
		memset(szrecv, 0x0, sizeof(szrecv));
	}
	
#if _DEBUG_
	std::cout << "Response:" << std::endl;
	std::cout << strrecv << std::endl;
#endif
		
	// close socket
	close();
	if (!strrecv.empty())
	{
		iret = 0;
	}
	return iret;
}

int CSocketHTTPRequest::recvchar(char* charreads, int iread)
{
	int icount = 0;
	int iret = 0;

	char* pszlen = NULL;
	int ilength = 0;
	int ibodylen = 0;
	int ichunklen = 0;
	bool bchunked = false;
	bool blength = false;
	
	char m_szbuf[MAX_BUFFER_BLOCK + 1] = {0};
	memset(m_szbuf, 0, sizeof(m_szbuf));
	while ((iret = recv(m_so, m_szbuf, MAX_BUFFER_BLOCK, 0)) > 0)
	{
		memcpy(charreads + icount, m_szbuf, iret);
		icount += iret;
		memset(m_szbuf, 0, iret);
		if (icount + MAX_BUFFER_BLOCK > iread)
		{
			break;
		}

		if (blength)
		{
			ibodylen += iret;
			if (ibodylen >= ilength)
			{
				break;
			}
		}		
		
		if (!bchunked && !blength)
		{
			if ((pszlen = strstr(charreads, "Content-Length: ")) != NULL)
			{
				blength = true;
				pszlen += strlen("Content-Length: ");
				ilength = atoi(pszlen);
				if (ilength == 0) //头响应包
				{
					break;
				}
				ibodylen = iret - (strstr(charreads, "\r\n\r\n") - charreads + 4);
				if (ibodylen >= ilength) // 首次完整包
				{
					break;
				}
			}
			else
			{
				bchunked = true;
			}
		}

		if (bchunked)
		{
			if (icount - 4 > 1 && strcmp(charreads + icount - 4 - 1, "0\r\n\r\n") == 0)
			{
				break;
			}
		}
	}

	close();

	return icount;
}

int CSocketHTTPRequest::sendandrecv(const string& strip, unsigned short usport, const string& strdata, string& strrecv)
{
	char szerror[ERROR_INFO_LEN + 1] = {0};	
	int iret = open(strip, usport);	
	
	if (iret < 0)
	{		
		sprintf(szerror, "open socket error. ret=%d", iret);
		strrecv = szerror;
		return -1;
	}
		
	iret = senddata(strdata.c_str(), strdata.size());
	if (iret < 0)
	{
		sprintf(szerror, "send data error. ret=%d", iret);
		strrecv = szerror;
		return -2;
	}
	
	string strRsp;
	iret = recvdata(strRsp);
	if (iret < 0)
	{
		sprintf(szerror, "recv data error. ret=%d", iret);
		strrecv = szerror;
		return -3;
	}
	
	strrecv = strRsp;	
	return 0;
}

string CSocketHTTPRequest::gethead(const string& strsrc)
{
	return strsrc.substr(0, strsrc.find("\r\n\r\n"));
}

string CSocketHTTPRequest::getbody(const string& strsrc)
{
	string strret;
	size_t ulpos = strsrc.find("\r\n\r\n");
	
	if (ulpos == string::npos)
	{
		return strret;
	}
	strret = strsrc.substr(ulpos + 4);
	
	string strlen;
	string strright = strret;
	strret = "";
	while ((ulpos = strright.find("\r\n")) != string::npos)
	{
		strlen = strright.substr(0, ulpos);		
		if (strlen.size() > 4)		
		{
			strret += strlen;
		}
		strright = strright.substr(ulpos + 2);
	}	
	strret += strright;
		
	return strret;
}

int CSocketHTTPRequest::get_chunked_data(const char* pszIn, unsigned int iIn, char* pszOut, unsigned int iOut)
{
	int icount = 0;
	char sztmp[10] = {0};
	char* pszbegin = strstr(pszIn, "\r\n\r\n");
	if (pszbegin == NULL)
	{
		return icount;
	}
	pszbegin += 4;

	char* pszlen = NULL;
	int itmp = 0;
	while ((pszlen = strstr(pszbegin, "\r\n")) != NULL)
	{
		itmp = 0;
		memset(sztmp, 0, sizeof(sztmp));
		memcpy(sztmp, pszbegin, pszlen - pszbegin);
		if (strcmp(sztmp, "") == 0)
		{
			pszbegin += 2;
			continue;
		}
		sscanf(sztmp, "%x", &itmp);
		if (itmp == 0)
		{
			break;
		}
		memcpy(pszOut + icount, pszlen + 2, min(iOut - icount, itmp));
		icount += itmp;

		pszbegin += strlen(sztmp);
		pszbegin += 2;
		pszbegin += itmp;
	}
	
	return icount;
}

