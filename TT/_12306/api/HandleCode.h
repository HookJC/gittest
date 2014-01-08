
#ifndef __HANDLE_CODE_H__
#define __HANDLE_CODE_H__

#include <string>

using std::string;

#ifdef WIN32
#include <cstring>
#include <windows.h>
#endif


#include <string>
using namespace std;

class CHandleCode
{
public:
	CHandleCode(){};
	~CHandleCode(){};
	
public:

	// gbk convert to utf8
	static string GBKToUTF8(const string& str);
	// utf8 convert to gbk
	static string UTF8ToGBK(string& str);	
	
	static int UTF8ToGBK(const char* instr, int inlen, char* outstr, int outlen);

	// gb2312 convert to utf8
	/*static string GB2312ToUTF8(string& pOut, const char *pText, int pLen);
	// utf8 convert to gb2312
	static string UTF8ToGB2312(string& str, const char *pText, int pLen);*/

	// base64 decode
	static int base64decode(char* pOut, const string& strIn);
	// base64 encode
	static string base64encode(string& strOut, const char* pszIn, int iIn);
	

	// url decode %20 = " "
	static string UrlDecode(string& str);
	// url encode " " = %20
	static string UrlEncode(const string& str);


private:
	
	static void UTF8ToUnicode(wchar_t* pOut,char *pText);
	
	static void UnicodeToUTF8(char* pOut,wchar_t* pText);
	
	static void UnicodeToGB2312(char* pOut,wchar_t uData);
	
	static void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer);







































};


#endif


