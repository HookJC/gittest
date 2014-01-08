// OpensslHttps.cpp: implementation of the OpensslHttps class.
//
//////////////////////////////////////////////////////////////////////

#include "OpensslHttpsClt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DEFAULT_HTTPS_PORT		443		// https默认端口
#define MAX_HTTPS_BLOCKS		102400	// 一次最大数据长度
#define PRINTF_BUFF_LEN			4096	// 最大格式化字串

// 算法列表
const char* algorithm[] = {
	"RSA", 
	"RC4-MD5"};

// 初始化
COpensslHttpsClt::COpensslHttpsClt()
{
	m_strerrmsg = "";
	m_ierr = 0;
	
	m_so = 0;
	m_itimeout  = 1000;
	memset(&m_srvaddr, 0, sizeof(m_srvaddr));
	memset(m_szbuf, 0, sizeof(MAX_BUFFER_BLOCK));
	
    m_ctx = NULL;
    m_ssl = NULL;
    m_server_cert = NULL;
	m_uialgorithm = RSA;
	memset(m_szcerpath, 0, sizeof(m_szcerpath));
}

// 释放资源
COpensslHttpsClt::~COpensslHttpsClt()
{
	https_free();
}

// 初始化openssl库
void COpensslHttpsClt::init_openssl()
{
	OpenSSL_add_ssl_algorithms(); /*初始化*/
    SSL_load_error_strings();     /*为打印调试信息作准备*/
}

// 返回错误信息
const char* COpensslHttpsClt::GetLastError()
{
	return m_strerrmsg.c_str();
}

// 网站ip及端口 默认443
bool COpensslHttpsClt::set_https_connect(const char* addrip, unsigned short usport)
{
	m_strip = addrip;
	m_usport = usport;
	m_so = socket(AF_INET, SOCK_STREAM, 0);
	if (m_so == INVALID_SOCKET)
	{
		https_get_printf("Create socket error. Error code: %d.", WSAGetLastError());
		return false;
	}

	// 超时设置
	int nNetTimeout = 1000; // 1秒，
	// 设置发送超时
	setsockopt(m_so, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
	// 设置接收超时
	setsockopt(m_so, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
	
	m_srvaddr.sin_family = AF_INET;
	m_srvaddr.sin_addr.S_un.S_addr = inet_addr(addrip);
	m_srvaddr.sin_port = htons(usport);
	return true;
}

// 设置证书路径
void COpensslHttpsClt::set_https_cerfile(const char* cerfile)
{
	strncpy(m_szcerpath, cerfile, sizeof(m_szcerpath) - 1);
}

// 设置加密算法
void COpensslHttpsClt::set_https_algorithm(EM_ALGORITHM algorithm)
{
	m_uialgorithm = algorithm;
}

// 检查证书是否可用
bool COpensslHttpsClt::https_check_cer()
{
	m_ctx = SSL_CTX_new(SSLv23_client_method()); // 采用什么协议(SSLv2/SSLv3/TLSv1)在此指定 
	if ( NULL == m_ctx )
	{
		https_get_printf("https_check_cer error. Error info: %s", ERR_reason_error_string(ERR_get_error()));
		return false;
	}

	SSL_CTX_set_verify(m_ctx, SSL_VERIFY_NONE, NULL);   /*验证与否*/
    SSL_CTX_load_verify_locations(m_ctx, m_szcerpath, NULL);
	
    //if ((err = SSL_CTX_use_certificate_file(ctx, PEM_CER, SSL_FILETYPE_PEM)) <= 0)
	if (SSL_CTX_use_certificate_file(m_ctx, m_szcerpath, SSL_FILETYPE_PEM) <= 0)
	{
		SSL_CTX_free(m_ctx);
        https_get_printf("https_check_cer error. Error info: %s", ERR_reason_error_string(ERR_get_error()));
        return false;
    }
	SSL_CTX_set_cipher_list(m_ctx, algorithm[m_uialgorithm]);
	return true;
}

// 连接https服务端
bool COpensslHttpsClt::https_connect()
{
	if (m_so > 0)
	{
		close(m_so);
		m_so = 0;
	}	
	if (m_ssl)
	{
		SSL_shutdown(m_ssl);
		SSL_free(m_ssl);
		m_ssl = NULL;
	}
	if (!set_https_connect(m_strip.c_str(), m_usport))
	{
		return false;
	}

	int iret = connect(m_so, (struct sockaddr*)&m_srvaddr, sizeof(m_srvaddr));
	if (iret != 0)
	{
		https_get_printf("connect error. Error code: %d", WSAGetLastError());
		return false;
	}	

	m_ssl = SSL_new (m_ctx);                        
    if ( NULL == m_ssl )
	{
		close(m_so);		
		
        https_get_printf("connect error. Error info: %s\n", ERR_reason_error_string(ERR_get_error()));
        return false;
	}
	
    SSL_set_fd(m_ssl, m_so);
	
    iret = SSL_connect (m_ssl);
	if ( iret <= 0 )
	{
		close(m_so);

        https_get_printf("connect error. Error info: %s\n", ERR_reason_error_string(ERR_get_error()));
        return false;
	}
	return true;
}

// 所有加密算法的信息
string COpensslHttpsClt::get_https_algorithm_info()
{
	char szcipher[100] = {0};
	sprintf(szcipher, "%s", SSL_get_cipher(m_ssl));
	return string(szcipher);
}

// 得到服务端的证书并打印些信息
string COpensslHttpsClt::get_https_certificate_info()
{
	string strret;
	char* str;

	if (m_server_cert == NULL)
	{
		m_server_cert = SSL_get_peer_certificate (m_ssl); 
	}
    if ( NULL == m_server_cert)
    {
        return ("Server does not have certificate.");
    }
		
    str = X509_NAME_oneline (X509_get_subject_name (m_server_cert), 0, 0);
    if ( NULL == str )
	{
        https_get_printf("Error: %s\n", ERR_reason_error_string(ERR_get_error()));
		return m_strerrmsg;
	}
	strret += "subject: ";
	strret += str;
	strret += "\n";
    OPENSSL_free(str);
	
    str = X509_NAME_oneline (X509_get_issuer_name(m_server_cert), 0, 0);
	if ( NULL == str )
	{
        https_get_printf("Error: %s\n", ERR_reason_error_string(ERR_get_error()));
		return m_strerrmsg;
	}
	strret += "issuer: ";
	strret += str;
	strret += "\n";
    OPENSSL_free(str);
	
    // X509_free(server_cert);  /*如不再需要,需将证书释放 */
	return strret;
}

// 接收数据
int COpensslHttpsClt::https_read_chars(char* charreads, int iread)
{
	int icount = 0;
	int iret = 0;
	
	char* pszlen = NULL;
	int ilength = 0;
	int ibodylen = 0;
	int ichunklen = 0;
	bool bchunked = false;
	bool blength = false;
	
	memset(m_szbuf, 0, sizeof(m_szbuf));
	while ((iret = SSL_read(m_ssl, m_szbuf, MAX_BUFFER_BLOCK)) > 0)
	{
		memcpy(charreads + icount, m_szbuf, iret);
		icount += iret;
		memset(m_szbuf, 0, iret);
		if (icount + MAX_BUFFER_BLOCK > iread)
		{
			break;
		}

		if (blength)
		{
			ibodylen += iret;
			if (ibodylen >= ilength)
			{
				break;
			}
		}		
		
		if (!bchunked && !blength)
		{
			if ((pszlen = strstr(charreads, "Content-Length: ")) != NULL)
			{
				blength = true;
				pszlen += strlen("Content-Length: ");
				ilength = atoi(pszlen);
				if (ilength == 0) //头响应包
				{
					break;
				}
				ibodylen = iret - (strstr(charreads, "\r\n\r\n") - charreads + 4);
				if (ibodylen >= ilength) // 首次完整包
				{
					break;
				}
			}
			else
			{
				bchunked = true;
			}
		}
		
		if (bchunked)
		{
			if (icount - 4 > 1 && strcmp(charreads + icount - 4 - 1, "0\r\n\r\n") == 0)
			{
				break;
			}
		}
	}
	return icount;
}

// 发送数据
int COpensslHttpsClt::https_write_chars(const char* charwrites, int iwrite)
{
	return SSL_write(m_ssl, charwrites, iwrite);
}

// 重置参数
void COpensslHttpsClt::https_reset()
{
	https_free();
}


// 释放资源
void COpensslHttpsClt::https_free()
{
	if (m_ctx)
	{
		SSL_CTX_free(m_ctx);
		m_ctx = NULL;
	}
	
	if (m_so > 0)
	{
		close(m_so);
		m_so = 0;
	}
	
	if (m_ssl)
	{
		SSL_shutdown(m_ssl);
		SSL_free(m_ssl);
		m_ssl = NULL;
	}
	
	if (m_server_cert)
	{
		X509_free(m_server_cert);
		m_server_cert = NULL;
	}

	m_strerrmsg = "";
	m_ierr = 0;
	
	m_so = 0;
	memset(&m_srvaddr, 0, sizeof(m_srvaddr));
	memset(m_szbuf, 0, sizeof(MAX_BUFFER_BLOCK));
	memset(m_szcerpath, 0, sizeof(m_szcerpath));
}

// 格式化数据
int COpensslHttpsClt::https_get_printf(const char* fmt, ...)
{
	char szprint[PRINTF_BUFF_LEN + 1] = {0};
	va_list args;
	va_start(args, fmt);
	_vsnprintf(szprint, PRINTF_BUFF_LEN, fmt, args);
	va_end(args);
	m_strerrmsg = szprint;
	return strlen(szprint);
}