
#ifndef __ENCRYPT_DATA_H__
#define __ENCRYPT_DATA_H__

#define MAX_IN_LEN 100 * 1024
#define MAX_OUT_LEN 200 * 1024
#define MAX_ENDE_LEN 256

#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN 1024
#endif

#include <string>
using std::string;

class CEncrypt_Data
{
public:
	CEncrypt_Data();
	~CEncrypt_Data(){};
	
	int enfile(const char* pszfile);
	int defile(const char* pszfile);

	int enfile(const char* pszfile, char* pszconn, int* ilen);
	int defile(const char* pszfile, char* pszconn, int* ilen);

	int encode(const char* pszInchar, int iInlen, char* pszOutchar, int* iOutlen);
	int decode(const char* pszInchar, int iInlen, char* pszOutchar, int* ioutlen);

	int save(const char* pszfile, const char* szfileconn, int iconlen, int add = false);

private:
	int m_iEnlen;
	char  m_szEnchar[MAX_ENDE_LEN + 1];

	int m_iDelen;
	char m_szDechar[MAX_ENDE_LEN + 1];

};

#endif





