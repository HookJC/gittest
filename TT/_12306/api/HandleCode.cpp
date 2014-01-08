
#include "HandleCode.h"

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#define IsHexNum(c) ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))

inline BYTE toHex(const BYTE &x)
{
   return x > 9 ? x + 55: x + 48;
}


#ifdef WIN32
// gb2312 convert to utf8
string CHandleCode::GBKToUTF8(const string& str)
{
	string strret;
	int len= MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)str.c_str(), -1, NULL,0);
    unsigned short * wszUtf8 = new unsigned short[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)str.c_str(), -1, wszUtf8, len);

    len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);
    char *szUtf8=new char[len + 1];
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL);

    strret = szUtf8;
    delete[] szUtf8;
    delete[] wszUtf8;
	szUtf8 = NULL;
	wszUtf8 = NULL;

	return strret;
}

// utf8 convert to gb2312
string CHandleCode::UTF8ToGBK(string& str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)str.c_str(), -1, NULL,0);
    unsigned short * wszGBK = new unsigned short[len+1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)str.c_str(), -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL);

    str = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
	szGBK = NULL;
	wszGBK = NULL;

	return str;
}

int CHandleCode::UTF8ToGBK(const char* instr, int inlen, char* outstr, int outlen)
{
	// 先转成宽字符  
    int nStrLen = MultiByteToWideChar(CP_UTF8, 0, instr, -1, NULL, 0);  
    wchar_t* pWStr = new wchar_t[nStrLen + 1];  
    memset(pWStr, 0, nStrLen + 1);  
    MultiByteToWideChar(CP_UTF8, 0, instr, -1, pWStr, nStrLen);  
	
    // 再转成GB2312  
    nStrLen = WideCharToMultiByte(CP_ACP, 0, pWStr, -1, NULL, 0, NULL, NULL);  
    char* pStr = new char[nStrLen + 1];
    memset(pStr, 0, nStrLen + 1);  
    WideCharToMultiByte(CP_ACP, 0, pWStr, -1, pStr, nStrLen, NULL, NULL);  
    if(pWStr)  
    {  
        delete[] pWStr;  
    }

	outlen = min(outlen, nStrLen);
	memcpy(outstr, pStr, min(outlen, nStrLen));
	delete[] pStr;

	return outlen;
}

void CHandleCode::UTF8ToUnicode(wchar_t* pOut,char *pText)  
{     
    char* uchar = (char *)pOut;   
    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);  
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);   
} 

void CHandleCode::UnicodeToUTF8(char* pOut,wchar_t* pText)   
{   
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后   
    char* pchar = (char *)pText;   
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));  
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);  
    pOut[2] = (0x80 | (pchar[0] & 0x3F));  
}

void CHandleCode::UnicodeToGB2312(char* pOut,wchar_t uData)  
{   
    WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);   
}

void CHandleCode::Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer)   
{     
    ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);  
} 

/*
string CHandleCode::GB2312ToUTF8(string& pOut, char *pText, int pLen)  
{    
    char buf[4] = {0};   
    int nLength = pLen* 3;   
    char* rst = new char[nLength];    
    memset(rst,0,nLength);    
    int i = 0 ,j = 0;   
    while(i < pLen)   
    {   
        //如果是英文直接复制就可以   
        if( *(pText + i) >= 0)   
        {    
            rst[j++] = pText[i++];  
        }   
        else    
        {    
            wchar_t pbuffer;    
            Gb2312ToUnicode(&pbuffer,pText+i);  
            UnicodeToUTF_8(buf,&pbuffer);  
            rst[j] = buf[0];     
            rst[j+1] = buf[1];   
            rst[j+2] = buf[2];   
            j += 3;   
            i += 2;   
        }     
    }    
  
    rst[j] ='\0';   //返回结果    
    pOut = rst;  
    delete []rst;
	rst = NULL;

	return pOut;
}     

string CHandleCode::UTF8ToGB2312(string& str, char *pText, int pLen)    
{     
	const int iout = pLen * 2;
    char Ctemp[4];     
    memset(Ctemp, 0, 4);   
	char* pOut = new char[iout + 1];
    int i =0 ,j = 0;   
    while(i < pLen)   
    {    
        if(pText[i] >= 0)    
        {    
            pOut[j++] = pText[i++];   
        }   
        else   
        {    
            WCHAR Wtemp;   
            UTF_8ToUnicode(&Wtemp, pText + i);  
            UnicodeToGB2312(Ctemp, Wtemp);   
            pOut[j] = Ctemp[0];  
            pOut[j + 1] = Ctemp[1];    
            i += 3;     
            j += 2;    
        }     
    }   
    pOut[j] ='\0';  
	str = pOut;

	 return str;
}
*/
#endif


// url decode %20 = " "
string CHandleCode::UrlDecode(string& str)
{
	string sOut;
	int ilen = str.size();
    for( int ix = 0; ix < ilen; ix++ )
    {
        BYTE buf[4];
        memset( buf, 0, 4 );
        if( isalnum( str[ix] ) )
        {
            buf[0] = str[ix];
        }
        else if ( '+'==( str[ix] ) )
        {
            buf[0] = ' ';
        }
        else
        {
            //buf[0] = '%';
            buf[0] = toHex( str[ix+1] << 4 );
            buf[1] = toHex( str[ix]);
        }
        sOut += (char *)buf;
    }
	str = sOut;

	return str;
}

// url encode " " = %20
string CHandleCode::UrlEncode(const string& str)
{
	string sOut;
	int ilen = str.size();
    for( int ix = 0; ix < ilen; ix++ )
    {
        BYTE buf[4];
        memset( buf, 0, 4 );
        if( isalnum( (BYTE)str[ix] ) )
        {
            buf[0] = str[ix];
        }
        else if ( isspace( (BYTE)str[ix] ) )
        {
            buf[0] = '+';
        }
        else
        {
            buf[0] = '%';
            buf[1] = toHex( (BYTE)str[ix] >> 4 );
            buf[2] = toHex( (BYTE)str[ix] % 16);
        }
        sOut += (char *)buf;
    }
	//str = sOut;
	
    return sOut;
}

