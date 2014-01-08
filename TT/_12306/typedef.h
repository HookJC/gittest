
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#pragma warning(disable:4786)
#pragma warning(disable:4503)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _DEBUG_ 0 // ���Դ�ӡ����

#define ERROR_INFO_LEN 1024

#define MAX_ACK_RSP_LEN 102400
#define MAX_ACK_REQ_LEN 10240

#define MAX_VERTIFY_IMG_SIZE 4096

#define MAX_PET_FRIENT	500 // ������
#define MAX_PET_FACTION	100 // ���ɳ�Ա
#define MAX_PET_LILIAN	20 // ��������
#define MAX_NAME_LEN 20 // ���Ƴ���

#define MAX_BROTHRE_COUNT 4 // �ֵ���
#define MAX_APPRENTICE_COUNT 4 // ͽ����

#define MAX_LIANYAO_DYAO 3 // ���Ƶ�ҩ��

#define MAX_MANOR_COUNT 10 // �����б���

#define MAX_MERID_COUNT 10 // ������
#define MAX_INTF_COUNT 20 // ������

#define MAX_INTF_MASTER 5 // ������ʦ��

#define MAX_ATTENTION_COUNT 100 //���ɹ�ע��
 
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

/************************************************************************/
//                          Q����ֶ�I
/************************************************************************/
// ������Ϣ 
typedef struct _PET_PERSON_INFO
{
	unsigned int level; // �ȼ� 
	unsigned int uin;	 // QQ�� 
	char	szname[MAX_NAME_LEN + 1]; // name
}PET_PERSON_INFO,* LP_PET_PERSON_INFO;

// ������ϸ��Ϣ 
typedef struct _PET_PERSON_DETAIL_INFO
{
	unsigned short level; // �ȼ�
	unsigned short bmarray;
	unsigned short bmaster;
	ushort usmanorid; // ����
	unsigned short ustudi;
	unsigned short usbrother;
	unsigned int uin;	 // QQ�� 
	int ifaction; // ����id 
	char szfactionname[MAX_NAME_LEN + 1]; // �������� 
	char	szname[MAX_NAME_LEN + 1]; // name
	char szmanorname[MAX_NAME_LEN + 1];

 	PET_PERSON_INFO marray; // ���� 
 	PET_PERSON_INFO master; // ʦ�� 
 	PET_PERSON_INFO apprentice[MAX_APPRENTICE_COUNT]; // ͽ�� 
	PET_PERSON_INFO brother[MAX_BROTHRE_COUNT]; // �ֵ�	 
	
}PET_PERSON_DETAIL_INFO,* LP_PET_PERSON_DETAIL_INFO;


// �����б� 
typedef struct _PET_FRIEND_LIST
{
	unsigned short uscount;
	PET_PERSON_INFO frinedlist[MAX_PET_FRIENT + 1];	
}PET_FRIEND_LIST,* LP_PET_FRIEND_LIST;

// ���ɳ�Ա�б� 
typedef struct _PET_FACTION_LIST
{
	unsigned short uscount;
	PET_PERSON_INFO frinelist[MAX_PET_FACTION + 1];	
}PET_FACTION_LIST,* LP_PET_FACTION_LIST;

// ���� 
typedef struct _PET_LILIAN_INFO
{	
	unsigned int uid;	// id 
	int	istatus; // ״̬ 
}PET_LILIAN_INFO,* LP_PET_LILIAN_INFO; 

// �����б� 
typedef struct _PET_LILIAN_LIST
{
	unsigned short uscount;
	unsigned short uscurid;
	unsigned short usmaxid;
	unsigned short usmapid;
	PET_LILIAN_INFO lilianlist[MAX_PET_LILIAN];	
}PET_LILIAN_LIST,* LP_PET_LILIAN_LIST;

// ��ҩ
typedef struct _PET_ALCHE_DYAO
{
	ushort uid;
	ushort ustimes; // ��ʱ��λ��s
	ushort usexp;
	char szname[MAX_NAME_LEN + 1]; 
}PET_ALCHE_DYAO,* LP_PET_ALCHE_DYAO;

typedef struct _PET_DYAO_LIST
{
	ushort uscount;
	ushort usmaxexpidx;
	PET_ALCHE_DYAO	dyaolist[MAX_LIANYAO_DYAO];
}PET_DYAO_LIST,* LP_PET_DYAO_LIST;

// ��¯��Ϣ 
typedef struct _PET_ALCHE_INFO
{
	ushort busing;
	ushort ustimes;
	ushort usmaxtimes;
	ushort usstrongtimes;
	ushort usmaxstrongtimes;
	ushort uslevel;
	ushort usliandan;
	PET_ALCHE_DYAO dyaolist[MAX_LIANYAO_DYAO]; // ���Ƶ�ҩ��Ϣ	
}PET_ALCHE_INFO,* LP_PET_ALCHE_INFO;
 
// ����
typedef struct _PET_MANOR_OWNERINFO
{
	uint uid;
	ushort ulevel;
	ushort usinvadetimes;
	ushort usownerlevel;
	
	char szname[MAX_NAME_LEN + 1];
	char szownername[MAX_NAME_LEN + 1];	
}PET_MANOR_OWNERINFO,* LP_PET_MANOR_OWNERINFO;

typedef struct _PET_MANORS_LIST
{
	ushort uscount;
	ushort usminlvlidx;
	PET_MANOR_OWNERINFO manorlist[MAX_MANOR_COUNT];
}PET_MANORS_LIST,* LP_PET_MANORS_LIST;

// ���˾���
typedef struct _PET_INTFMASTER_INFO // ������ʦ��Ϣ
{
	ushort uid;
	ushort uscont; // ���Ĵ�����
	char szname[MAX_NAME_LEN + 1];
	char szdes[2 * MAX_NAME_LEN + 1];	
}PET_INTFMASTER_INFO,* LP_PET_INTFMASTER_INFO;

typedef struct _PET_INTFMASTER_LIST
{
	ushort uscount;
	ushort usfree; // ��ѻ�ȡ����
	ushort uscgfcount; // ����������
	ushort usgetintfid; // ��ȡ������ʦid
	PET_INTFMASTER_INFO masters[MAX_INTF_MASTER];
}PET_INTFMASTER_LIST,* LP_PET_INTFMASTER_LIST;

typedef struct _PET_MERID_INFO // ������Ϣ
{	
	ushort uid; // ����id
	ushort ustype; // �������� 1�� 2��
	ushort usintfid; // ����id
	ushort uslevel; // �����ȼ�

	char szmeridname[MAX_NAME_LEN + 1]; //����������
	char szintfname[MAX_NAME_LEN + 1]; // ��������
}PET_MERID_INFO,* LP_PET_MERID_INFO;


typedef struct _PET_INTF_INFO // �������е�����
{
	ushort ustype; // ���� 1�� 2��
	ushort uid; // ����id
	ushort uslevel; // �����ȼ�
	ushort uscult; // ��Ϊ
	char szname[MAX_NAME_LEN + 1]; // ��������
}PET_INTF_INFO,* LP_PET_INTF_INFO;

typedef struct _PET_INTF_LIST // �������е�����
{
	ushort uscount;
	PET_INTF_INFO intflist[MAX_INTF_COUNT]; // �����б�
}PET_INTF_LIST,* LP_PET_INTF_LIST;

typedef struct _PET_INTFMERID_LIST // ���������б�
{
	ushort usmeridcount; // ��������
	ushort usbagcount; // ���������
	ushort usintfcount; // ����������������
	PET_MERID_INFO meridlist[MAX_MERID_COUNT]; // �����б�
	PET_INTF_INFO intflist[MAX_INTF_COUNT]; // �����б�
}PET_INTF_MERID_LIST,* LP_PET_INTFMERID_LIST;

/************************************************************************/
//                        �ٶ�����ǩ��                                                                     
/************************************************************************/
typedef struct _WAP_BAIDU_ATTENTION
{
	ushort uslevel;								// ͷ�εȼ�
	ushort uscurexp;							// ��ǰ����
	uint uid;
	char szname[MAX_NAME_LEN + 1];				// ��������
	char sznickname[MAX_NAME_LEN + 1];			// ͷ�γƺ�
}WAP_BAIDU_ATTENTION,* LP_WAP_BAIDU_ATTENTION;

typedef struct _WAP_BAIDU_ATTENTION_LIST
{
	ushort uscount;											// ��ע������
	char sztbs[2 * MAX_NAME_LEN + 1];
	WAP_BAIDU_ATTENTION stattention[MAX_ATTENTION_COUNT];	// �����б�
}WAP_BAIDU_ATTENTION_LIST,* LP_WAP_BAIDU_ATTENTION_LIST;

/************************************************************************/
//                           12306��Ʊ
/************************************************************************/

#define MAX_TRAIN_NO 5						// ����
#define MAX_CITY_LEN 10						// վ��
#define MAX_TIME_LEN 8						// ʱ��
#define TRAIN_TICKET_TYPE 11				// Ʊ������
#define TICKET_SIGN_LEN 5					// Ʊ����Ϣ�� ��,1,��,--
#define SUBMIT_CODE_LEN 300					// Ԥ���ύ����
#define MAX_LOAD_DATA 50					// һ�β�ѯ��������

#define MAX_TRAIN_PASSENGER 20
#define MAX_PASSENGER_NAME 10
#define MAX_PASSENGER_IDNO 35
#define MAX_MOBILE_LEN 11
#define MAX_PHONE_LEN 15

typedef struct _QUERY_DATA_TRAIN
{
	ushort bsubmit;							// �ܷ�Ԥ��
	ushort bstart;							// ʼ��վ
	ushort bend;							// �յ�վ
	char sztrain_nos[MAX_PHONE_LEN + 1];	// ���δ���
	char sztrain_no[MAX_TRAIN_NO + 1];		// ���� 
	char szstartcity[MAX_CITY_LEN + 1];		// ��վ
	char szstarttime[MAX_TIME_LEN + 1];		// ��վʱ��
	char szendcity[MAX_CITY_LEN + 1];		// ��վ
	char szendtime[MAX_TIME_LEN + 1];		// ��վʱ��
	char szusetime[MAX_TIME_LEN + 1];		// ��ʱ
	char szTicket[TRAIN_TICKET_TYPE][TICKET_SIGN_LEN + 1]; // ��Ʊ����Ϣ
	char szsubmitcode[SUBMIT_CODE_LEN + 1];	// �ɷ�Ʊ����Ʊ����
}QUERY_DATA_TRAIN,* LP_QUERY_DATA_TRAIN;

typedef struct _QUERY_DATA_LIST
{
	ushort uscount;							// �г�����
	QUERY_DATA_TRAIN stlist[MAX_LOAD_DATA]; // �г���Ϣ
}QUERY_DATA_LIST,* LP_QUERY_DATA_LIST;

typedef struct _TRAIN_PASSENGER_INFO
{
	ushort ustype;							// �˿����ͣ�����|��ͯ|ѧ��|�о�
	char szname[MAX_PASSENGER_NAME + 1];	// ����
	char szidtype[MAX_TRAIN_NO + 1];		// ֤������
	char szidno[MAX_PASSENGER_IDNO + 1];	// ֤����
	char szmobile[MAX_MOBILE_LEN + 1];		// �ֻ���
}TRAIN_PASSENGER_INFO,* LP_TRAIN_PASSENGER_INFO;

typedef struct _TRAIN_PASSENGER_LIST
{
	ushort uscount;
	TRAIN_PASSENGER_INFO stpassenger[MAX_TRAIN_PASSENGER];
}TRAIN_PASSENGER_LIST,* LP_TRAIN_PASSENGER_LIST;

#endif
