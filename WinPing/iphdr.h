
#ifndef __IP_HDR_H__
#define __IP_HDR_H__

//-----------------------iphdr.h-----------------------//
//Դ�����������ipv6

//�߽�������ֽ�
#include <pshpack1.h>

// 1 -- ipv4 ͷ��
typedef struct ip_hdr
{
    unsigned char  ip_verlen;       // ǰ4λIP�汾��(IPv4 ����IPv6)
	// ��4λͷ������(32λ��4�ֽ�)��1.1��
    unsigned char  ip_tos;          // ǰ3λΪ���ȼ�����5λΪ�������ͣ�1.2��
    unsigned short ip_totallength;  // 16λ���ܳ��Ȱ���ͷ��������(�ֽ�)��1.3��
    unsigned short ip_id;           // 16λID��ʶ
    unsigned short ip_offset;       // ǰ3λΪ�ֶα�ʶ����5λΪ�ֶ�ƫ��
    unsigned char  ip_ttl;          // �ð��ɾ�����·������������
    unsigned char  ip_protocol;     // Э�����ͣ�TCP��UDP��ICMP��IGMP�ȣ�
    unsigned short ip_checksum;     // ipv4 ͷ����У���
    unsigned int   ip_srcaddr;      // ipv4 Դ��ַ
    unsigned int   ip_destaddr;     // ipv4 Ŀ�ĵ�ַ
} IPV4_HDR, *PIPV4_HDR, FAR * LPIPV4_HDR;

// 2 -- ipv4 ѡ��ͷ��
typedef struct ipv4_option_hdr
{
    unsigned char   opt_code;          // ipv4 ѡ��ͷ����
    unsigned char   opt_len;           // ipv4 ѡ��ͷ����
    unsigned char   opt_ptr;           // ipv4 ѡ��ͷָ��
    unsigned long   opt_addr[9];       // ipv4 9����ַ�б�(2.1)
} IPV4_OPTION_HDR, *PIPV4_OPTION_HDR, FAR *LPIPV4_OPTION_HDR;

// 3 -- icmp ͷ��
typedef struct icmp_hdr
{
    unsigned char   icmp_type;            // icmp ����
    unsigned char   icmp_code;            // ipv4 ��
    unsigned short  icmp_checksum;        // icmp ͷ��������У���
    unsigned short  icmp_id;              // icmp id��ʶ(3.1)
    unsigned short  icmp_sequence;        // icmp ���кţ������Ӧ��Ϣ��
} ICMP_HDR, *PICMP_HDR, FAR *LPICMP_HDR;

// 4 -- udp ͷ������ͷ��δ�ڳ������õ���
typedef struct udp_hdr
{
    unsigned short src_portno;            // Դ�˿�
    unsigned short dst_portno;            // Ŀ�Ķ˿�
    unsigned short udp_length;            // udp ���ܳ���(�ֽ�)
    unsigned short udp_checksum;          // udp ͷ���Լ�����У���(4.1)
} UDP_HDR, *PUDP_HDR;

// 5 -- ipv4  ·����¼��
#define IP_RECORD_ROUTE     0x7            

// icmp ���ͺ���(5.1)
#define ICMPV4_ECHO_REQUEST_TYPE   8        // icmp  ������������
#define ICMPV4_ECHO_REQUEST_CODE   0        // icmp  ����������
#define ICMPV4_ECHO_REPLY_TYPE     0        // icmp  ���Ի�Ӧ����
#define ICMPV4_ECHO_REPLY_CODE     0        // icmp  ���Ի�Ӧ��
#define ICMPV4_MINIMUM_HEADER      8        // icmp  ��Сͷ��

// �ָ�Ĭ�϶��뷽ʽ
#include <poppack.h>

#endif
