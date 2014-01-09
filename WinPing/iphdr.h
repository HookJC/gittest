
#ifndef __IP_HDR_H__
#define __IP_HDR_H__

//-----------------------iphdr.h-----------------------//
//源码分析将忽略ipv6

//边界对齐至字节
#include <pshpack1.h>

// 1 -- ipv4 头部
typedef struct ip_hdr
{
    unsigned char  ip_verlen;       // 前4位IP版本号(IPv4 或者IPv6)
	// 后4位头部长度(32位，4字节)（1.1）
    unsigned char  ip_tos;          // 前3位为优先级，后5位为服务类型（1.2）
    unsigned short ip_totallength;  // 16位包总长度包括头部和数据(字节)（1.3）
    unsigned short ip_id;           // 16位ID标识
    unsigned short ip_offset;       // 前3位为分段标识，后5位为分段偏移
    unsigned char  ip_ttl;          // 该包可经过的路由器数量上限
    unsigned char  ip_protocol;     // 协议类型（TCP，UDP，ICMP，IGMP等）
    unsigned short ip_checksum;     // ipv4 头部的校验和
    unsigned int   ip_srcaddr;      // ipv4 源地址
    unsigned int   ip_destaddr;     // ipv4 目的地址
} IPV4_HDR, *PIPV4_HDR, FAR * LPIPV4_HDR;

// 2 -- ipv4 选项头部
typedef struct ipv4_option_hdr
{
    unsigned char   opt_code;          // ipv4 选项头类型
    unsigned char   opt_len;           // ipv4 选项头长度
    unsigned char   opt_ptr;           // ipv4 选项头指针
    unsigned long   opt_addr[9];       // ipv4 9个地址列表(2.1)
} IPV4_OPTION_HDR, *PIPV4_OPTION_HDR, FAR *LPIPV4_OPTION_HDR;

// 3 -- icmp 头部
typedef struct icmp_hdr
{
    unsigned char   icmp_type;            // icmp 类型
    unsigned char   icmp_code;            // ipv4 码
    unsigned short  icmp_checksum;        // icmp 头部及数据校验和
    unsigned short  icmp_id;              // icmp id标识(3.1)
    unsigned short  icmp_sequence;        // icmp 序列号，请求回应消息对
} ICMP_HDR, *PICMP_HDR, FAR *LPICMP_HDR;

// 4 -- udp 头部（此头部未在程序中用到）
typedef struct udp_hdr
{
    unsigned short src_portno;            // 源端口
    unsigned short dst_portno;            // 目的端口
    unsigned short udp_length;            // udp 包总长度(字节)
    unsigned short udp_checksum;          // udp 头部以及数据校验和(4.1)
} UDP_HDR, *PUDP_HDR;

// 5 -- ipv4  路径记录宏
#define IP_RECORD_ROUTE     0x7            

// icmp 类型和码(5.1)
#define ICMPV4_ECHO_REQUEST_TYPE   8        // icmp  回显请求类型
#define ICMPV4_ECHO_REQUEST_CODE   0        // icmp  回显请求码
#define ICMPV4_ECHO_REPLY_TYPE     0        // icmp  回显回应类型
#define ICMPV4_ECHO_REPLY_CODE     0        // icmp  回显回应码
#define ICMPV4_MINIMUM_HEADER      8        // icmp  最小头部

// 恢复默认对齐方式
#include <poppack.h>

#endif
