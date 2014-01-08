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
	SetWindowText(("��Ʊ���� - " + strfixuserinfo).c_str());	
	m_cbtn.SubclassWindow(GetDlgItem(IDC_BTN_RCODE)->m_hWnd);	

	char sztmp[1024 + 1] = {0};
	CMemory_Conf::instance()->get_conf_str("����", "�û���", "", sztmp, 1024);
	m_strUserName = sztmp;
	CMemory_Conf::instance()->get_conf_str("����", "����", "", sztmp, 1024);
	m_strPassWord = sztmp;
	UpdateData(FALSE);

	CMemory_Conf::instance()->get_conf_str("����", "Cookie", "", sztmp, 1024);	
	// ����ʹ�ñ����session��ѯ������Ϣ
	if (strcmp(sztmp, "") != 0)
	{
		m_strSession = sztmp;
		creq_->SetSession(m_strSession);
		string strret;
		creq_->http_get("/otn/modifyUser/initQueryUserInfo", strret);
		if (strret.find(strfixuserinfo) != string::npos)
		{			
			SetTimer(2, 100, NULL);
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
	// ����ҳ�漰��֤��
	string strret;
	//strret = creq_->GetLoginuiKey("/otn/login/init");	// ��ʼ����½ҳ��

	pT->OnBtnRcode(); // ������֤��: ��Cookie

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
			// Enter��			
			// ��½ 
			if (LoginTicket())
			{
				CDialog::OnOK();
			}
			return FALSE; // ��½ʧ��
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CLoginDlg::OnBtnRcode() 
{
	// TODO: Add your control notification handler code here
	// ������֤�� /otsweb/passCodeNewAction.do?module=login&rand=sjrand&0.7617307564114698
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(FALSE);
	SetDlgItemText(IDC_EDIT_RCODE, "������");
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
			AfxMessageBox("��ȡ������֤�룬�����������г���");
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
	UpdateData();

	// ��֤
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
	
	// ��½	
	string strloginrand;
	int icount = 0;
	string strret;
	Json::Reader jread;
	Json::Value jvalue;
	

	SetWindowText("��֤��֤��....");
	do
	{	strret = str_format("randCode=%s&rand=sjrand", m_strRCode);
		creq_->http_post("/otn/passcodeNew/checkRandCodeAnsyn", strret, strret);
		if (jread.parse(strret, jvalue))
		{
			if (jvalue["data"].asString() == "Y")
			{
				strloginrand = jvalue["loginRand"].asString();
				SetWindowText("��֤����ȷ");
				break;
			}
			else
			{
				SetWindowText("��֤�����");
				return FALSE;
			}
		}
		else
		{
			++icount;
		}		
		if (icount > 10)
		{
			AfxMessageBox("Ч����֤��ʧ��, ������������֤�룡");
			return FALSE;
		}
		_sleep(1000);
	}while (1);

	SetWindowText("��½��...");
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
		// �����û���������
		SetWindowText("��½�ɹ�");
		// Ч��Ȩ�ޣ�
		creq_->http_get("/otn/modifyUser/initQueryUserInfo", strret);
		if (strret.find(strfixuserinfo) == string::npos)
		{
			AfxMessageBox("δ�Դ��˺���Ȩ������ϵ������Ա");
			return FALSE;
		}		
		CMemory_Conf::instance()->write_conf_str("����", "�û���", m_strUserName);
		CMemory_Conf::instance()->write_conf_str("����", "����", m_strPassWord);
		m_strSession = creq_->GetSession();
		CMemory_Conf::instance()->write_conf_str("����", "Cookie", m_strSession.c_str());
		
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
		AfxMessageBox("�û������������֤�����");
		return FALSE;
	}
	
	return TRUE;
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
		if (LoginTicket())
		{
			CDialog::OnOK();
		}
		else
		{
			AfxBeginThread(ThreadLoadInit, this);
		}
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
		CDialog::OnOK();
	}
	
}
