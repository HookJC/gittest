// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetTicket.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLoginRequest* creq_ = NULL;
/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_strRCode = _T("");
	m_strUserName = _T("");
	m_strPassWord = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bLogin = FALSE;
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_EDIT_RCODE, m_strRCode);
	DDV_MaxChars(pDX, m_strRCode, 4);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PSWORD, m_strPassWord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_RCODE, OnBtnRcode)
	ON_EN_CHANGE(IDC_EDIT_RCODE, OnChangeEditRcode)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetWindowText(("订票工具 - " + strfixuserinfo).c_str());	
	m_cbtn.SubclassWindow(GetDlgItem(IDC_BTN_RCODE)->m_hWnd);	

	char sztmp[1024 + 1] = {0};
	CMemory_Conf::instance()->get_conf_str("配置", "用户名", "", sztmp, 1024);
	m_strUserName = sztmp;
	CMemory_Conf::instance()->get_conf_str("配置", "密码", "", sztmp, 1024);
	m_strPassWord = sztmp;
	UpdateData(FALSE);

	CMemory_Conf::instance()->get_conf_str("配置", "Cookie", "", sztmp, 1024);
	
	SetTimer(2, 100, NULL);
	
	// 尝试使用保存的session查询个人信息
	if (strcmp(sztmp, "") != 0)
	{
		m_strSession = sztmp;
		creq_->SetSession(m_strSession);
		string strret;
		creq_->http_get("/otn/modifyUser/initQueryUserInfo", strret);
		if (strret.find(strfixuserinfo) != string::npos)
		{
			m_bLogin = TRUE;			
			return TRUE;
		}		
	}

	AfxBeginThread(ThreadLoadInit, this);

	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

UINT CLoginDlg::ThreadLoadInit(LPVOID lpVoid)
{
	CLoginDlg* pT = (CLoginDlg* )lpVoid;
	pT->m_bLogin = TRUE;
	// 加载页面及验证码
	string strret;
	//strret = creq_->GetLoginuiKey("/otn/login/init");	// 初始化登陆页面

	pT->OnBtnRcode(); // 加载验证码: 含Cookie

	pT->m_bLogin = FALSE;
	return 0;
}

void CLoginDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
		
	CDialog::OnCancel();
}

void CLoginDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CLoginDlg::SetCookieClass(LPVOID lpVoid)
{
	creq_ = (CLoginRequest* )lpVoid;	
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return FALSE;
		}
		if (pMsg->wParam == VK_RETURN)
		{
			// Enter键			
			// 登陆 
			UpdateData();
			AfxBeginThread(ThreadLogin, (LPVOID)this);
			return FALSE; // 登陆失败
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CLoginDlg::OnBtnRcode() 
{
	// TODO: Add your control notification handler code here
	// 加载验证码 /otsweb/passCodeNewAction.do?module=login&rand=sjrand&0.7617307564114698
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(FALSE);
	SetDlgItemText(IDC_EDIT_RCODE, "加载中");
	m_cbtn.EnableWindow(FALSE);
	const char* pszloginimg = "/otn/passcodeNew/getPassCodeNew?module=login&rand=sjrand&0.";//"/otsweb/passCodeNewAction.do?module=login&rand=sjrand&";
	int icount = 0;
	bool bret = false;
	do
	{
		if (bret = creq_->GetVertifyImg(pszloginimg, strRCodePath))
		{			
			break;
		}
		++icount;
		if (icount > 10)
		{
			AfxMessageBox("获取不到验证码，建议重新运行程序！");
			_sleep(1000);
		}
	}while(!bret);
	m_cbtn.EnableWindow(TRUE);
	m_cbtn.Reflush();
	SetDlgItemText(IDC_EDIT_RCODE, "");
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RCODE)->SetFocus();	
}

BOOL CLoginDlg::LoginTicket()
{
	if (m_bLogin)
	{
		return FALSE;
	}	

	// 验证
	if (m_strUserName.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_USERNAME)->SetFocus();
		return FALSE;
	}
	if (m_strPassWord.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_PSWORD)->SetFocus();
		return FALSE;
	}
	if (m_strRCode.IsEmpty() || m_strRCode.GetLength() < 4)
	{
		GetDlgItem(IDC_EDIT_RCODE)->SetFocus();
		return FALSE;
	}
	
	// 登陆	
	string strloginrand;
	int icount = 0;
	string strret;
	Json::Reader jread;
	Json::Value jvalue;
	

	SetWindowText("验证验证码....");
	do
	{	strret = str_format("randCode=%s&rand=sjrand", m_strRCode);
		creq_->http_post("/otn/passcodeNew/checkRandCodeAnsyn", strret, strret);
		if (jread.parse(strret, jvalue))
		{
			if (jvalue["data"].asString() == "Y")
			{
				strloginrand = jvalue["loginRand"].asString();
				SetWindowText("验证码正确");
				break;
			}
			else
			{
				SetWindowText("验证码错误");
				return FALSE;
			}
		}
		else
		{
			++icount;
		}		
		if (icount > 10)
		{
			AfxMessageBox("效验验证码失败, 请重新输入验证码！");
			return FALSE;
		}
		_sleep(1000);
	}while (1);

	SetWindowText("登陆中...");
	m_bLogin = TRUE;

	// second check verifycode
	strret = str_format("randCode=%s&rand=sjrand", m_strRCode);
	creq_->http_post("/otn/passcodeNew/checkRandCodeAnsyn", strret, strret);
	
	strret = "";
	
	// login 
	while (strret.empty())
	{	
		string strack = str_format("loginUserDTO.user_name=%s&userDTO.password=%s&randCode=%s"
			, m_strUserName, m_strPassWord, m_strRCode);	
		creq_->http_post("/otn/login/loginAysnSuggest", strack, strret);
		_sleep(100);
	}

	//write_to_file(g_strapppath +"\\loginAction.htm", CSocketHTTPRequest::getbody(strret), false);	
	m_bLogin = FALSE;
	if (strret.find("{\"loginCheck\":\"Y\"}") != string::npos)
	{
		// 保存用户名和密码
		SetWindowText("登陆成功");
		// 效验权限：
		creq_->http_get("/otn/modifyUser/initQueryUserInfo", strret);
		if (strret.find(strfixuserinfo) == string::npos)
		{
			AfxMessageBox("未对此账号授权，请联系开发人员");
			return FALSE;
		}		
		CMemory_Conf::instance()->write_conf_str("配置", "用户名", m_strUserName);
		CMemory_Conf::instance()->write_conf_str("配置", "密码", m_strPassWord);
		m_strSession = creq_->GetSession();
		CMemory_Conf::instance()->write_conf_str("配置", "Cookie", m_strSession.c_str());
		
		char szfileconn[4096 + 1] = {0};
		int iconn = 4096;
		CEncrypt_Data ce;
		CMemory_Conf::instance()->get_str(szfileconn, iconn);
		ce.encode(szfileconn, strlen(szfileconn), szfileconn, &iconn);
		ce.save(strinifile.c_str(), szfileconn, iconn);
		
		//CMemory_Conf::instance()->save_conf();
		return TRUE;
	}
	else
	{
		AfxMessageBox("用户名或密码或验证码错误");
		return FALSE;
	}
	
	return TRUE;
}

UINT CLoginDlg::ThreadLogin(LPVOID lpVoid)
{
	CLoginDlg* pThis = (CLoginDlg* )lpVoid;
	
	pThis->m_bLogin = pThis->LoginTicket();
	if (!pThis->m_bLogin)
	{
		pThis->ThreadLoadInit(pThis);
	}

	return 0;
}


void CLoginDlg::OnChangeEditRcode() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_RCODE, m_strRCode);
	if (m_strRCode.GetLength() == 4)
	{
		UpdateData();
		AfxBeginThread(ThreadLogin, (LPVOID)this);
// 		if (LoginTicket())
// 		{
// 			CDialog::OnOK();
// 		}
// 		else
// 		{
// 			AfxBeginThread(ThreadLoadInit, this);
// 		}
	}
}

void CLoginDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		LoginTicket();
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		KillTimer(1);
		CDialog::OnTimer(nIDEvent);
		return;
	}

	if (nIDEvent == 2)
	{
		if (m_bLogin)
		{
			CDialog::OnOK();
		}		
	}
	
}
