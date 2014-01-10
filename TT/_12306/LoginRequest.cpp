
#include "typedef.h"
#include "function.h"
#include "LoginRequest.h"
#include "api/HandleCode.h"
#include "shlwapi.h"

#ifdef MT_THREAD
#ifdef USE_HTTPS
#undef USE_HTTPS
#endif
#endif

#define MAX_RSP_BLOCK	10240
#define URL_PAGE_PORT 80

string CLoginRequest::GetRand()
{
	string strrand = "";
	char szBuf[16 + 1] = {0};
	static time_t s_tm = time(0);	
	int irand = rand();
	while (strlen(szBuf) < 16)
	{
		srand((++s_tm) % 10);
		irand = rand();
		strncat(szBuf + strlen(szBuf), i2str(irand).c_str(), sizeof(szBuf) - strlen(szBuf) - 1);		
	}
	strrand = szBuf;
	return strrand;
}

CLoginRequest::CLoginRequest()
{
	m_chreqsocket.addmap("Accept", "*/*");
	m_chreqsocket.addmap("Connection", "keep-alive");
	m_chreqsocket.addmap("Accept-Language", "zh-cn");
	m_chreqsocket.addmap("Referer", "https://kyfw.12306.cn/otn/login/init");
	m_chreqsocket.addmap("Content-Type", "application/x-www-form-urlencoded"); /*; charset=GBK */
	m_chreqsocket.addmap("User-Agent", "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727)");
	m_chreqsocket.addmap("Cache-Control", "no-cache");
	m_chreqsocket.addmap("Host", "kyfw.12306.cn");

	COpensslHttpsClt::init_openssl();
}

CLoginRequest::~CLoginRequest()
{
	
}

bool CLoginRequest::init(const string& strip, unsigned short usport, const string& strsignfile)
{
	m_strip = strip;
	m_usport = usport;

#ifdef USE_HTTPS
	m_chttps.set_https_cerfile(strsignfile.c_str());

	return (m_chttps.set_https_connect(strip.c_str(), usport)
		&& m_chttps.https_check_cer() && m_chttps.https_connect()); //*/
#else
	return Init(strip.c_str(), usport); 
#endif
}

int CLoginRequest::senddata(const char* pszdata, unsigned long ulsize)
{
#ifdef USE_HTTPS
	return m_chttps.https_write_chars(pszdata, ulsize);
#else
	return CSocketHTTPRequest::senddata(pszdata, ulsize);
#endif
}

int CLoginRequest::recvdata(char* pszoutbuff, unsigned int ulchlen)
{
#ifdef USE_HTTPS
	return m_chttps.https_read_chars(pszoutbuff, ulchlen);
#else
	return recvchar(pszoutbuff, ulchlen);
#endif
}

int CLoginRequest::sendandrecv(const string& strdata, string& strrecv)
{
	int iret = 0;
	char szblock[MAX_ACK_RSP_LEN + 1] = {0};

#ifdef MT_THREAD
	SOCKET so = open_socket(m_strip, m_usport);
	iret = send_socket_data(strdata.c_str(), strdata.size(), so);
#else
	iret = senddata(strdata.c_str(), strdata.size());
#endif
	if (iret < 0) 
	{
#ifdef USE_HTTPS
		if (m_chttps.https_connect())
#else
#ifdef MT_THREAD
		so = open_socket(m_strip, m_usport);
		if (so > 0)
#else
		if (reconn() == 0)
#endif
#endif
		{
#ifdef MT_THREAD
			iret = send_socket_data(strdata.c_str(), strdata.size(), so);
#else
			iret = senddata(strdata.c_str(), strdata.size());
#endif
		}
		else
		{
			strrecv = "sendandrecv send https_reconn error. Error code: -1.";
			strrecv += m_chttps.GetLastError();
			return -1;
		}			
	}
#ifdef MT_THREAD
	iret = recv_socket_data(szblock, MAX_ACK_RSP_LEN, so);
#else
	iret = recvdata(szblock, MAX_ACK_RSP_LEN);	
#endif
	if (iret < 0)
	{
#ifdef USE_HTTPS
		if (!m_chttps.https_connect())
#else
#ifndef MT_THREAD
			if (reconn() != 0)
#endif
#endif
			{
				strrecv = "sendandrecv recv https_reconn error. Error code: -2.";
				strrecv += m_chttps.GetLastError();
				return -2;
			}
	}
	
	strrecv = szblock;

	return 0;
}

string CLoginRequest::GetLoginuiKey(const string& strpath)
{
	string strRsp;
	string strReq;
	int iret = 0;

	m_chreqsocket.setvale("Referer", "https://kyfw.12306.cn/otn/index/init");
	strReq += m_chreqsocket.get_gethead(strpath);
	
	iret = sendandrecv(strReq, strRsp);
	if (iret != 0)
	{
		strRsp += "\r\n";
		strRsp += "GetLoginuiKey error. error_num = ";
		strRsp += i2str(iret);
		return strRsp;
	}
	
	m_chrspsocket.parse(strRsp);
	m_strsess = m_chrspsocket.getval("Set-Cookie");
	if (!m_strsess.empty())
	{
		m_chreqsocket.addmap("Cookie", m_strsess);
	} //*/	

	if (StrStrI(m_chrspsocket.getval("Content-Type").c_str(), "utf") != NULL)
	{
		// utf8 ת gbk
		CHandleCode::UTF8ToGBK(strRsp);
	}
	
	return strRsp;
}

string CLoginRequest::GetCheckVertify(const char* pszrandcode)
{
	int iret = 0;
	string strRsp;	
	string strReq;
	string strpath = "/otn/passcodeNew/checkRandCodeAnsyn"; //"/otsweb/loginAction.do?method=loginAysnSuggest";

	strReq = m_chreqsocket.get_posthead(strpath, str_format("randCode=%s&rand=sjrand", pszrandcode));
	iret = sendandrecv(strReq, strRsp);
	if (iret != 0)
	{
		strRsp = "GetCheckVertify error. error_num = ";
		strRsp += i2str(iret);
	}	
	
	return strRsp;
}

bool CLoginRequest::GetVertifyImg(const string& strfixpath, const string& strfilename)
{
	char szrecvbuff[4 * MAX_BUFFER_BLOCK + 1] = {
		0
	};
	string strReq;
	string strpath = strfixpath;
	strpath += GetRand();
	
	strReq = m_chreqsocket.get_gethead(strpath);

	
	int iret = 0;	
#ifdef MT_THREAD
	SOCKET so = open_socket(m_strip, m_usport);
	iret = send_socket_data(strReq.c_str(), strReq.size(), so);
#else
	iret = senddata(strReq.c_str(), strReq.size());
#endif
	
	if (iret < 0)
	{
#ifdef USE_HTTPS
		if (m_chttps.https_connect())
#else
#ifndef MT_THREAD
		if (reconn() == 0)
#endif
#endif

#ifndef MT_THREAD
		{
			iret = senddata(strReq.c_str(), strReq.size());
			if (iret < 0)
			{
				return false;
			}
		}
		else
#endif
		{
			return false;
		}		
	}
#ifdef MT_THREAD
	iret = recv_socket_data(szrecvbuff, sizeof(szrecvbuff) - 1, so);
#else
	iret = recvdata(szrecvbuff, sizeof(szrecvbuff) - 1);
#endif

#ifdef USE_HTTPS
	if (iret < 0 && !m_chttps.https_connect())
#else
#ifndef MT_THREAD
	if (iret < 0 && reconn() != 0)
#endif  // MT_THREAD
#endif // USE_HTTPS	
#ifndef MT_THREAD
	{
		return false;
	}
	else
#endif
	{
		m_chrspsocket.parse(szrecvbuff);
		m_strsess = m_chrspsocket.getval("Set-Cookie");
		if (!m_strsess.empty())
		{
			m_chreqsocket.addmap("Cookie", m_strsess);
		} 
	}
	
	int icount = 0;
	CHttpSocket cimg(szrecvbuff);
	char szImg[MAX_VERTIFY_IMG_SIZE + 1] = {0};
	if (cimg.getval("Content-Length").empty() && cimg.getval("Transfer-Encoding") == "chunked")
	{
		icount = get_chunked_data(szrecvbuff, icount, szImg, MAX_VERTIFY_IMG_SIZE);
	}
	else
	{
		icount = atoi(cimg.getval("Content-Length").c_str());
		memcpy(szImg, szrecvbuff + cimg.get_rsphead().size() + 4, min(icount, MAX_VERTIFY_IMG_SIZE));
	}	
	
	if (icount > 0)
	{
		FILE* pf = fopen(strfilename.c_str(), "wb");
		if (pf)
		{
			fwrite(szImg, 1, icount, pf);
			fflush(pf);
			fclose(pf);
			return true;
		}
	}
	
	return false;
}

string CLoginRequest::GetLoginReturn(const string& struin, const string& strpwd, const string& strvertify)
{
	string strRsp;
	string strReq;
	int iret = 0;

	strReq += m_chreqsocket.get_posthead(struin, strpwd);
	
	iret = sendandrecv(strReq, strRsp);
	if (iret != 0)
	{
		strRsp += "\r\n";
		strRsp += "GetLoginReturn error. error_num = ";
		strRsp += i2str(iret);
		return strRsp;
	}

	m_chrspsocket.parse(strRsp);
	if (StrStrI(m_chrspsocket.getval("Content-Type").c_str(), "utf") != NULL)
	{
		// utf8 ת gbk
		CHandleCode::UTF8ToGBK(strRsp);
	}	

	return strRsp;
}

string CLoginRequest::GetSession()
{
	return m_chreqsocket.getval("Cookie");
}

void CLoginRequest::SetSession(const string& strsess)
{
	m_chreqsocket.addmap("Cookie", strsess);
}

void CLoginRequest::CleanSession()
{
	m_chreqsocket.remove_key("Cookie");
}

void CLoginRequest::https_close_connect()
{
	//m_chttps.https_free();
}

/*****************************************************************************
* Use cmd to get server info, could create class with the CLoginRequest base class.
* The new class use for difference appid,now all is here and change at last.
* Could use case cmd function.
* first to use function petgetinfo for set cookie
*
*****************************************************************************/ 

#include "petcmd.h"
int CLoginRequest::parse(CQMap& req, string& strack)
{
	int cmd = req.cmd();
	int iret = 0;
	
	switch (cmd)
	{
		case _12306_LOAD_DATA:
		{
			iret = query_ticket_info(req, strack);
		}
		break;

		case WAP_BAIDU_MODYFY_ATTENTION:
		{
			iret = sign_in_attention(req, strack);
		}
		break;

		case _12306_PREPARE_ORDER_DATA:
		{
			iret = post_submit_order(req, strack);
		}
		break;

		case _12306_GETTRAIN_PASSENGER:
		{
			iret = get_passenger_info(req, strack);
		}
		break;
		
		case _12306_CONFIRM_PASSENGER:
		{
			iret = post_passenger_info_togetflow(req, strack);
		}
		break;

		case _12306_GET_QUEUE_COUNT:
		{
			iret = get_queue_count(req, strack);
		}
		break;

		case _12306_PASSENGER_ACTION_INIT:
		{
			iret = get_passenger_action_init(req, strack);
		}
		break;

		case _12306_CONFRIM_QUEUE:
			{
				iret = get_confirm_single_queue(req, strack);
			}
			break;

		case _12306_queryOrderWaitTime:
			{
				iret = get_queryOrderWaitTime(req, strack);
			}
			break;

		default:
			iret = -5000;
		break;
	}

	string strhead = gethead(strack);	
	if (StrStrI(strhead.c_str(), "utf") != NULL)
	{
		strack = getbody(strack);
		CHandleCode::UTF8ToGBK(strack);	
	}
	if (strack.empty() && strhead.find("error. error_num") != string::npos)
	{
		if (!m_chttps.https_connect())
		{
			strack = strhead;
		}
	}
	
	return iret;
}

int CLoginRequest::query_ticket_info(CQMap& req, string& strack)
{
	string strpath = "/otsweb/order/querySingleAction.do?method=";
	strpath += req["method"];
	strpath += req["path"];

	string strreq = m_chreqsocket.get_gethead(strpath);
	int iret = sendandrecv(strreq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "GetLoginReturn error. error_num = ";
		strack += i2str(iret);
		return iret;
	}

	return 0;
}

int CLoginRequest::sign_in_attention(CQMap& req, string& strack)
{
	string strpath = "/mo/0/sign?tbs=";
	strpath += req["tbs"];
	strpath += "&fid=";
	strpath += req["fid"];
	strpath += "&kw=";
	strpath += req["kw"];

	m_chreqsocket.setvale("User-Agent", "Safari(U;iOS 5.1.1;Zh-cn;320*480;)/UCWEB8.4.1.169/42/997");
	m_chreqsocket.setvale("Host", "wapp.baidu.com");

	string strreq = m_chreqsocket.get_gethead(strpath);
	int iret = sendandrecv(strreq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "GetLoginReturn error. error_num = ";
		strack += i2str(iret);
		return iret;
	}

	return 0;
}

int CLoginRequest::get_passenger_info(CQMap& req, string& strack)
{
	// otn/passengers/init 
	string strpath = "/otsweb/order/confirmPassengerAction.do?method=getpassengerJson";
	string strReq = m_chreqsocket.get_posthead(strpath, "");

	int iret = sendandrecv(strReq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "get_passenger_info error. error_num = ";
		strack += i2str(iret);
		return iret;
	}

	return 0;
}

int CLoginRequest::post_submit_order(CQMap& req, string& strack)
{
	string strpath = "/otsweb/order/querySingleAction.do?method=submutOrderRequest";
	string strdata = req["data"];	
	string strReq = m_chreqsocket.get_posthead(strpath, strdata);
	
	int iret = sendandrecv(strReq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "post_submit_order error. error_num = ";
		strack += i2str(iret);
		return iret;
	}

	return 0;
}

int CLoginRequest::post_passenger_info_togetflow(CQMap& req, string& strack)
{
	string strpath = "/otsweb/order/confirmPassengerAction.do?method=checkOrderInfo&rand=";
	strpath += req["randCode"];
	string strdata = req["data"];

	string strReq = m_chreqsocket.get_posthead(strpath, strdata);
	
	int iret = sendandrecv(strReq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "post_passenger_info_togetflow error. error_num = ";
		strack += i2str(iret);
		return iret;
	}

	return 0;
}

// train_date=2013-10-03&train_no=69000K906408&station=K9064&seat=3&from=OSQ&to=HYQ&ticket=100770327410077000333017300010
int CLoginRequest::get_queue_count(CQMap& req, string& strack)
{
	string strpath = "/otsweb/order/confirmPassengerAction.do?method=getQueueCount";
	strpath += req["path"];
	/*strpath += "&train_date=";
	strpath += req["orderRequest.train_date"];
	strpath += "&train_no=";
	strpath += req["orderRequest.train_no"];
	strpath += "&station=";
	strpath += req["orderRequest.station_train_code"];
	strpath += "&seat=";
	strpath += req["passenger_1_seat"];
	strpath += "&from=";
	strpath += req["orderRequest.from_station_telecode"];
	strpath += "&to=";
	strpath += req["orderRequest.to_station_telecode"];
	strpath += "&ticket=";
	strpath += req["leftTicketStr"];*/

	string strReq = m_chreqsocket.get_gethead(strpath);
	
	int iret = sendandrecv(strReq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "post_passenger_info_togetflow error. error_num = ";
		strack += i2str(iret);
		return iret;
	}

	return 0;
}

int CLoginRequest::get_passenger_action_init(CQMap& req, string& strack)
{
	string strpath = "/otsweb/order/confirmPassengerAction.do?method=init";

	string strReq = m_chreqsocket.get_gethead(strpath);
	
	int iret = sendandrecv(strReq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "post_passenger_info_togetflow error. error_num = ";
		strack += i2str(iret);
		return iret;
	}
	return 0;
}

int CLoginRequest::get_confirm_single_queue(CQMap& req, string& strack)
{
	string strpath = "/otsweb/order/confirmPassengerAction.do?method=confirmSingleForQueue";
	string strdata = req["data"];
	
	string strReq = m_chreqsocket.get_posthead(strpath, strdata);
	
	int iret = sendandrecv(strReq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "get_confirm_single_queue error. error_num = ";
		strack += i2str(iret);
		return iret;
	}
	
	return 0;
}

int CLoginRequest::get_queryOrderWaitTime(CQMap& req, string& strack)
{
	string strpath = "/otsweb/order/myOrderAction.do?method=queryOrderWaitTime&tourFlag=dc";
	
	string strReq = m_chreqsocket.get_gethead(strpath);
	
	int iret = sendandrecv(strReq, strack);
	if (iret != 0)
	{
		strack += "\r\n";
		strack += "get_queryOrderWaitTime error. error_num = ";
		strack += i2str(iret);
		return iret;
	}
	return 0;
}

int CLoginRequest::convert_code(string& strrsp)
{
	string strhead = gethead(strrsp);	
	if (StrStrI(strhead.c_str(), "utf") != NULL)
	{
		strrsp = getbody(strrsp);
		CHandleCode::UTF8ToGBK(strrsp);	
	}
	if (strrsp.empty() && strhead.find("error. error_num") != string::npos)
	{
#ifdef USE_HTTPS
		if (!m_chttps.https_connect())
#else
		if (reconn() != 0)
#endif
		{
			strrsp = strhead;
		}
	}
	return 0;
}

int CLoginRequest::http_get(const string& strgeturl, string& strrsp)
{
	string strReq = m_chreqsocket.get_gethead(strgeturl);
	
	int iret = sendandrecv(strReq, strrsp);
	if (iret != 0)
	{
		strrsp += "\r\n";
		strrsp += "get_queryOrderWaitTime error. error_num = ";
		strrsp += i2str(iret);
		return iret;
	}

	convert_code(strrsp);

	return 0;
}

int CLoginRequest::http_post(const string& strposturl, const string& strdata, string& strrsp)
{
	string strReq = m_chreqsocket.get_posthead(strposturl, strdata);

	int iret = sendandrecv(strReq, strrsp);
	if (iret != 0)
	{
		strrsp += "\r\n";
		strrsp += "get_queryOrderWaitTime error. error_num = ";
		strrsp += i2str(iret);
		return iret;
	}
	
	convert_code(strrsp);

	return 0;
}