
#include <stdio.h>

// 64位架构
#ifdef _IA64_
#pragma warning (disable: 4267)
#endif
// 此宏定义使windows.h剔除部分头文件，加快编译速度
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include "resolve.h"
#include "iphdr.h"

#define DEFAULT_DATA_SIZE      32       // icmp 数据段大小
#define DEFAULT_SEND_COUNT     4        // ping 的次数
#define DEFAULT_RECV_TIMEOUT   6000     // 接收超时 
#define DEFAULT_TTL            128      // 最大跳站术
#define MAX_RECV_BUF_LEN       0xFFFF   // 最大接收缓冲区大小

int   gAddressFamily=AF_UNSPEC,        
gProtocol=IPPROTO_ICMP,          
gTtl=DEFAULT_TTL;                
int   gDataSize=DEFAULT_DATA_SIZE;     
BOOL  bRecordRoute=FALSE;               //  是否记录路由
char *gDestination=NULL,               
recvbuf[MAX_RECV_BUF_LEN];       
int   recvbuflen = MAX_RECV_BUF_LEN; 

/************************************************************************/
// struct 


/************************************************************************/


//  ping 命令使用方法
void usage(char *progname)
{
    printf("usage: %s [options] <host> \n", progname);
    printf("        host        Remote machine to ping\n");
    printf("        options: \n");
    printf("            -a 4|6       Address family (default: AF_UNSPEC)\n");
    printf("            -i ttl       Time to live (default: 128) \n");
    printf("            -l bytes     Amount of data to send (default: 32) \n");
    printf("            -r           Record route (IPv4 only)\n");
 
    return;
}
//  初始话icmp头部
void InitIcmpHeader(char *buf, int datasize)
{
    ICMP_HDR   *icmp_hdr=NULL;
    char       *datapart=NULL;
    //  详见icmp头部定义
    icmp_hdr = (ICMP_HDR *)buf;
    icmp_hdr->icmp_type     = ICMPV4_ECHO_REQUEST_TYPE;  
    icmp_hdr->icmp_code     = ICMPV4_ECHO_REQUEST_CODE;
    icmp_hdr->icmp_id       = (USHORT)GetCurrentProcessId();  //进程号
    icmp_hdr->icmp_checksum = 0;   //  序列号未定义，校验和未计算
    icmp_hdr->icmp_sequence = 0;   //  序列号置空
 
    datapart = buf + sizeof(ICMP_HDR);   //  指针移至数据段头
    memset(datapart, 'E', datasize);     //  填充数据段
}
// 计算校验和
USHORT checksum(USHORT *buffer, int size)
{
    unsigned long cksum=0;
 
    while (size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size)
    {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (USHORT)(~cksum);
}
// 传参解析
BOOL ValidateArgs(int argc, char **argv)
{
    int                i;
    BOOL               isValid = FALSE;
 
    for(i=1; i < argc ;i++)
    {
        if ((argv[i][0] == '-') || (argv[i][0] == '/'))
        {
            switch (tolower(argv[i][1]))
            {
                case 'a':       
                    if (i+1 >= argc)
                    {
                        usage(argv[0]);
                        goto CLEANUP;
                    }
                    if (argv[i+1][0] == '4')
                        gAddressFamily = AF_INET;
                    else if (argv[i+1][0] == '6')
                        gAddressFamily = AF_INET6;
                    else
                    {
                        usage(argv[0]);
                        goto CLEANUP;
                    }
 
                    i++;
                    break;
                case 'i':        // 设置最大跳值
                    if (i+1 >= argc)
                    {
                        usage(argv[0]);
                        goto CLEANUP;
                    }
 
                    gTtl = atoi(argv[++i]);
                    break;
                case 'l':        //  icmp数据段大小设置
                    if (i+1 >= argc)
                    {
                        usage(argv[0]);
                        goto CLEANUP;
                    }
 
                    gDataSize = atoi(argv[++i]);
                    break;
                case 'r':        //  记录路由选项
                    bRecordRoute = TRUE;
                    break;
                default:
                    usage(argv[0]);
                    goto CLEANUP;
            }
        }
        else
        {
            gDestination = argv[i];
        }
    }
 
    isValid = TRUE;
 
CLEANUP:   
    return isValid;
}
//  设置icmp序列号
void SetIcmpSequence(char *buf)
{
    ULONG    sequence=0;
 
    sequence = GetTickCount();
    if (gAddressFamily == AF_INET)
    {
        ICMP_HDR    *icmpv4=NULL;
 
        icmpv4 = (ICMP_HDR *)buf;
 
        icmpv4->icmp_sequence = (USHORT)sequence;
    }
    else if (gAddressFamily == AF_INET6)
    {
        ICMPV6_HDR          *icmpv6=NULL;
        ICMPV6_ECHO_REQUEST *req6=NULL;
 
        icmpv6 = (ICMPV6_HDR *)buf;
        req6   = (ICMPV6_ECHO_REQUEST *)(buf + sizeof(ICMPV6_HDR));
 
        req6->icmp6_echo_sequence = (USHORT)sequence;
    }
}
//  计算icmp校验和
void ComputeIcmpChecksum(SOCKET s, char *buf, int packetlen, struct addrinfo *dest)
{
    if (gAddressFamily == AF_INET)
    {
        ICMP_HDR    *icmpv4=NULL;
 
        icmpv4 = (ICMP_HDR *)buf;
        icmpv4->icmp_checksum = 0;
        icmpv4->icmp_checksum = checksum((USHORT *)buf, packetlen);
    }
    else if (gAddressFamily == AF_INET6)
    {
        ICMPV6_HDR  *icmpv6=NULL;
 
        icmpv6 = (ICMPV6_HDR *)buf;
        icmpv6->icmp6_checksum = 0;
        icmpv6->icmp6_checksum = ComputeIcmp6PseudoHeaderChecksum(
                s,
                buf,
                packetlen,
                dest
                );
    }
}
//  发布异步接收
int PostRecvfrom(SOCKET s, char *buf, int buflen, SOCKADDR *from, int *fromlen, WSAOVERLAPPED *ol)
{
    WSABUF  wbuf;
    DWORD   flags,
            bytes;
    int     rc;
 
    wbuf.buf = buf;
    wbuf.len = buflen;
 
    flags = 0;
 
    rc = WSARecvFrom(         //  通过重叠IO实现异步接收
            s,
           &wbuf,
            1,
           &bytes,
           &flags,
            from,
            fromlen,
            ol,
            NULL
            );
    if (rc == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            fprintf(stderr, "WSARecvFrom failed: %d\n", WSAGetLastError());
            return SOCKET_ERROR;
        }
    }
    return NO_ERROR;
}

//  8
void PrintPayload(char *buf, int bytes)
{
    int     hdrlen=0,
		routes=0,
		i;
	
    UNREFERENCED_PARAMETER(bytes);
	
    if (gAddressFamily == AF_INET)
    {
        SOCKADDR_IN      hop;
        IPV4_OPTION_HDR *v4opt=NULL;
        IPV4_HDR        *v4hdr=NULL;
		
        hop.sin_family = (USHORT)gAddressFamily;
        hop.sin_port   = 0;
		
        v4hdr = (IPV4_HDR *)buf;
        hdrlen = (v4hdr->ip_verlen & 0x0F) * 4;  // 计算IP头部长度(8.1)
		
        if (hdrlen > sizeof(IPV4_HDR))   // 如果长度大于无选项IP头部长度
        {
			// 选项头部指针
            v4opt = (IPV4_OPTION_HDR *)(buf + sizeof(IPV4_HDR));
			//计算路由数(8.2)
            routes = (v4opt->opt_ptr / sizeof(ULONG)) - 1;
            for(i=0; i < routes ;i++)
            {
                hop.sin_addr.s_addr = v4opt->opt_addr[i];
                if (i == 0)
                    printf("    Route: ");
                else
                    printf("           ");
                PrintAddress((SOCKADDR *)&hop, sizeof(hop));
				if (i < routes-1)
                    printf(" ->\n");
                else
                    printf("\n");
            }
        }
    }
    return;
}

// 设置最大跳站数
int SetTtl(SOCKET s, int ttl)
{
    int     optlevel = 0,
		option = 0,
		rc;
	
    rc = NO_ERROR;
    if (gAddressFamily == AF_INET)
    {
        optlevel = IPPROTO_IP;
        option   = IP_TTL;
    }
    else if (gAddressFamily == AF_INET6)
    {
        optlevel = IPPROTO_IPV6;
        option   = IPV6_UNICAST_HOPS;
    }
    else
    {
        rc = SOCKET_ERROR;
    }
    if (rc == NO_ERROR)
    {
        rc = setsockopt(
			s,
			optlevel,
			option,
			(char *)&ttl,
			sizeof(ttl)
			);
    }
    if (rc == SOCKET_ERROR)
    {
        fprintf(stderr, "SetTtl: setsockopt failed: %d\n", WSAGetLastError());
    }
    return rc;
}


// 9
int __cdecl main(int argc, char **argv)
{
    WSADATA            wsd;
    WSAOVERLAPPED      recvol;        // 重叠 IO
    SOCKET             s=INVALID_SOCKET;
    char              *icmpbuf=NULL;
    struct addrinfo   *dest=NULL,
                      *local=NULL;
    IPV4_OPTION_HDR    ipopt;
    SOCKADDR_STORAGE   from;          // socket地址存储结构（9.1）
    DWORD              bytes,
                       flags;
    int                packetlen=0,
                       fromlen,
                       time=0,
                       rc,
                       i,
                       status = 0;
 
    recvol.hEvent = WSA_INVALID_EVENT;
 
    //  分析输入参数
    if (ValidateArgs(argc, argv) == FALSE)
    {
        status = -1;
        goto EXIT;
    }
 
    // socket模块启动初始化
    if ((rc = WSAStartup(MAKEWORD(2,2), &wsd)) != 0)
    {
        printf("WSAStartup() failed: %d\n", rc);
        status = -1;
        goto EXIT;
    }
 
    //  解析目的地址
    dest = ResolveAddress(
            gDestination,
            "0",
            gAddressFamily,
            0,
            0
            );
    if (dest == NULL)
    {
        printf("bad name %s\n", gDestination);
        status = -1;
        goto CLEANUP;
    }
    gAddressFamily = dest->ai_family;
 
    if (gAddressFamily == AF_INET)
        gProtocol = IPPROTO_ICMP;
    else if (gAddressFamily == AF_INET6)
        gProtocol = IPPROTO_ICMP6;
 
    // 获得本地地址，绑定使用
    local = ResolveAddress(
            NULL,
            "0",
            gAddressFamily,
            0,
            0
            );
    if (local == NULL)
    {
        printf("Unable to obtain the bind address!\n");
        status = -1;
        goto CLEANUP;
    }
 
    //创建Raw套接字，protocol = IPPROTO_ICMP
    s = socket(gAddressFamily, SOCK_RAW, gProtocol);           
    if (s == INVALID_SOCKET)
    {
        printf("socket failed: %d\n", WSAGetLastError());
        status = -1;
        goto CLEANUP;
    }
 
    SetTtl(s, gTtl); //设置最大跳站数为128
 
    if (gAddressFamily == AF_INET)
        packetlen += sizeof(ICMP_HDR);
    else if (gAddressFamily == AF_INET6)
        packetlen += sizeof(ICMPV6_HDR) + sizeof(ICMPV6_ECHO_REQUEST);
 
    // packetlen 为  数据长度+ICMP头部长度
    packetlen += gDataSize;
 
    // 分配空间存储ICMP请求(9.2)
    icmpbuf = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, packetlen);
    if (icmpbuf == NULL)
    {
        fprintf(stderr, "HeapAlloc failed: %d\n", GetLastError());
        status = -1;
        goto CLEANUP;
    }
 
    // 初始化 ICMP 头部
    if (gAddressFamily == AF_INET)
    {
        if (bRecordRoute) // 如有路由记录功能，初始化选项数据段
        {
            ZeroMemory(&ipopt, sizeof(ipopt));
            ipopt.opt_code = IP_RECORD_ROUTE; // 路由记录选项
            ipopt.opt_ptr  = 4;               // 指向可用的地址，每个地址占用4字节，此指针指向第一个可用的存储偏移地址
            ipopt.opt_len  = 39;              // 选项数据段长度
 
            rc = setsockopt(s, IPPROTO_IP, IP_OPTIONS,
                    (char *)&ipopt, sizeof(ipopt));
            if (rc == SOCKET_ERROR)
            {
                fprintf(stderr, "setsockopt(IP_OPTIONS) failed: %d\n", WSAGetLastError());
                status = -1;
                goto CLEANUP;
            }
        }
 
        InitIcmpHeader(icmpbuf, gDataSize);
    }
    else if (gAddressFamily == AF_INET6)
    {
        InitIcmp6Header(icmpbuf, gDataSize);
    }
 
    // 绑定地址，此套接字可获得外部单元向这个地址发送的数据
    rc = bind(s, local->ai_addr, (int)local->ai_addrlen);
    if (rc == SOCKET_ERROR)
    {
        fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
        status = -1;
        goto CLEANUP;
    }
 
    // 建立接收操作
    memset(&recvol, 0, sizeof(recvol));
    recvol.hEvent = WSACreateEvent();    // 事件初始化
    if (recvol.hEvent == WSA_INVALID_EVENT)
    {
        fprintf(stderr, "WSACreateEvent failed: %d\n", WSAGetLastError());
        status = -1;
        goto CLEANUP;
    }
 
    //  异步接收
    fromlen = sizeof(from);
    PostRecvfrom(s, recvbuf, recvbuflen, (SOCKADDR *)&from, &fromlen, &recvol);
 
    printf("\nPinging ");
    PrintAddress(dest->ai_addr, (int)dest->ai_addrlen);
    printf(" with %d bytes of data\n\n", gDataSize);

    // ping 4 次
    for(i=0; i < DEFAULT_SEND_COUNT ;i++)
    {
        // 设置序列号并计算校验和
        SetIcmpSequence(icmpbuf);
        ComputeIcmpChecksum(s, icmpbuf, packetlen, dest);
 
        time = GetTickCount();
        rc = sendto(      // 发送icmp请求
                s,
                icmpbuf,
                packetlen,
                0,
                dest->ai_addr,
                (int)dest->ai_addrlen
                );
        if (rc == SOCKET_ERROR)
        {
            fprintf(stderr, "sendto failed: %d\n", WSAGetLastError());
            status = -1;
            goto CLEANUP;
        }
 
        // 等待ICMP回复（9.3）
        rc = WaitForSingleObject((HANDLE)recvol.hEvent, DEFAULT_RECV_TIMEOUT);
        if (rc == WAIT_FAILED)
        {
            fprintf(stderr, "WaitForSingleObject failed: %d\n", GetLastError());
            status = -1;
            goto CLEANUP;
        }
        else if (rc == WAIT_TIMEOUT)
        {
            printf("Request timed out.\n");
        }
        else
        {
                   // 收到ICMP回复
            rc = WSAGetOverlappedResult(
                   s,
                   &recvol,
                   &bytes,
                    FALSE,
                   &flags
                    );
            if (rc == FALSE)
            {
                fprintf(stderr, "WSAGetOverlappedResult failed: %d\n", WSAGetLastError());
            }
            time = GetTickCount() - time;
 
            WSAResetEvent(recvol.hEvent);
 
            printf("Reply from ");
            PrintAddress((SOCKADDR *)&from, fromlen);
            if (time == 0)
                printf(": bytes=%d time<1ms TTL=%d\n", gDataSize, gTtl);
            else
                printf(": bytes=%d time=%dms TTL=%d\n", gDataSize, time, gTtl);
 
            PrintPayload(recvbuf, bytes);
 
            if (i < DEFAULT_SEND_COUNT - 1)
            {
                fromlen = sizeof(from);
                PostRecvfrom(s, recvbuf, recvbuflen, (SOCKADDR *)&from, &fromlen, &recvol);
            }
        }
        Sleep(1000);
    }
 
CLEANUP:
   
    if (dest)
         freeaddrinfo(dest);
    if (local)
         freeaddrinfo(local);
    if (s != INVALID_SOCKET)
        closesocket(s);
    if (recvol.hEvent != WSA_INVALID_EVENT)
        WSACloseEvent(recvol.hEvent);
    if (icmpbuf)
        HeapFree(GetProcessHeap(), 0, icmpbuf);
 
    WSACleanup();
 
EXIT:
 
    system("pause");
 
    return status;
}