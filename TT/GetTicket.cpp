// GetTicket.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GetTicket.h"
#include "GetTicketDlg.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetTicketApp

BEGIN_MESSAGE_MAP(CGetTicketApp, CWinApp)
	//{{AFX_MSG_MAP(CGetTicketApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetTicketApp construction

CGetTicketApp::CGetTicketApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGetTicketApp object

CGetTicketApp theApp;

string g_strapppath; // 应用程序路径
CLoginRequest* creq = NULL;
CHttpSocket chs;
const string strhost = "kyfw.12306.cn";//"dynamic.12306.cn";
/////////////////////////////////////////////////////////////////////////////
// CGetTicketApp initialization

BOOL CGetTicketApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif	

	string str = "ABC\u6210\u4ebaABC\u6210\u4ebaABC\u6210\u4eba";
	char sbuff[100] = {0};
	unicode_to_utf(str, sbuff, 100);
	string sbtr(sbuff);
	CHandleCode::UTF8ToGBK(sbtr);

	//只能启动一个程序
	HANDLE hMutex=CreateMutex(NULL,FALSE,"RunOnlyOneInstance_GETTICKETS");
    if( hMutex==NULL || ERROR_ALREADY_EXISTS==::GetLastError()) 
    {
		HWND hWnd = ::FindWindow(NULL, "订票工具");
		if (hWnd != NULL)
		{
			SetWindowPos(hWnd, CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetWindowPos(hWnd, CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		else
		{
			AfxMessageBox("程序已经启动，请检测后重试！");
		}
        exit(1);     
    }

	char szAppPath[MAX_PATH + 1] = {0};
	::GetModuleFileName(NULL, szAppPath, MAX_PATH);
	g_strapppath = szAppPath;
	g_strapppath = getlastpath(g_strapppath);

	// 获取路径	
	strsignfile = g_strapppath + strsignfile;
	strRCodePath = g_strapppath + strRCodePath;
	strerrorfile = g_strapppath + strerrorfile;
	strinifile = g_strapppath + strinifile;
	
	CMemory_Conf::instance()->load_conf(strinifile.c_str());
	
	WSAData wsdata;
	int iret = WSAStartup(0x0101, &wsdata);
	if (iret != 0)
	{
		AfxMessageBox("WSAStartup error.");
		exit(0);
	}
	creq = new CLoginRequest();
	if (creq == NULL)
	{
		AfxMessageBox("Create Https socket error.");
		exit(0);
	}
	chs.addmap("Host", strhost); // 解析域名
#ifdef USE_HTTPS
	if (!creq->init(chs.get_hostip(), CLoginRequest::HTTPS_PORT, strsignfile))
#else
	if (!creq->init(chs.get_hostip(), CLoginRequest::HTTP_PORT, ""))
#endif
	{
		AfxMessageBox("Init Https socket error.");
		exit(0);
	}
	
	// TODO: Add extra initialization here
	// 先登陆
	CLoginDlg dlgLogin;
	dlgLogin.SetCookieClass(creq);
	if (dlgLogin.DoModal() != IDOK)
	{
		exit(0);
	}	

	CGetTicketDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
