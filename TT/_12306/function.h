
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include "typedef.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "md5.h"
#include <map>
#include <vector>
#include <string>

using std::map;
using std::string;
using std::vector;

#ifndef min
#define min(a,b) (a > b ? b : a)
#endif

extern string g_strapppath;

string getlastpath(const string& strpath);
unsigned long getfirstint(const char* pszstr);
string i2str(long inum);
string touppercase(string& str);
string getMD5(const string& str);
string getMD5(const char* pszstr, int istrlen);
string getenpwd(const string& struin, const string& strpwd, const string& strvertify);
string getmidstr(const string& strsrc, const string& strbegin, const string& strend);
void write_to_file(const string& strfilepath, const string& strconn, bool isadd = true);
string get_from_file(const string& strfilepath);
int get_from_file(const string& strfilepath, char* szconn, int icount);
string str_replace(const string& strsrc, const string& strold, const string& strnew);
string str_format(const char* fmt, ...);
int str_explode(const string& strsrc, const string& strsplit, vector<string>& vecstr);
int str_getformsubmit(const string& strsrc, map<string,string>& mapnval, const char** pnamefilter = NULL, int ilen = 0);
void replace(std::string& str, std::string olds, std::string news);
std::string date(std::string fmt = "Y-m-d H:i:s", int nday  = 0 );
int hex_to_int(const char* phexstr);
int unicode_to_utf(const string& struni, char* ascii, int asilen);
int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput, int outSize);
int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic);
#endif



