
#pragma warning(disable:4786)

#include "HttpSocket.h"
#ifdef WIN32
#include <winsock.h>
#endif

#include "function.h"

#define HTTP_VERSION_STR	"HTTP/1.1"
#define MAX_HTTPHEAD_LEN	2048

// request head key sort
const char* lpszhead[] = {
	"Accept",
	"Referer",
	"Accept-Language",
	"User-Agent",
	"Accept-Encoding",
	"Host",
	"Connection",
	"Cookie"
};

CHttpSocket::CHttpSocket()
{
	
}

CHttpSocket::CHttpSocket(const string& str)
{
	parse(str);
}

CHttpSocket::~CHttpSocket()
{
	m_maphead.clear();
}

string CHttpSocket::trimspace(string& str)
{
	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ") + 1);
	return str;	
}

// 
// HTTP/1.1 OK\r\n
// Content-Type: text/html;charset=gbk\r\n
// Content-Length: 18\r\n
// ......
// \r\n	
bool CHttpSocket::parse(const string& str)
{
	m_maphead.clear();	
	m_strrsphead = str.substr(0, str.find("\r\n\r\n"));

	if (m_strrsphead.size() > MAX_HTTPHEAD_LEN)
	{
		return false;
	}
	
	return (0 < insertmap(m_maphead, m_strrsphead, "\r\n", ": "));
}

// Content-Type: text/html
// key = "Content-Type"
// value = "text/html"
string CHttpSocket::getval(const string& strkey)
{
	tpfhttpmap::const_iterator it = m_maphead.find(strkey);
	if (it == m_maphead.end())
	{
		return "";
	}
	return it->second;
}

// SET-COOKIE: a=b;c=d;e=f
// key = "SET-COOKIE"
// valmap = {"a":"b", "c":"d", "e":"f" };
int CHttpSocket::getmap(const string& strkey, tpfhttpmap& valmap)
{
	valmap.clear();
	string strdata = getval(strkey);
	return insertmap(valmap, strdata, ";", "=");
}

// GET / HTTP/1.1\r\n
// Content-Type: text/html\r\n
// .......
// \r\n
string CHttpSocket::get_gethead(const string& strpath, const tpfhttpmap& headmap)
{
	string strdata = getstrfmap(headmap, "\r\n", ": ");

	m_strreqhead = "GET ";
	m_strreqhead += strpath;
	m_strreqhead += " ";
	m_strreqhead += HTTP_VERSION_STR;
	m_strreqhead += "\r\n";

	m_strreqhead = m_strreqhead + strdata;
	m_strreqhead += "\r\n";
	
	return m_strreqhead;
}

string CHttpSocket::get_gethead_sort(const string& strpath, const char** pszsortkey/* = NULL*/, int ilen/* = 0 */)
{	
	string strdata;
	const char** psort = pszsortkey;
	if (psort == NULL || ilen == 0)
	{
		psort = lpszhead;
		ilen = sizeof(lpszhead)	/ sizeof(char*);
	}
	for (int i = 0; i < ilen; ++i)
	{
		if (m_maphead.find(lpszhead[i]) == m_maphead.end())
		{
			continue;
		}
		strdata += lpszhead[i];
		strdata += ": ";
		strdata += m_maphead[lpszhead[i]];
		strdata += "\r\n";
	}	
	
	m_strreqhead = "GET ";
	m_strreqhead += strpath;
	m_strreqhead += " ";
	m_strreqhead += HTTP_VERSION_STR;
	m_strreqhead += "\r\n";

	strdata = m_strreqhead + strdata;
	strdata += "\r\n";
	
	return strdata;
}

// GET / HTTP/1.1\r\n
// Content-Type: text/html\r\n
// .......
// \r\n
string CHttpSocket::get_gethead(const string& strpath)
{
	string strdata = getstrfmap(m_maphead, "\r\n", ": ");

	m_strreqhead = "GET ";
	m_strreqhead += strpath;
	m_strreqhead += " ";
	m_strreqhead += HTTP_VERSION_STR;
	m_strreqhead += "\r\n";

	strdata = m_strreqhead + strdata;
	strdata += "\r\n";
	
	return strdata;
}

string CHttpSocket::get_posthead(const string& strpath, const string& strdata)
{
	string strpost = getstrfmap(m_maphead, "\r\n", ": ");
	
	m_strreqhead = "POST ";
	m_strreqhead += strpath;
	m_strreqhead += " ";
	m_strreqhead += HTTP_VERSION_STR;
	m_strreqhead += "\r\n";
	
	strpost = m_strreqhead + strpost;
	strpost += "Content-Length: ";
	strpost += i2str(strdata.size());
	strpost += "\r\n\r\n";
	strpost += strdata;
	
	return strpost;
}

// Set-Cookie: = Cookie:
string CHttpSocket::get_replace(const string& stroldkey, const string& strnewkey)
{
	tpfhttpmap::const_iterator it = m_maphead.find(stroldkey);
	if (it == m_maphead.end())
	{
		return "";
	}
	m_maphead[strnewkey] = it->second;
	m_maphead.erase(it->first);
	return m_maphead[strnewkey];	
}

// Server:
string CHttpSocket::remove_key(const string& strkey)
{
	tpfhttpmap::const_iterator it = m_maphead.find(strkey);
	if (it == m_maphead.end())
	{
		return "";
	}
	string str = m_maphead[strkey];
	m_maphead.erase(strkey);
	return str;
}

// Connection: close = Connection: keep-alive
string CHttpSocket::setvale(const string& strkey, const string& strval)
{
	tpfhttpmap::const_iterator it = m_maphead.find(strkey);
	if (it == m_maphead.end())
	{
		return "";
	}
	string str = m_maphead[strkey];
	m_maphead[strkey] = strval;
	return str;
}

// add one key: val
string CHttpSocket::addmap(const string& strkey, const string& strval)
{
	tpfhttpmap::const_iterator it = m_maphead.find(strkey);
	if (it == m_maphead.end())
	{
		m_maphead[strkey] = strval;
		return strval;
	}
	string str = m_maphead[strkey];
	m_maphead[strkey] = strval;
	return str;
}
		
// valmap = {"a":"b", "c":"d", "e":"f" };
// a=b;c=d;e=f
string CHttpSocket::getstr(const tpfhttpmap& strmap)
{
	return getstrfmap(strmap, ";", "=");
}


// strrowsplit = "\r\n"
// strmidsplit = ": "
int CHttpSocket::insertmap(tpfhttpmap& mapstr, const string& str, const string& strrowsplit, const string& strmidsplit)
{
	if (str.empty())
	{
		return 0;
	}

	int iret = 0;
	size_t ulrpos = 0;
	size_t ulmpos = 0;
	string strrow;
	string strkey;
	string strval;
	string strdata = str;
	
	while (!strdata.empty())
	{
		if ((ulrpos = strdata.find(strrowsplit)) != string::npos)
		{
			strrow = strdata.substr(0, ulrpos);
			strdata = strdata.substr(ulrpos + strrowsplit.size());
		}
		else
		{
			strrow = strdata;
			strdata = "";
		}
		
		if ((ulmpos = strrow.find(strmidsplit)) != string::npos)
		{
			strkey = strrow.substr(0, ulmpos);
			strval = strrow.substr(ulmpos + strmidsplit.size());
			if (strkey.empty() || strval.empty())
			{
				continue;
			}
			trimspace(strkey);
			if (mapstr.find(strkey) != mapstr.end())
			{
				mapstr[strkey] += ";"; 
				mapstr[strkey] += strval;
			}
			else
			{
				mapstr[strkey] = strval;
			}			
			++iret;
		}		
	}

	return iret;
}

// get string from map as special format
string CHttpSocket::getstrfmap(const tpfhttpmap& mapstr, const string& strrowsplit, const string& strmidsplit)
{
	if (mapstr.empty())
	{
		return "";
	}

	string strret;
	tpfhttpmap::const_iterator it = mapstr.begin();
	while (it != mapstr.end())
	{
		strret += it->first;
		strret += strmidsplit;
		strret += it->second;
		strret += strrowsplit;
		++it;
	}
	return strret;
}

//HTTP/1.1 OK 200
string CHttpSocket::get_rsphead()
{
	return m_strrsphead;
}

//GET / HTTP/1.1
string CHttpSocket::get_reqhead()
{
	return m_strreqhead;
}

string CHttpSocket::get_hostip()
{
	string strip;
#ifdef WIN32
	char szip[16] = {0};
	HOSTENT* host;
	host = ::gethostbyname(getval("Host").c_str());
	if (host)
	{
		char **p = host->h_addr_list;
		while (*p)
		{
			strip = inet_ntoa(*(struct in_addr*)*p);
			if (!strip.empty())
			{
				break;
			}
			++p;
		}
	}	
#endif

	return strip;
}

string CHttpSocket::getbody(const string& strsrc)
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

int CHttpSocket::get_chunked_data(const char* pszIn, unsigned int iIn, char* pszOut, unsigned int iOut)
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


































