#ifndef __HTTP_SOCKET__
#define __HTTP_SOCKET__

#include <map>
#include <string>

using std::string;
using std::map;

typedef map<string, string> tpfhttpmap;

class CHttpSocket
{
public:
	CHttpSocket();
	CHttpSocket(const string& str);
	~CHttpSocket();
	string trimspace(string& str);

	static string getbody(const string& strsrc); // 支持chunked段数据 | 检测编码，负责转码
	static int get_chunked_data(const char* pszIn, unsigned int iIn, char* pszOut, unsigned int iOut);

public:
	// 
	// HTTP/1.1 OK\r\n
	// Content-Type: text/html;charset=gbk\r\n
	// Content-Length: 18\r\n
	// ......
	// \r\n	
	bool parse(const string& str);

	// Content-Type: text/html
	// key = "Content-Type"
	// value = "text/html"
	string getval(const string& strkey);

	// SET-COOKIE: a=b;c=d;e=f
	// key = "SET-COOKIE"
	// valmap = {"a":"b", "c":"d", "e":"f" };
	int getmap(const string& strkey, tpfhttpmap& valmap);

	// GET / HTTP/1.1\r\n
	// Content-Type: text/html\r\n
	// .......
	// \r\n
	string get_gethead(const string& strpath, const tpfhttpmap& headmap);
	
	// GET / HTTP/1.1\r\n
	// Content-Type: text/html\r\n
	// .......
	// \r\n
	string get_gethead_sort(const string& strpath, const char** pszsortkey = NULL, int ilen = 0);
	
	// GET / HTTP/1.1\r\n
	// Content-Type: text/html\r\n
	// .......
	// \r\n
	string get_gethead(const string& strpath);

	// POST / HTTP/1.1\r\n
	// Content-Type: text/html\r\n
	// .......
	// \r\n
	string get_posthead(const string& strpath, const string& strdata);
	
	// Set-Cookie: = Cookie:
	string get_replace(const string& stroldkey, const string& strnewkey);
	
	// Connection: close = Connection: keep-alive
	string setvale(const string& strkey, const string& strval);
	
	// add one key: val
	string addmap(const string& strkey, const string& strval);
	
	// Server:
	string remove_key(const string& strkey);

	// valmap = {"a":"b", "c":"d", "e":"f" };
	// a=b;c=d;e=f
	string getstr(const tpfhttpmap& strmap);

	string get_rsphead();
	string get_reqhead();

	string get_hostip();

private:
	// strrowsplit = "\r\n"
	// strmidsplit = ": "
	int insertmap(tpfhttpmap& mapstr, const string& str, const string& strrowsplit, const string& strmidsplit);

	// 
	string getstrfmap(const tpfhttpmap& mapstr, const string& strrowsplit, const string& strmidsplit);

private:
	tpfhttpmap m_maphead;

	string m_strreqhead;
	string m_strrsphead;


};





#endif
