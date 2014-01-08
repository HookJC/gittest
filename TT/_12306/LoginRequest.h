/******************************************
* qq web login for webapp
* add qqpet:fight.pet.qq.com
* 
*
* create by: hookjc.taobao.com
* date: 2013-08-27
*******************************************/
#ifndef __LOGIN_REQUEST_H__
#define __LOGIN_REQUEST_H__

#include "SocketHTTPRequest.h"
#include "typedef.h"
#include "qmap.h"
#include "HttpSocket.h"
#include "OpensslHttpsClt.h"

class CLoginRequest : public CSocketHTTPRequest
{
public:
	CLoginRequest();
	~CLoginRequest();

	enum {HTTP_PORT = 80, HTTPS_PORT = 443};
	
public:
	bool init(const string& strip, unsigned short usport, const string& strsignfile);
	string GetLoginuiKey(const string& strpath); // 加载登陆所需要的Cookie
	string GetCheckVertify(const char* pszrandcode);
	bool GetVertifyImg(const string& strfixpath, const string& strfilename); // 获取并保存验证码图片 
	string GetLoginReturn(const string& struin, const string& strpwd, const string& strvertify);
	string GetSession();
	void SetSession(const string& strsess);
	void CleanSession();
	void https_close_connect();

	int http_get(const string& strgeturl, string& strrsp);
	int http_post(const string& strposturl, const string& strdata, string& strrsp);

public:	
	int parse(CQMap& req, string& strack);

protected:	
	int convert_code(string& strrsp);
	int senddata(const char* pszdata, unsigned long ulsize);
	int recvdata(char* pszoutbuff, unsigned int ulchlen);
	int sendandrecv(const string& strdata, string& strrecv);

private:
	int query_ticket_info(CQMap& req, string& strack);
	int sign_in_attention(CQMap& req, string& strack);
	int get_passenger_info(CQMap& req, string& strack);
	int post_submit_order(CQMap& req, string& strack);
	int post_passenger_info_togetflow(CQMap& req, string& strack);
	int get_queue_count(CQMap& req, string& strack);
	int get_passenger_action_init(CQMap& req, string& strack);
	int get_confirm_single_queue(CQMap& req, string& strack);
	int get_queryOrderWaitTime(CQMap& req, string& strack);
	
private:	
	CHttpSocket m_chreqsocket;
	CHttpSocket m_chrspsocket;
	tpfhttpmap m_headmap;
	tpfhttpmap m_sessionmap;
	string		m_strsess;

	// https
	COpensslHttpsClt m_chttps;
};



#endif

