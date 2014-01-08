
#pragma warning(disable:4786)

#include "encrypt_data.h"

#define CODE_TABLE_LEN 95
#ifndef min 
#define min(a,b) (a>b?b:a)
#endif

CEncrypt_Data::CEncrypt_Data()
{
	memset(m_szEnchar, 0, sizeof(m_szEnchar));
	strcpy(m_szEnchar, "OPUV01234wxyzABCD9abcdeJKLMNWXYZ `~!@#f5678ghijklmnopquvEFGHIQRST$%^&*()_+-=[]\\{}|;':\",./<>?");
	////////////////////0123456789
	memset(m_szDechar, 0, sizeof(m_szDechar));
	int nch = 0;
	for (int i = 0; i < CODE_TABLE_LEN; ++i)
	{
		nch = m_szEnchar[i];
		m_szDechar[nch] = i;
	}
}

int CEncrypt_Data::enfile(const char* pszfile)
{
	size_t ullen = 0;
	size_t ulcount = 0;
	char szblock[MAX_LINE_LEN + 1] = {0};
	char szInchar[MAX_IN_LEN + 1] = {0};
	char szEnchar[MAX_OUT_LEN + 1] = {0};
	int iOutLen = MAX_OUT_LEN;

	FILE* pfile = fopen(pszfile, "r+b");
	if (pfile == NULL)
	{
		return -1;
	}
	while ((ullen = fread(szblock, 1, MAX_LINE_LEN, pfile)) == MAX_LINE_LEN)
	{	
		ullen = min(MAX_IN_LEN - ulcount, ullen);
		memcpy(szInchar + ulcount, szblock, ullen);
		ulcount += ullen;
		if (ulcount >= MAX_IN_LEN)
		{
			break;
		}
		memset(szblock, 0x0, ullen);
	}
	fclose(pfile);
	ullen = min(MAX_IN_LEN - ulcount, ullen);
	memcpy(szInchar + ulcount, szblock, ullen);
	ulcount += ullen;

	int iret = encode(szInchar, ulcount, szEnchar, &iOutLen);
	if (iret == 0)
	{
		FILE* pfile = fopen(pszfile, "wb");
		fwrite(szEnchar, 1, iOutLen, pfile);
		fclose(pfile);
	}	
	return iret;
	
}

int CEncrypt_Data::defile(const char* pszfile)
{
	size_t ullen = 0;
	size_t ulcount = 0;
	char szblock[MAX_LINE_LEN + 1] = {0};
	char szInchar[MAX_OUT_LEN + 1] = {0};
	char szDechar[MAX_IN_LEN + 1] = {0};
	int iOutLen = MAX_IN_LEN;

	FILE* pfile = fopen(pszfile, "r+b");
	if (pfile == NULL)
	{
		return -1;
	}
	while ((ullen = fread(szblock, 1, MAX_LINE_LEN, pfile)) == MAX_LINE_LEN)
	{	
		ullen = min(MAX_OUT_LEN - ulcount, ullen);
		memcpy(szInchar + ulcount, szblock, ullen);
		ulcount += ullen;
		if (ulcount >= MAX_OUT_LEN)
		{
			break;
		}
		memset(szblock, 0x0, ullen);
	}
	fclose(pfile);
	ullen = min(MAX_OUT_LEN - ulcount, ullen);
	memcpy(szInchar + ulcount, szblock, ullen);
	ulcount += ullen;

	int iret = decode(szInchar, ulcount, szDechar, &iOutLen);
	if (iret == 0)
	{
		FILE* pfile = fopen(pszfile, "wb");
		fwrite(szDechar, 1, iOutLen, pfile);
		fclose(pfile);
	}

	return iret;
}

int CEncrypt_Data::enfile(const char* pszfile, char* pszconn, int* ilen)
{
	size_t ullen = 0;
	size_t ulcount = 0;
	char szblock[MAX_LINE_LEN + 1] = {0};
	char szInchar[MAX_IN_LEN + 1] = {0};
	char szEnchar[MAX_OUT_LEN + 1] = {0};
	int iOutLen = MAX_OUT_LEN;
	
	FILE* pfile = fopen(pszfile, "r+b");
	if (pfile == NULL)
	{
		return -1;
	}
	while ((ullen = fread(szblock, 1, MAX_LINE_LEN, pfile)) == MAX_LINE_LEN)
	{	
		ullen = min(MAX_IN_LEN - ulcount, ullen);
		memcpy(szInchar + ulcount, szblock, ullen);
		ulcount += ullen;
		if (ulcount >= MAX_IN_LEN)
		{
			break;
		}
		memset(szblock, 0x0, ullen);
	}
	fclose(pfile);
	ullen = min(MAX_IN_LEN - ulcount, ullen);
	memcpy(szInchar + ulcount, szblock, ullen);
	ulcount += ullen;
	
	int iret = encode(szInchar, ulcount, szEnchar, &iOutLen);
	if (iret == 0)
	{
		*ilen = min(*ilen, iOutLen);
		memcpy(pszconn, szEnchar, *ilen);
	}
	
	return iret;
}

int CEncrypt_Data::defile(const char* pszfile, char* pszconn, int* ilen)
{
	size_t ullen = 0;
	size_t ulcount = 0;
	char szblock[MAX_LINE_LEN + 1] = {0};
	char szInchar[MAX_OUT_LEN + 1] = {0};
	char szDechar[MAX_IN_LEN + 1] = {0};
	int iOutLen = MAX_IN_LEN;
	
	FILE* pfile = fopen(pszfile, "r+b");
	if (pfile == NULL)
	{
		return -1;
	}
	while ((ullen = fread(szblock, 1, MAX_LINE_LEN, pfile)) == MAX_LINE_LEN)
	{	
		ullen = min(MAX_OUT_LEN - ulcount, ullen);
		memcpy(szInchar + ulcount, szblock, ullen);
		ulcount += ullen;
		if (ulcount >= MAX_OUT_LEN)
		{
			break;
		}
		memset(szblock, 0x0, ullen);
	}
	fclose(pfile);
	ullen = min(MAX_OUT_LEN - ulcount, ullen);
	memcpy(szInchar + ulcount, szblock, ullen);
	ulcount += ullen;
	
	int iret = decode(szInchar, ulcount, szDechar, &iOutLen);
	if (iret == 0)
	{
		*ilen = min(*ilen, iOutLen);
		memcpy(pszconn, szDechar, *ilen);
	}
	
	return iret;
}

int CEncrypt_Data::encode(const char* pszInchar, int iInlen, char* pszOutchar, int* iOutlen)
{	
	std::string strEncode;
	int ncount =  iInlen / 3;
	int nval = 0;
	int nidx = 0;
	unsigned char sztmp[3] = {0};
	unsigned char* Data = (unsigned char* )pszInchar;
	
	for (nidx = 0; nidx < ncount; ++nidx)
	{
		sztmp[0] = *Data++;
		sztmp[1] = *Data++;
		sztmp[2] = *Data++;
		strEncode += m_szEnchar[(sztmp[0]>>2)];
		strEncode += m_szEnchar[((sztmp[0]<<4)&0x30)|(sztmp[1]>>4)];
		strEncode += m_szEnchar[((sztmp[1]<<2)&0x3F)|(sztmp[2]>>6)];
		strEncode += m_szEnchar[sztmp[2]&0x3F];
	}
	
	ncount = iInlen % 3;
	nidx = 0;
	while (nidx < ncount)
	{
		sztmp[nidx++] = *Data++;
	}
	if (ncount > 0)
	{
		strEncode += m_szEnchar[(sztmp[0]>>2)];
		if (ncount == 1)
		{			
			strEncode += m_szEnchar[((sztmp[0]<<4)&0x30)];
		}
		else
		{		
			strEncode += m_szEnchar[((sztmp[0]<<4)&0x30)|(sztmp[1]>>4)];
			strEncode += m_szEnchar[(sztmp[1]<<2)&0x3F];
		}
	}
	strncpy(pszOutchar, strEncode.c_str(), *iOutlen);
	*iOutlen = strlen(pszOutchar);

	return 0;
}

int CEncrypt_Data::decode(const char* pszInchar, int iInlen, char* pszOutchar, int* ioutlen)
{ 
   int ncount = iInlen/4;
   int nidx = 0;
   unsigned char usztmp[4] = {0};
   unsigned char uszval[3] = {0};
   int noutlen = 0;
   char* szdecode = new char[iInlen*3/4 + 1];

   if (iInlen == 0)
   {
	   return 0;
   }

   if (szdecode)
   {
	   memset(szdecode, 0, sizeof(szdecode));
	   
	   for (; nidx < ncount; ++nidx)
	   {
		   usztmp[0] = m_szDechar[*pszInchar++];
		   usztmp[1] = m_szDechar[*pszInchar++];
		   usztmp[2] = m_szDechar[*pszInchar++];
		   usztmp[3] = m_szDechar[*pszInchar++];
		   
		   uszval[0] = usztmp[0]<<2|usztmp[1]>>4;
		   uszval[1] = usztmp[1]<<4|usztmp[2]>>2;
		   uszval[2] = usztmp[2]<<6|usztmp[3];

		   memcpy(szdecode + noutlen, uszval, 3);
		   noutlen += 3;
	   }
	   ncount = iInlen % 4;
	   memset(uszval, 0, sizeof(uszval));
	   if (ncount > 0)
	   {
		   if (ncount == 2)
		   {
			   usztmp[0] = m_szDechar[*pszInchar++];
			   usztmp[1] = m_szDechar[*pszInchar++];
			   uszval[0] = usztmp[0]<<2|usztmp[1]>>4;
			   //uszval[1] = usztmp[1]<<4;
			   
			   memcpy(szdecode + noutlen, uszval, 1);
			   noutlen += 1;
		   }
		   else
		   {
			   usztmp[0] = m_szDechar[*pszInchar++];
			   usztmp[1] = m_szDechar[*pszInchar++];
			   usztmp[2] = m_szDechar[*pszInchar++];
			   uszval[0] = usztmp[0]<<2|usztmp[1]>>4;
			   uszval[1] = usztmp[1]<<4|usztmp[2]>>2;
			   
			   memcpy(szdecode + noutlen, uszval, 2);
			   noutlen += 2;
		   }
	   }	   
	   noutlen = min(*ioutlen, noutlen);
	   memcpy(pszOutchar, szdecode, noutlen); 
	   *ioutlen = noutlen;
   }   
   
   if (szdecode)
   {
	   delete []szdecode;
	   szdecode = NULL;
   }   
   
   return 0;
}

int CEncrypt_Data::save(const char* pszfile, const char* szfileconn, int iconlen, int add /* = false */)
{
	FILE* pf = NULL;
	if (add)
	{
		pf = fopen(pszfile, "ab");
	}
	else
	{
		pf = fopen(pszfile, "wb");
	}
	if (pf == NULL)
	{
		return -1;
	}

	fwrite(szfileconn, sizeof(char), iconlen, pf);
	fflush(pf);
	fclose(pf);
	return 0;
}
	

