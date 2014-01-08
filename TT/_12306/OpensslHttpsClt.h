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
	// 初始化openssl库
	static void init_openssl();

public:
	// 返回错误信息
	const char* GetLastError();

	// 网站ip及端口 默认443
	bool set_https_connect(const char* addrip, unsigned short usport);
	
	// 设置证书路径
	void set_https_cerfile(const char* cerfile);

	// 设置加密算法
	void set_https_algorithm(EM_ALGORITHM algorithm);
	
	// 检查证书是否可用
	bool https_check_cer();

	// 连接https服务端
	bool https_connect();

	// 所有加密算法的信息
	string get_https_algorithm_info();

	// 得到服务端的证书并打印些信息
	string get_https_certificate_info();

	// 接收数据
	int https_read_chars(char* charreads, int iread);

	// 发送数据
	int https_write_chars(const char* charwrites, int iwrite);

	// 重置参数
	void https_reset();

	// 释放资源
	void https_free();

private:
	// 格式化数据
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
