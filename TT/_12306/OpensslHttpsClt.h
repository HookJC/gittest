// OpensslHttps.h: interface for the OpensslHttps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COPENSSLHTTPSCLT_H__4FF311B1_2D8F_4947_9853_8D2E8A9A2D4E__INCLUDED_)
#define AFX_COPENSSLHTTPSCLT_H__4FF311B1_2D8F_4947_9853_8D2E8A9A2D4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <openssl/rsa.h>   
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/rand.h>

#ifdef WIN32

#include <WINSOCK.H>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libeay32.lib")

#define usleep(usecond) _sleep(usecond);
#define close(so) closesocket(so)

#elif defined LINUX

#include <sys/types.h>

#endif

#include <IOSTREAM>
#include <string>
using namespace std;

#define MAX_BUFFER_BLOCK 4096

class COpensslHttpsClt  
{
public:
	enum EM_ALGORITHM
	{
		RSA			= 0,
		RC4_MD5		= 1
	};
public:
	COpensslHttpsClt();
	virtual ~COpensslHttpsClt();

public:
	// ��ʼ��openssl��
	static void init_openssl();

public:
	// ���ش�����Ϣ
	const char* GetLastError();

	// ��վip���˿� Ĭ��443
	bool set_https_connect(const char* addrip, unsigned short usport);
	
	// ����֤��·��
	void set_https_cerfile(const char* cerfile);

	// ���ü����㷨
	void set_https_algorithm(EM_ALGORITHM algorithm);
	
	// ���֤���Ƿ����
	bool https_check_cer();

	// ����https�����
	bool https_connect();

	// ���м����㷨����Ϣ
	string get_https_algorithm_info();

	// �õ�����˵�֤�鲢��ӡЩ��Ϣ
	string get_https_certificate_info();

	// ��������
	int https_read_chars(char* charreads, int iread);

	// ��������
	int https_write_chars(const char* charwrites, int iwrite);

	// ���ò���
	void https_reset();

	// �ͷ���Դ
	void https_free();

private:
	// ��ʽ������
	int https_get_printf(const char* fmt, ...);

private:
	string m_strerrmsg;
	int m_ierr;

	SOCKET m_so;    
    struct sockaddr_in m_srvaddr;
	int m_itimeout;
	string m_strip;
	unsigned short m_usport;

	EM_ALGORITHM m_uialgorithm;
    SSL_CTX* m_ctx;
    SSL*     m_ssl;
    X509*    m_server_cert;
    char     m_szbuf[MAX_BUFFER_BLOCK + 1];
	char     m_szcerpath[MAX_PATH + 1];
};

#endif // !defined(AFX_COPENSSLHTTPSCLT_H__4FF311B1_2D8F_4947_9853_8D2E8A9A2D4E__INCLUDED_)
