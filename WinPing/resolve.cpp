// 6 
#include <winsock2.h>              // socket 标准头文件
#include <ws2tcpip.h>              // TCP/IP实现相关(6.1)
#include <strsafe.h>               // 提供安全的字符串操作(6.2)
#include <stdio.h>
#include <stdlib.h>
 
#include "resolve.h"


// 7
int PrintAddress(SOCKADDR *sa, int salen)
{
    char    host[NI_MAXHOST],
		serv[NI_MAXSERV];
    int     hostlen = NI_MAXHOST,
		servlen = NI_MAXSERV,
		rc;
	
    rc = getnameinfo(               // 提供协议无关的名字解析(7.1)
		sa,
		salen,
		host,
		hostlen,
		serv,
		servlen,
		NI_NUMERICHOST | NI_NUMERICSERV
		);
    if (rc != 0)
    {
        fprintf(stderr, "%s: getnameinfo failed: %d\n", __FILE__, rc);
        return rc;
    }
    if (strcmp(serv, "0") != 0)
    {
        if (sa->sa_family == AF_INET)
            printf("[%s]:%s", host, serv);
        else
            printf("%s:%s", host, serv);
    }
    else
        printf("%s", host);
	
    return NO_ERROR;
}

int FormatAddress(SOCKADDR *sa, int salen, char *addrbuf, int addrbuflen)
{
    char    host[NI_MAXHOST],
		serv[NI_MAXSERV];
    int     hostlen = NI_MAXHOST,
		servlen = NI_MAXSERV,
		rc;
    HRESULT hRet;
	
    rc = getnameinfo(
		sa,
		salen,
		host,
		hostlen,
		serv,
		servlen,
		NI_NUMERICHOST | NI_NUMERICSERV
		);
    if (rc != 0)
    {
        fprintf(stderr, "%s: getnameinfo failed: %d\n", __FILE__, rc);
        return rc;
    }
	
    if ( (strlen(host) + strlen(serv) + 1) > (unsigned)addrbuflen)
        return WSAEFAULT;
	
    addrbuf[0] = '\0';
	
    if (sa->sa_family == AF_INET)
    {
        if(FAILED(hRet = StringCchPrintf(addrbuf, addrbuflen, "%s:%s", host, serv)))
        {
            fprintf(stderr,"%s StringCchPrintf failed: 0x%x\n",__FILE__,hRet);
            return (int)hRet;
        }
    }
    else if (sa->sa_family == AF_INET6)
    {
        if(FAILED(hRet = StringCchPrintf(addrbuf, addrbuflen, "[%s]:%s", host, serv)))
        {
            fprintf(stderr,"%s StringCchPrintf failed: 0x%x\n",__FILE__,hRet);
            return (int)hRet;
        }
    }
    return NO_ERROR;
}

struct addrinfo *ResolveAddress(char *addr, char *port, int af, int type, int proto)
{
    struct addrinfo hints,
		*res = NULL;
    int             rc;
	
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags  = ((addr) ? 0 : AI_PASSIVE);
    hints.ai_family = af;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;
	
    rc = getaddrinfo(
		addr,
		port,
		&hints,
		&res
		);
    if (rc != 0)
    {
        fprintf(stderr, "Invalid address %s, getaddrinfo failed: %d\n", addr, rc);
        return NULL;
    }
    return res;
}

int ReverseLookup(SOCKADDR *sa, int salen, char *buf, int buflen)
{
    char    host[NI_MAXHOST];
    int     hostlen=NI_MAXHOST,
		rc;
    HRESULT hRet;
	
    rc = getnameinfo(
		sa,
		salen,
		host,
		hostlen,
		NULL,
		0,
		0
		);
    if (rc != 0)
    {
        fprintf(stderr, "getnameinfo failed: %d\n", rc);
        return rc;
    }
	
    buf[0] = '\0';
    if(FAILED(hRet = StringCchCopy(buf, buflen, host)))
    {
        fprintf(stderr,"StringCchCopy failed: 0x%x\n",hRet);
        return (int)hRet;
    }
	
    return NO_ERROR;
}
