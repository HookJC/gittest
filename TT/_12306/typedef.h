
#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#pragma warning(disable:4786)
#pragma warning(disable:4503)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _DEBUG_ 0 // 调试打印开关

#define ERROR_INFO_LEN 1024

#define MAX_ACK_RSP_LEN 102400
#define MAX_ACK_REQ_LEN 10240

#define MAX_VERTIFY_IMG_SIZE 4096

#define MAX_PET_FRIENT	500 // 好友数
#define MAX_PET_FACTION	100 // 帮派成员
#define MAX_PET_LILIAN	20 // 历练个数
#define MAX_NAME_LEN 20 // 名称长度

#define MAX_BROTHRE_COUNT 4 // 兄弟数
#define MAX_APPRENTICE_COUNT 4 // 徒弟数

#define MAX_LIANYAO_DYAO 3 // 炼制丹药数

#define MAX_MANOR_COUNT 10 // 地盘列表数

#define MAX_MERID_COUNT 10 // 经脉数
#define MAX_INTF_COUNT 20 // 内力箱

#define MAX_INTF_MASTER 5 // 内力大师数

#define MAX_ATTENTION_COUNT 100 //贴吧关注数
 
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

/************************************************************************/
//                          Q宠大乐斗I
/************************************************************************/
// 个人信息 
typedef struct _PET_PERSON_INFO
{
	unsigned int level; // 等级 
	unsigned int uin;	 // QQ号 
	char	szname[MAX_NAME_LEN + 1]; // name
}PET_PERSON_INFO,* LP_PET_PERSON_INFO;

// 个人详细信息 
typedef struct _PET_PERSON_DETAIL_INFO
{
	unsigned short level; // 等级
	unsigned short bmarray;
	unsigned short bmaster;
	ushort usmanorid; // 地盘
	unsigned short ustudi;
	unsigned short usbrother;
	unsigned int uin;	 // QQ号 
	int ifaction; // 帮派id 
	char szfactionname[MAX_NAME_LEN + 1]; // 帮派名称 
	char	szname[MAX_NAME_LEN + 1]; // name
	char szmanorname[MAX_NAME_LEN + 1];

 	PET_PERSON_INFO marray; // 对象 
 	PET_PERSON_INFO master; // 师傅 
 	PET_PERSON_INFO apprentice[MAX_APPRENTICE_COUNT]; // 徒弟 
	PET_PERSON_INFO brother[MAX_BROTHRE_COUNT]; // 兄弟	 
	
}PET_PERSON_DETAIL_INFO,* LP_PET_PERSON_DETAIL_INFO;


// 好友列表 
typedef struct _PET_FRIEND_LIST
{
	unsigned short uscount;
	PET_PERSON_INFO frinedlist[MAX_PET_FRIENT + 1];	
}PET_FRIEND_LIST,* LP_PET_FRIEND_LIST;

// 帮派成员列表 
typedef struct _PET_FACTION_LIST
{
	unsigned short uscount;
	PET_PERSON_INFO frinelist[MAX_PET_FACTION + 1];	
}PET_FACTION_LIST,* LP_PET_FACTION_LIST;

// 历练 
typedef struct _PET_LILIAN_INFO
{	
	unsigned int uid;	// id 
	int	istatus; // 状态 
}PET_LILIAN_INFO,* LP_PET_LILIAN_INFO; 

// 历练列表 
typedef struct _PET_LILIAN_LIST
{
	unsigned short uscount;
	unsigned short uscurid;
	unsigned short usmaxid;
	unsigned short usmapid;
	PET_LILIAN_INFO lilianlist[MAX_PET_LILIAN];	
}PET_LILIAN_LIST,* LP_PET_LILIAN_LIST;

// 丹药
typedef struct _PET_ALCHE_DYAO
{
	ushort uid;
	ushort ustimes; // 耗时单位：s
	ushort usexp;
	char szname[MAX_NAME_LEN + 1]; 
}PET_ALCHE_DYAO,* LP_PET_ALCHE_DYAO;

typedef struct _PET_DYAO_LIST
{
	ushort uscount;
	ushort usmaxexpidx;
	PET_ALCHE_DYAO	dyaolist[MAX_LIANYAO_DYAO];
}PET_DYAO_LIST,* LP_PET_DYAO_LIST;

// 丹炉信息 
typedef struct _PET_ALCHE_INFO
{
	ushort busing;
	ushort ustimes;
	ushort usmaxtimes;
	ushort usstrongtimes;
	ushort usmaxstrongtimes;
	ushort uslevel;
	ushort usliandan;
	PET_ALCHE_DYAO dyaolist[MAX_LIANYAO_DYAO]; // 练制丹药信息	
}PET_ALCHE_INFO,* LP_PET_ALCHE_INFO;
 
// 地盘
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

// 个人经脉
typedef struct _PET_INTFMASTER_INFO // 内力大师信息
{
	ushort uid;
	ushort uscont; // 消耗传功符
	char szname[MAX_NAME_LEN + 1];
	char szdes[2 * MAX_NAME_LEN + 1];	
}PET_INTFMASTER_INFO,* LP_PET_INTFMASTER_INFO;

typedef struct _PET_INTFMASTER_LIST
{
	ushort uscount;
	ushort usfree; // 免费获取次数
	ushort uscgfcount; // 传功符数量
	ushort usgetintfid; // 获取内力大师id
	PET_INTFMASTER_INFO masters[MAX_INTF_MASTER];
}PET_INTFMASTER_LIST,* LP_PET_INTFMASTER_LIST;

typedef struct _PET_MERID_INFO // 经脉信息
{	
	ushort uid; // 经脉id
	ushort ustype; // 经脉类型 1阳 2阴
	ushort usintfid; // 内力id
	ushort uslevel; // 内力等级

	char szmeridname[MAX_NAME_LEN + 1]; //　经脉名称
	char szintfname[MAX_NAME_LEN + 1]; // 内力名称
}PET_MERID_INFO,* LP_PET_MERID_INFO;


typedef struct _PET_INTF_INFO // 内力箱中的内力
{
	ushort ustype; // 内力 1阳 2阴
	ushort uid; // 内力id
	ushort uslevel; // 内力等级
	ushort uscult; // 修为
	char szname[MAX_NAME_LEN + 1]; // 内力名称
}PET_INTF_INFO,* LP_PET_INTF_INFO;

typedef struct _PET_INTF_LIST // 内力箱中的内力
{
	ushort uscount;
	PET_INTF_INFO intflist[MAX_INTF_COUNT]; // 内力列表
}PET_INTF_LIST,* LP_PET_INTF_LIST;

typedef struct _PET_INTFMERID_LIST // 经脉内力列表
{
	ushort usmeridcount; // 经脉个数
	ushort usbagcount; // 内力箱个数
	ushort usintfcount; // 内力箱中内力个数
	PET_MERID_INFO meridlist[MAX_MERID_COUNT]; // 经脉列表
	PET_INTF_INFO intflist[MAX_INTF_COUNT]; // 内力列表
}PET_INTF_MERID_LIST,* LP_PET_INTFMERID_LIST;

/************************************************************************/
//                        百度贴吧签到                                                                     
/************************************************************************/
typedef struct _WAP_BAIDU_ATTENTION
{
	ushort uslevel;								// 头衔等级
	ushort uscurexp;							// 当前经验
	uint uid;
	char szname[MAX_NAME_LEN + 1];				// 贴吧名称
	char sznickname[MAX_NAME_LEN + 1];			// 头衔称呼
}WAP_BAIDU_ATTENTION,* LP_WAP_BAIDU_ATTENTION;

typedef struct _WAP_BAIDU_ATTENTION_LIST
{
	ushort uscount;											// 关注贴吧数
	char sztbs[2 * MAX_NAME_LEN + 1];
	WAP_BAIDU_ATTENTION stattention[MAX_ATTENTION_COUNT];	// 贴吧列表
}WAP_BAIDU_ATTENTION_LIST,* LP_WAP_BAIDU_ATTENTION_LIST;

/************************************************************************/
//                           12306订票
/************************************************************************/

#define MAX_TRAIN_NO 5						// 车次
#define MAX_CITY_LEN 10						// 站点
#define MAX_TIME_LEN 8						// 时间
#define TRAIN_TICKET_TYPE 11				// 票务数量
#define TICKET_SIGN_LEN 5					// 票务信息： 有,1,无,--
#define SUBMIT_CODE_LEN 300					// 预定提交编码
#define MAX_LOAD_DATA 50					// 一次查询车次数量

#define MAX_TRAIN_PASSENGER 20
#define MAX_PASSENGER_NAME 10
#define MAX_PASSENGER_IDNO 35
#define MAX_MOBILE_LEN 11
#define MAX_PHONE_LEN 15

typedef struct _QUERY_DATA_TRAIN
{
	ushort bsubmit;							// 能否预订
	ushort bstart;							// 始发站
	ushort bend;							// 终点站
	char sztrain_nos[MAX_PHONE_LEN + 1];	// 车次代码
	char sztrain_no[MAX_TRAIN_NO + 1];		// 车次 
	char szstartcity[MAX_CITY_LEN + 1];		// 发站
	char szstarttime[MAX_TIME_LEN + 1];		// 发站时间
	char szendcity[MAX_CITY_LEN + 1];		// 到站
	char szendtime[MAX_TIME_LEN + 1];		// 到站时间
	char szusetime[MAX_TIME_LEN + 1];		// 历时
	char szTicket[TRAIN_TICKET_TYPE][TICKET_SIGN_LEN + 1]; // 各票务信息
	char szsubmitcode[SUBMIT_CODE_LEN + 1];	// 可否订票，订票编码
}QUERY_DATA_TRAIN,* LP_QUERY_DATA_TRAIN;

typedef struct _QUERY_DATA_LIST
{
	ushort uscount;							// 列车数量
	QUERY_DATA_TRAIN stlist[MAX_LOAD_DATA]; // 列车信息
}QUERY_DATA_LIST,* LP_QUERY_DATA_LIST;

typedef struct _TRAIN_PASSENGER_INFO
{
	ushort ustype;							// 乘客类型：成人|儿童|学生|残军
	char szname[MAX_PASSENGER_NAME + 1];	// 姓名
	char szidtype[MAX_TRAIN_NO + 1];		// 证件类型
	char szidno[MAX_PASSENGER_IDNO + 1];	// 证件号
	char szmobile[MAX_MOBILE_LEN + 1];		// 手机号
}TRAIN_PASSENGER_INFO,* LP_TRAIN_PASSENGER_INFO;

typedef struct _TRAIN_PASSENGER_LIST
{
	ushort uscount;
	TRAIN_PASSENGER_INFO stpassenger[MAX_TRAIN_PASSENGER];
}TRAIN_PASSENGER_LIST,* LP_TRAIN_PASSENGER_LIST;

#endif
