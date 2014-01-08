

#include "function.h"
#include <assert.h>

using namespace std;

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef IshexNum
#define IsHexNum(c) ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#endif

#define MAX_MD5_LEN	32
#define MAX_QQ_LEN	16

string getlastpath(const string& strpath)
{
	string strret = strpath;
	size_t npos = 0;
	size_t nlastpos = 0;
	while (((npos = strret.find("\\")) != string::npos) || ((npos = strret.find("/")) != string::npos))
	{
		npos += 1;
		strret = strret.substr(npos);
		nlastpos += npos;
	}
	strret = strpath.substr(0, nlastpos - 1);
	return strret;	
}

unsigned long getfirstint(const char* pszstr)
{
	unsigned long ulnum = 0;
	int ilen = strlen(pszstr);
	int inum = 0;

	for (int i = 0; i < ilen; ++i)
	{
		inum = pszstr[i] - '0';
		if (ulnum > 0 && (inum < 0 || inum > 9))
		{
			break;
		}
		if (inum >= 0 && inum <= 9)
		{
			ulnum = ulnum * 10 + inum;
		}
	}

	return ulnum;
}

string i2str(long inum)
{
	string strret;
	char szbuf[20 + 1] = {
		0
	};
	sprintf(szbuf, "%d", inum);
	return strret = szbuf;	
}

string touppercase(string& str)
{
	int ilen = str.size();
	while (--ilen >= 0)
	{
		if (str[ilen] >='a' && str[ilen] <= 'z')
		{
			str[ilen] = str[ilen] - 32;
		}
	}

	return str;
}

string getMD5(const string& str)
{
	MD5 getmd5(str);
	string md5 = getmd5.md5();
	touppercase(md5);
	return md5;
}

string getMD5(const char* pszstr, int istrlen)
{
	string str;	
	MD5 getmd5;

	getmd5.update(pszstr, istrlen);
	str = getmd5.finalize().md5();
	touppercase(str);

	return str;
}

// 数字右对齐，转内存数据
char* uistrHex(char* szUi)
{
	char* psz = szUi;
	int ilen = 0;
	char sztmp[MAX_QQ_LEN + 1] = {0};	
	unsigned char szHex[MAX_QQ_LEN / 2 + 1] = {0};
	char szuinhex[MAX_QQ_LEN + 1] = {0};

	memset(sztmp, '0', MAX_QQ_LEN);
	sprintf(szuinhex, "%0x", atol(szUi));
	ilen = strlen(szuinhex);
	memcpy(sztmp + MAX_QQ_LEN - ilen, szuinhex, min(MAX_QQ_LEN, ilen));

	for (int i = 0; i < MAX_QQ_LEN; i += 2)
	{
		szHex[i/2] = ((sztmp[i] >= '0' && sztmp[i] <= '9') ? sztmp[i] - '0' : sztmp[i] - 'a' + 10) * 16
			 + ((sztmp[i + 1] >= '0' && sztmp[i + 1] <= '9') ? sztmp[i + 1] - '0' : sztmp[i + 1] - 'a' + 10);
	}

	memset(szUi, 0x0, strlen(szUi));
	memcpy(szUi, (char* )szHex, MAX_QQ_LEN / 2);
	return psz;
}

char* strtoHex(char* szstr)
{
	string strpsz = szstr;
	int ilen = strlen(szstr);
	unsigned char szHex[MAX_MD5_LEN] = {0};
	int iNum = 0;

	for (int i = 0; i < ilen; i += 2)
	{
		 iNum = ((szstr[i] >= '0' && szstr[i] <= '9') ? szstr[i] - '0' : szstr[i] - 'A' + 10) * 16
			 + ((szstr[i + 1] >= '0' && szstr[i + 1] <= '9') ? szstr[i + 1] - '0' : szstr[i + 1] - 'A' + 10);
		 szHex[i / 2] = iNum;
	}
	
	memset(szstr, 0x0, ilen);
	memcpy(szstr, (char* )szHex, MAX_MD5_LEN / 2);	
	return szstr;
}

string getenpwd(const string& struin, const string& strpwd, const string& strvertify)
{
	char szstr[64] = {0};
	char szuin[MAX_QQ_LEN + 1] = {0};
	char szpwd[MAX_QQ_LEN + 1] = {0};	
	char szHexMd5[MAX_MD5_LEN + 1] = {0};
	string strresult = "";
	string strver = strvertify;
	
	strcpy(szuin, struin.c_str());
	strcpy(szpwd, strpwd.c_str());
	
	uistrHex(szuin);
	strresult = getMD5(szuin, MAX_QQ_LEN / 2);

	strresult = getMD5(szpwd);
	strcpy(szHexMd5, strresult.c_str());
	strtoHex(szHexMd5);

	memcpy(szHexMd5 + MAX_MD5_LEN / 2, szuin, MAX_QQ_LEN / 2);
	strresult = getMD5(szHexMd5, MAX_MD5_LEN / 2 + MAX_QQ_LEN / 2);

	strresult = strresult + touppercase(strver);	
	strresult = getMD5(strresult);
	strresult = touppercase(strresult);
	return strresult;
}

string getmidstr(const string& strsrc, const string& strbegin, const string& strend)
{
	size_t ulpos = 0;
	string strmid;
	if ((ulpos = strsrc.find(strbegin.c_str())) == string::npos)
	{
		return strmid; 
	}	
	strmid = strsrc.substr(ulpos + strbegin.size());
	
	if ((ulpos = strmid.find(strend.c_str())) == string::npos)
	{
		return strmid;
	}
	
	strmid = strmid.substr(0, ulpos);
	return strmid;
}

void write_to_file(const string& strfilepath, const string& strconn, bool isadd /* = true */)
{
	if (strconn.empty())
	{
		return;
	}

	FILE* pf = NULL;
	if (isadd)
	{
		pf = fopen(strfilepath.c_str(), "a+");
	}
	else
	{
		pf = fopen(strfilepath.c_str(), "w+");
	} 
	if (pf)
	{
		fwrite(strconn.c_str(), 1, strconn.size(), pf);
		fprintf(pf, "\n");
		fflush(pf);
		fclose(pf);
	}
}

string get_from_file(const string& strfilepath)
{
	string strret;
	FILE* pf = fopen(strfilepath.c_str(), "rb");
	if (pf)
	{
		char szblock[1024 + 1] = {0};
		while (fread(szblock, 1, 1024, pf) > 0)
		{
			strret += szblock;
			memset(szblock, 0, 1024);
		}
		fclose(pf);
	}
	return strret;
}

int get_from_file(const string& strfilepath, char* szconn, int icount)
{
	int iread = 0;
	int ircount = 0;
	FILE* pf = fopen(strfilepath.c_str(), "rb");
	{
		char szblock[1024 + 1] = {0};
		while ((iread = fread(szblock, 1, 1024, pf)) > 0)
		{
			memcpy(szconn + ircount, szblock, min(iread, icount - ircount));
			ircount += iread;
			if (ircount >= icount)
			{
				break;
			}
			memset(szblock, 0, iread);
		}
		fclose(pf);
	}
	return ircount;
}

string str_replace(const string& strsrc, const string& strold, const string& strnew)
{
	string strret;
	size_t ulbpos = 0;
	size_t ulepos = 0;

	while ((ulepos = strsrc.find(strold, ulbpos)) != string::npos)
	{
		strret += strsrc.substr(ulbpos, ulepos - ulbpos);
		strret += strnew;
		ulbpos = ulepos + strold.size();
	}
	strret += strsrc.substr(ulbpos);

	return strret;
}

string str_format(const char* fmt, ...)
{
	const int ilen = 4096;
	char szprint[ilen + 1] = {0};
	va_list args;
	va_start(args, fmt);
	_vsnprintf(szprint, ilen, fmt, args);
	va_end(args);
	return string(szprint);
}

int str_explode(const string& strsrc, const string& strsplit, vector<string>& vecstr)
{
	int icount = 0;
	size_t ulpos = 0;
	string strright = strsrc;

	while (!strright.empty())
	{
		++icount;
		if ((ulpos = strright.find(strsplit)) == string::npos)
		{
			vecstr.push_back(strright);
			break;
		}
		vecstr.push_back(strright.substr(0, ulpos));
		strright = strright.substr(ulpos +  strsplit.size());
	}
	return icount;
}

int str_getformsubmit(const string& strsrc, map<string,string>& mapnval, const char** pnamein /* = NULL */, int ilen /* = 0 */)
{
	int icount = 0;
	size_t ulbpos = 0;
	size_t ulepos = 0;
	string strform = strsrc;
	string strinput,strname,strvalue;
	int iname = ilen;
	int idx = 0;

	while ((ulbpos = strform.find("<input", ulepos)) != string::npos)
	{ 
		ulepos = strform.find(">", ulbpos);
		strinput = strform.substr(ulbpos, ulepos - ulbpos + 1);
		strname = getmidstr(strinput, "name=\"", "\"");
		strvalue = getmidstr(strinput, "value=\"", "\"");
		if (strname.empty())
		{
			continue;
		}

		// 只取需要的
		if (iname > 0)
		{
			for (idx = 0; idx < iname; ++idx)
			{
				if (strcmp(strname.c_str(), pnamein[idx]) == 0)
				{
					break;
				}
			}
			if (idx == iname)
			{
				continue;
			}
		}
		
		++icount;
		mapnval[strname] = strvalue;	
	}

	return icount;	
}

void replace(std::string& str, std::string olds, std::string news)
{
	size_t pos = 0;	
	while ((pos = str.find(olds, pos)) != std::string::npos)
	{
		str.replace(pos, olds.size(), news);
		pos += news.length();
	}
}

std::string date(std::string fmt /* = "Y-m-d H:i:s" */, int nday /* = 0 */)
{	
	struct tm* newtime; 
	char tmpbuf[100] = {0}; 
	time_t lt; 
	lt = time(NULL) + nday * 24 * 60 * 60; 	 
	newtime = localtime(&lt); 
	replace(fmt, "Y", "%Y");
	replace(fmt, "m", "%m");
	replace(fmt, "d", "%d");
	replace(fmt, "H", "%H");
	replace(fmt, "i", "%M");
	replace(fmt, "s", "%S");		
	strftime(tmpbuf, 100, fmt.c_str(), newtime);
	std::string str(tmpbuf);
	return str;
}

int hex_to_int(const char* phexstr)
{
	int ihexlen = strlen(phexstr);
	int inumber = 0;
	int ipos = 0;
	while (ihexlen-- > 0)
	{
		if (phexstr[ihexlen] >= '0' && phexstr[ihexlen] <= '9')
		{
			inumber += (phexstr[ihexlen] - 0x30) * pow(16, ipos);
		}
		else if (phexstr[ihexlen] >= 'a' && phexstr[ihexlen] <= 'f')
		{
			inumber += (phexstr[ihexlen] - 'a' + 10) * pow(16, ipos);
		}
		else
		{
			inumber += (phexstr[ihexlen] - 'A' + 10) * pow(16, ipos);
		}
		++ipos;
	}
	return inumber;
}

int unicode_to_utf(const string& struni, char* ascii, int asilen)
{
	size_t ulsize = struni.size();	
	size_t i, j;
	char szbuff[7] = {0};
	string strtmp;
	int icount = 0;

	for (i = 0, j = 0; i < ulsize && j < asilen;)
	{
		if (struni[i] != '\u')
		{
			ascii[j++] = struni[i++];
		}
		else
		{
			icount = 0;
			while (struni[++i] != '\u' && i < ulsize 
				&& IsHexNum(struni[i]) && icount < 4)
			{
				strtmp += struni[i];
				++icount;
			}
			if (!strtmp.empty())
			{
				int ihex = hex_to_int(strtmp.c_str());
				strtmp = "";
				ihex = enc_unicode_to_utf8_one(ihex, (unsigned char* )szbuff, 6);
				memcpy(ascii + j, szbuff, ihex);
				j += ihex;

				memset(szbuff, 0, ihex);
			}
			else
			{
				ascii[j++] = 'u';
			}
		}
	}
	return j;
}

// #c---
/*****************************************************************************
* 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.
*
* 参数:
*    unic     字符的Unicode编码值
*    pOutput  指向输出的用于存储UTF8编码值的缓冲区的指针
*    outsize  pOutput缓冲的大小
*
* 返回值:
*    返回转换后的字符的UTF8编码所占的字节数, 如果出错则返回 0 .
*
* 注意:
*     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
*        字节序分为大端(Big Endian)和小端(Little Endian)两种;
*        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
*     2. 请保证 pOutput 缓冲区有最少有 6 字节的空间大小!
****************************************************************************/
int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,
							int outSize)
{
    assert(pOutput != NULL);
    assert(outSize >= 6);
	
    if ( unic <= 0x0000007F )
    {
        // * U-00000000 - U-0000007F:  0xxxxxxx
        *pOutput     = (unic & 0x7F);
        return 1;
    }
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )
    {
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
        *(pOutput+1) = (unic & 0x3F) | 0x80;
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )
    {
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
        *(pOutput+2) = (unic & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )
    {
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+3) = (unic & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )
    {
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+4) = (unic & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )
    {
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput+5) = (unic & 0x3F) | 0x80;
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;
        return 6;
    }
	
    return 0;
}
// #c---end

/*****************************************************************************
* 根据字符的UTF-8编码的第一个字节求出该字符用UTF-8编码存储时所需要多少个
* 字节空间. 特殊地, 对于只占一个字节的字符(ASCII), 返回值为 0 .
*
* 参数:
*    char c    字符的UTF-8编码的第一个字节的值
*
* 返回值:
*   该字符用UTF-8编码存储时所需要多少个字节空间.
*   特殊地, 对于只占一个字节的字符(ASCII), 返回值为 0 .
****************************************************************************/
int enc_get_utf8_size(char c)
{
    char t = 1 << 7;
    char r = c;
    int count = 0;
    while (r & t)
    {
        r = r << 1;
        count++;
    }
    return count;
}

// #c---
/*****************************************************************************
 * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    pInput      指向输入缓冲区, 以UTF-8编码
 *    Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
 *                类型为unsigned long .
 *
 * 返回值:
 *    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned long *Unic)
{
    assert(pInput != NULL && Unic != NULL);

    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;

    *Unic = 0x0; // 把 *Unic 初始化为全零
    int utfbytes = enc_get_utf8_size(*pInput);
    unsigned char *pOutput = (unsigned char *) Unic;

    switch ( utfbytes )
    {
        case 0:
            *pOutput     = *pInput;
            utfbytes    += 1;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
            if ( (b2 & 0xE0) != 0x80 )
                return 0;
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput+3) = (b1 << 6);
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                    || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                    || ((b6 & 0xC0) != 0x80) )
                return 0;
            *pOutput     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            break;
        default:
            return 0;
            break;
    }

    return utfbytes;
}
// #c---end
