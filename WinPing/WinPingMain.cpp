
#include <stdio.h>

// 64λ�ܹ�
#ifdef _IA64_
#pragma warning (disable: 4267)
#endif
// �˺궨��ʹwindows.h�޳�����ͷ�ļ����ӿ�����ٶ�
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include "resolve.h"
#include "iphdr.h"

#define DEFAULT_DATA_SIZE      32       // icmp ���ݶδ�С
#define DEFAULT_SEND_COUNT     4        // ping �Ĵ���
#define DEFAULT_RECV_TIMEOUT   6000     // ���ճ�ʱ 
#define DEFAULT_TTL            128      // �����վ��
#define MAX_RECV_BUF_LEN       0xFFFF   // �����ջ�������С

int   gAddressFamily=AF_UNSPEC,        
gProtocol=IPPROTO_ICMP,          
gTtl=DEFAULT_TTL;                
int   gDataSize=DEFAULT_DATA_SIZE;     
BOOL  bRecordRoute=FALSE;               //  �Ƿ��¼·��
char *gDestination=NULL,               
recvbuf[MAX_RECV_BUF_LEN];       
int   recvbuflen = MAX_RECV_BUF_LEN; 

/************************************************************************/
// struct 


/************************************************************************/


//  ping ����ʹ�÷���
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
//  ��ʼ��icmpͷ��
void InitIcmpHeader(char *buf, int datasize)
{
    ICMP_HDR   *icmp_hdr=NULL;
    char       *datapart=NULL;
    //  ���icmpͷ������
    icmp_hdr = (ICMP_HDR *)buf;
    icmp_hdr->icmp_type     = ICMPV4_ECHO_REQUEST_TYPE;  
    icmp_hdr->icmp_code     = ICMPV4_ECHO_REQUEST_CODE;
    icmp_hdr->icmp_id       = (USHORT)GetCurrentProcessId();  //���̺�
    icmp_hdr->icmp_checksum = 0;   //  ���к�δ���壬У���δ����
    icmp_hdr->icmp_sequence = 0;   //  ���к��ÿ�
 
    datapart = buf + sizeof(ICMP_HDR);   //  ָ���������ݶ�ͷ
    memset(datapart, 'E', datasize);     //  ������ݶ�
}
// ����У���
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
// ���ν���
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
                case 'i':        // ���������ֵ
                    if (i+1 >= argc)
                    {
                        usage(argv[0]);
                        goto CLEANUP;
                    }
 
                    gTtl = atoi(argv[++i]);
                    break;
                case 'l':        //  icmp���ݶδ�С����
                    if (i+1 >= argc)
                    {
                        usage(argv[0]);
                        goto CLEANUP;
                    }
 
                    gDataSize = atoi(argv[++i]);
                    break;
                case 'r':        //  ��¼·��ѡ��
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
//  ����icmp���к�
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
//  ����icmpУ���
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
//  �����첽����
int PostRecvfrom(SOCKET s, char *buf, int buflen, SOCKADDR *from, int *fromlen, WSAOVERLAPPED *ol)
{
    WSABUF  wbuf;
    DWORD   flags,
            bytes;
    int     rc;
 
    wbuf.buf = buf;
    wbuf.len = buflen;
 
    flags = 0;
 
    rc = WSARecvFrom(         //  ͨ���ص�IOʵ���첽����
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
        hdrlen = (v4hdr->ip_verlen & 0x0F) * 4;  // ����IPͷ������(8.1)
		
        if (hdrlen > sizeof(IPV4_HDR))   // ������ȴ�����ѡ��IPͷ������
        {
			// ѡ��ͷ��ָ��
            v4opt = (IPV4_OPTION_HDR *)(buf + sizeof(IPV4_HDR));
			//����·����(8.2)
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

// ���������վ��
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
    WSAOVERLAPPED      recvol;        // �ص� IO
    SOCKET             s=INVALID_SOCKET;
    char              *icmpbuf=NULL;
    struct addrinfo   *dest=NULL,
                      *local=NULL;
    IPV4_OPTION_HDR    ipopt;
    SOCKADDR_STORAGE   from;          // socket��ַ�洢�ṹ��9.1��
    DWORD              bytes,
                       flags;
    int                packetlen=0,
                       fromlen,
                       time=0,
                       rc,
                       i,
                       status = 0;
 
    recvol.hEvent = WSA_INVALID_EVENT;
 
    //  �����������
    if (ValidateArgs(argc, argv) == FALSE)
    {
        status = -1;
        goto EXIT;
    }
 
    // socketģ��������ʼ��
    if ((rc = WSAStartup(MAKEWORD(2,2), &wsd)) != 0)
    {
        printf("WSAStartup() failed: %d\n", rc);
        status = -1;
        goto EXIT;
    }
 
    //  ����Ŀ�ĵ�ַ
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
 
    // ��ñ��ص�ַ����ʹ��
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
 
    //����Raw�׽��֣�protocol = IPPROTO_ICMP
    s = socket(gAddressFamily, SOCK_RAW, gProtocol);           
    if (s == INVALID_SOCKET)
    {
        printf("socket failed: %d\n", WSAGetLastError());
        status = -1;
        goto CLEANUP;
    }
 
    SetTtl(s, gTtl); //���������վ��Ϊ128
 
    if (gAddressFamily == AF_INET)
        packetlen += sizeof(ICMP_HDR);
    else if (gAddressFamily == AF_INET6)
        packetlen += sizeof(ICMPV6_HDR) + sizeof(ICMPV6_ECHO_REQUEST);
 
    // packetlen Ϊ  ���ݳ���+ICMPͷ������
    packetlen += gDataSize;
 
    // ����ռ�洢ICMP����(9.2)
    icmpbuf = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, packetlen);
    if (icmpbuf == NULL)
    {
        fprintf(stderr, "HeapAlloc failed: %d\n", GetLastError());
        status = -1;
        goto CLEANUP;
    }
 
    // ��ʼ�� ICMP ͷ��
    if (gAddressFamily == AF_INET)
    {
        if (bRecordRoute) // ����·�ɼ�¼���ܣ���ʼ��ѡ�����ݶ�
        {
            ZeroMemory(&ipopt, sizeof(ipopt));
            ipopt.opt_code = IP_RECORD_ROUTE; // ·�ɼ�¼ѡ��
            ipopt.opt_ptr  = 4;               // ָ����õĵ�ַ��ÿ����ַռ��4�ֽڣ���ָ��ָ���һ�����õĴ洢ƫ�Ƶ�ַ
            ipopt.opt_len  = 39;              // ѡ�����ݶγ���
 
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
 
    // �󶨵�ַ�����׽��ֿɻ���ⲿ��Ԫ�������ַ���͵�����
    rc = bind(s, local->ai_addr, (int)local->ai_addrlen);
    if (rc == SOCKET_ERROR)
    {
        fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
        status = -1;
        goto CLEANUP;
    }
 
    // �������ղ���
    memset(&recvol, 0, sizeof(recvol));
    recvol.hEvent = WSACreateEvent();    // �¼���ʼ��
    if (recvol.hEvent == WSA_INVALID_EVENT)
    {
        fprintf(stderr, "WSACreateEvent failed: %d\n", WSAGetLastError());
        status = -1;
        goto CLEANUP;
    }
 
    //  �첽����
    fromlen = sizeof(from);
    PostRecvfrom(s, recvbuf, recvbuflen, (SOCKADDR *)&from, &fromlen, &recvol);
 
    printf("\nPinging ");
    PrintAddress(dest->ai_addr, (int)dest->ai_addrlen);
    printf(" with %d bytes of data\n\n", gDataSize);

    // ping 4 ��
    for(i=0; i < DEFAULT_SEND_COUNT ;i++)
    {
        // �������кŲ�����У���
        SetIcmpSequence(icmpbuf);
        ComputeIcmpChecksum(s, icmpbuf, packetlen, dest);
 
        time = GetTickCount();
        rc = sendto(      // ����icmp����
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
 
        // �ȴ�ICMP�ظ���9.3��
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
                   // �յ�ICMP�ظ�
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