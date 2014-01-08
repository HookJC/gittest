// GetTicketDlg.cpp : implementation file
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

#ifdef _DEBUG
#pragma comment(lib, "libjsond.lib")
#else
#pragma comment(lib, "libjson.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/************************************************************************/
//                                �������
/*Json::Reader jread;
Json::Value jvalue;
Json::Value jlist;
Json::Value jlistitem;
int i,j,k,m,n,icount;
int iret = 0;
bool bret = false;
CQMap cmap;
CHttpSocket chs; // host
CLoginRequest* creq = NULL; // main class
string strret;
string strack;
string strtmp;
string strloginrand;
char sztmp[MAX_BUFFER_BLOCK + 1] = {0};*/
string strrandcode;
/************************************************************************/

// Param  ����
const unsigned int uigetday = 20; // 20���ڶ�Ʊ
const unsigned int uiTicketSpace = 3; // �ύ������ȷ�϶���ʱ�������ύ���콫����������ܴ������⣬�����ԣ�
const unsigned int uireflushms = 5000; // ��ѯ���
const string strfixuserinfo = "������"; // ָ���û�����ʹ�øù���
map<string, string> mapstrcity; // ������Ϣ ���У�����

string strRCodePath = "\\randCode.jpg"; // ��֤��
string strsignfile = "\\pem_12306.pem";	// ֤��
string strerrorfile = "\\error.txt";
string strinifile = "\\��������.ini";
string stralterapp = "RightDownNotify.exe";
const char* pszloginimg = "/otn/passcodeNew/getPassCodeNew?module=login&rand=sjrand&0."; // ��½��֤��
const char* pszpasgeimg = "/otsweb/passCodeNewAction.do?module=passenger&rand=randp&"; // ��Ʊ��֤��
const char* pszseattext[] = {"������", "�ص���", "һ����", "������", "�߼�����","����", "Ӳ��", "����", "Ӳ��", "����", "����"}; // ��ϯ
const char* pszseattype[] = {"9", "P", "M", "O", "", "4", "3", "2", "1", "1", ""}; // ��ϯ����
const char* pszpassenger[] = {"����Ʊ", "��ͯƱ", "ѧ��Ʊ", "�о�Ʊ"}; // 1|2|3|4
const char* pszpassenger_cardtext[] = {"�������֤", "һ�����֤", "�۰�ͨ��֤", "̨��ͨ��֤", "����"}; //1|2|C|G|B
const char* pszpassenger_cardno[] = {"1", "2", "C", "G", "B"}; // ֤������
const char* pszorderparam[] = {"station_train_code", "lishi", "train_start_time", "trainno4", "from_station_telecode"
, "to_station_telecode", "arrive_time", "from_station_telecode_name", "to_station_telecode_name", "from_station_no"
, "to_station_no", "ypInfoDetail", "mmStr", "locationCode"}; // ת��Ԥ��ҳ���ύ�ֶ�
const char* pszsubmitconfirm[] = {"org.apache.struts.taglib.html.TOKEN", "leftTicketStr", "orderRequest.train_date"
, "orderRequest.train_no", "orderRequest.station_train_code", "orderRequest.from_station_telecode"
, "orderRequest.to_station_telecode", "orderRequest.seat_type_code", "orderRequest.ticket_type_order_num"
, "orderRequest.bed_level_order_num", "orderRequest.start_time", "orderRequest.end_time", "orderRequest.from_station_name"
		, "orderRequest.to_station_name", "orderRequest.cancel_flag", "orderRequest.id_mode"}; // �����ύҳ����ֶ�

/************************************************************************/


/************************************************************************/
//                               ��Ʊ�˹�Ʊ��Ϣ
// ��ѯ����: ����
string startcity = "";
string startcitycode =""; // �������� station_name.txt
string endcity = "";
string endcitycode = ""; // Ŀ�ĳ���	
string trainCodeText = ""; // ����
string train_no = ""; // ���δ���
string trainPassType = "QB"; // ��· : ȫ��|ʼ��|��·
string trainClass = "QB#D#Z#T#K#QT#"; // ���ͣ�ȫ��|����|Z��ͷ|T��ͷ|K��ͷ|����
string includeStudent = "ADULT"; // ����ѧ��Ʊ
string seatTypeAndNum = ""; // 
string startdate = ""; // ��������
string startTime = "00:00--24:00";	// ����ʱ��
// end ��ѯ����

// �˿���Ϣ: �Ѿ����س˿���Ϣֱ�ӻ�ȡ
string strseattext = "Ӳ��";
ushort usseatidx = 10; // ��ϯλ��
ushort usselseat = 0;
string strpassenger_cardno = "";
// end �˿���Ϣ
/************************************************************************/

string gettickinfo(const string& strtickinfo, const string& strseattype)
{
	string strret;
	string strseat;
	ushort seat_1 = 0;
	ushort seat_2 = 0;
	ushort pos = 0;
	string str = "";
	int icounts = 0;
	while (pos < strtickinfo.size())
	{
		str = strtickinfo.substr(pos, 10);
		strseat = str.substr(0, 1);
		if (strseattype == strseat)
		{
			icounts = getfirstint(str.substr(6, 4).c_str());
			if (icounts < 3000)
			{
				seat_1 = icounts;
			}
			else
			{
				seat_2 = icounts - 3000;
			}
		}
		pos += 10;
	}
	if (seat_1 > 0) {
		strret += i2str(seat_1) + "��";
	}
	if (seat_2 > 0) {
		strret += ",����";
		strret += i2str(seat_2) + "��";
	}
	return strret;
}



/////////////////////////////////////////////////////////////////////////////
// CGetTicketDlg dialog

#define MAX_TICKET_LENGTHS 32

void CGetTicketDlg::alertexit()
{
	creq->https_close_connect();
	AfxMessageBox("�������н��������ȷ���˳�������");	
	CDialog::OnOK();
}

CGetTicketDlg::CGetTicketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetTicketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetTicketDlg)
	m_nSeatType = -1;
	m_strEndCity = _T("");
	m_strRCode = _T("");
	m_strStartCity = _T("");
	m_strTrainNo = _T("");
	m_strName = _T("");
	m_strVerifyCode = _T("");
	m_nTicketType = -1;
	m_strStartDate = _T("");
	m_strMobile = _T("");
	m_uiTmGetTicket = 0;
	m_uiTmSubmitTicket = 0;
	m_strStartTime = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iSelMan = -1;
	m_bRunning = FALSE;
	m_nGetTicketTimes = 0;
}

void CGetTicketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetTicketDlg)
	DDX_Control(pDX, IDC_ST_INFO, m_cInfo);
	DDX_Control(pDX, IDC_LIST_TICKETS, m_listTicket);
	DDX_Control(pDX, IDC_LIST_MAN, m_listMan);
	DDX_CBIndex(pDX, IDC_COMBO_SETTYPE, m_nSeatType);
	DDX_Text(pDX, IDC_EDIT_ENDCITY, m_strEndCity);
	DDX_Text(pDX, IDC_EDIT_RCODE, m_strRCode);
	DDX_Text(pDX, IDC_EDIT_STARTCITY, m_strStartCity);
	DDX_Text(pDX, IDC_EDIT_TRANSNO, m_strTrainNo);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_VERIFYCODE, m_strVerifyCode);
	DDX_CBIndex(pDX, IDC_COMBO_TICKETTYPE, m_nTicketType);
	DDX_CBString(pDX, IDC_COMBO_STARTDATE, m_strStartDate);
	DDX_Text(pDX, IDC_EDIT_MOBILE, m_strMobile);
	DDX_Text(pDX, IDC_EDIT_TMGETTICKET, m_uiTmGetTicket);
	DDX_Text(pDX, IDC_EDIT_TMSUBMITTICKET, m_uiTmSubmitTicket);
	DDX_CBString(pDX, IDC_COMBO_STARTTIME, m_strStartTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetTicketDlg, CDialog)
	//{{AFX_MSG_MAP(CGetTicketDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_LIST_MAN, OnClickListMan)
	ON_BN_CLICKED(IDC_BTN_RUNNING, OnBtnRunning)
	ON_BN_CLICKED(IDC_BTN_RCODE, OnBtnRcode)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_GETTICKET, OnBtnGetticket)
	ON_EN_CHANGE(IDC_EDIT_RCODE, OnChangeEditRcode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetTicketDlg message handlers

BOOL CGetTicketDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// add you code
	CenterWindow();
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	// ��ʼ��
	m_cbtn.SubclassWindow(GetDlgItem(IDC_BTN_RCODE)->m_hWnd);
	InitParam();
	InitManList();
	InitTicketList();
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(FALSE);

	m_hdWaitCode = CreateEvent(NULL, FALSE, FALSE, _T("RandCode"));

	// ��ȡ�˿�
	AfxBeginThread(GetPassernger, this);

	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGetTicketDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGetTicketDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGetTicketDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGetTicketDlg::AddInfo(const char* fmt, ...)
{
	char szinfo[4096] = {0};
	va_list ap;
	va_start(ap, fmt);
	vsprintf(szinfo, fmt, ap);
	va_end(ap);
	
	static CString strShowInfo;
	if (strShowInfo.IsEmpty())
	{
		GetDlgItemText(IDC_ST_INFO, strShowInfo);
	}
	if (strShowInfo.GetLength() > 1024000)
	{
		strShowInfo.Empty();
	}
	string strrowinfo = date();
	strrowinfo += " ";
	strrowinfo += szinfo;
	
	strShowInfo += strrowinfo.c_str();
	strShowInfo += "\r\n";
	SetDlgItemText(IDC_ST_INFO, strShowInfo);
	m_cInfo.LineScroll(m_cInfo.GetLineCount());
	
	// д��־
	write_to_file(g_strapppath  + "\\" + date("Ymd") + ".txt", strrowinfo);
}


void CGetTicketDlg::InitParam()
{
	string strret = get_from_file(g_strapppath + "\\station_name.txt");
	if (!strret.empty())
	{
		vector<string> citylist;
		vector<string> cityinfo;
		int icount = str_explode(strret, "@", citylist);
		for (int i = 1; i < icount; ++i)
		{
			cityinfo.clear();
			int j = str_explode(citylist[i], "|", cityinfo);
			if (j < 6) continue;
			mapstrcity[cityinfo[1]] = cityinfo[2];
		}
	} 

	char sztmp[MAX_BUFFER_BLOCK + 1] = {0};
	CMemory_Conf::instance()->get_conf_str("����", "������", "", sztmp, MAX_BUFFER_BLOCK);
	m_strStartCity = sztmp;
	CMemory_Conf::instance()->get_conf_str("����", "Ŀ�ĵ�", "", sztmp, MAX_BUFFER_BLOCK);
	m_strEndCity = sztmp;

	
	for (int i = 0; i < uigetday; ++i)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_STARTDATE))->AddString(date("Y-m-d", i).c_str());
	}
	CMemory_Conf::instance()->get_conf_str("����", "��������", date("Y-m-d").c_str(), sztmp, MAX_BUFFER_BLOCK);
	m_strStartDate = sztmp;
	if (m_strStartDate.IsEmpty() || m_strStartDate.CompareNoCase(date("Y-m-d").c_str()) == -1)
	{
		m_strStartDate = date("Y-m-d").c_str();
	}

	CMemory_Conf::instance()->get_conf_str("����", "����ʱ��", "00:00--24:00", sztmp, MAX_BUFFER_BLOCK);
	m_strStartTime = sztmp;
	if (m_strStartTime.IsEmpty())
	{
		m_strStartTime = "00:00--24:00";
	}
	

	CMemory_Conf::instance()->get_conf_str("����", "����", "", sztmp, MAX_BUFFER_BLOCK);
	m_strTrainNo = sztmp;

	CMemory_Conf::instance()->get_conf_str("����", "����", "", sztmp, MAX_BUFFER_BLOCK);
	m_strName = sztmp;
	CMemory_Conf::instance()->get_conf_str("����", "���֤", "", sztmp, MAX_BUFFER_BLOCK);
	m_strVerifyCode = sztmp;
	m_nTicketType = CMemory_Conf::instance()->get_conf_int("����", "Ʊ��", 1);

	m_nSeatType = CMemory_Conf::instance()->get_conf_int("����", "��ϯ", 1);	
	if (m_nSeatType > 0)
	{
		usseatidx = m_nSeatType - 1;
		usselseat = m_nSeatType - 1;
	}

	CMemory_Conf::instance()->get_conf_str("����", "�ֻ���", "", sztmp, MAX_BUFFER_BLOCK);
	m_strMobile = sztmp;

	m_uiTmGetTicket = CMemory_Conf::instance()->get_conf_int("����", "ˢƱ", 5);
	m_uiTmSubmitTicket = CMemory_Conf::instance()->get_conf_int("����", "�ᵥ", 3);

	replace(trainClass, "#", "%23");

	UpdateData(FALSE);
}

void CGetTicketDlg::InitManList()
{
	LONG lStyle; 
	lStyle = GetWindowLong(m_listMan.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ 
	lStyle |= LVS_REPORT; //����style 	
	SetWindowLong(m_listMan.m_hWnd, GWL_STYLE, lStyle);//����style 
	DWORD dwStyle = m_listMan.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl�� 
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl�� 
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ� 
	m_listMan.SetExtendedStyle(dwStyle); //������չ��� 
	
	//m_listMan.InsertColumn(0, _T("Y/N"), LVCFMT_CENTER, 30, 0);
	m_listMan.InsertColumn(0, _T("����"), LVCFMT_LEFT, 50, 0);
	m_listMan.InsertColumn(1, _T("�������֤"), LVCFMT_LEFT, 126, 0);

	/*/ test data
	m_listMan.InsertItem(0, "����");
	m_listMan.SetItemText(0, 1, "433423199303131413");

	m_listMan.InsertItem(1, "����");
	m_listMan.SetItemText(1, 1, "433423199303131413"); //*/
}

void CGetTicketDlg::InitTicketList()
{
	LONG lStyle; 
	lStyle = GetWindowLong(m_listTicket.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ 
	lStyle |= LVS_REPORT; //����style 
	lStyle |= LVS_NOSCROLL; //���ع�����
	SetWindowLong(m_listTicket.m_hWnd, GWL_STYLE, lStyle);//����style 
	DWORD dwStyle = m_listTicket.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl�� 
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl�� 
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ� 
	m_listTicket.SetExtendedStyle(dwStyle); //������չ��� 
	//m_listTicket.ShowScrollBar(SB_HORZ, FALSE); // ����ˮƽ������
	
	int i = 0;
	m_listTicket.InsertColumn(i++, _T("����"), LVCFMT_LEFT, 40, 0);
	m_listTicket.InsertColumn(i++, _T("(ʼ)��վ(ʱ��)"), LVCFMT_CENTER, 104, 0);
	m_listTicket.InsertColumn(i++, _T("(ʼ)��վ(ʱ��)"), LVCFMT_CENTER, 104, 0);
	m_listTicket.InsertColumn(i++, _T("��ʱ"), LVCFMT_CENTER, 42, 0);
	m_listTicket.InsertColumn(i++, _T("������"), LVCFMT_CENTER, 50, 0);
	m_listTicket.InsertColumn(i++, _T("�ص���"), LVCFMT_CENTER, 50, 0);
	m_listTicket.InsertColumn(i++, _T("һ����"), LVCFMT_CENTER, 50, 0);
	m_listTicket.InsertColumn(i++, _T("������"), LVCFMT_CENTER, 50, 0);
	m_listTicket.InsertColumn(i++, _T("�߼�����"), LVCFMT_CENTER, 60, 0);
	m_listTicket.InsertColumn(i++, _T("����"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("Ӳ��"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("����"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("Ӳ��"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("����"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("����"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("��Ʊ"), LVCFMT_CENTER, 36, 0);
	
	/*/ test data
	int j = 1;
	m_listTicket.InsertItem(0, _T("K9122"));
	m_listTicket.SetItemText(0, j++, "(ʼ) ���� 21:43");
	m_listTicket.SetItemText(0, j++, "(��) ���� 06:21");
	m_listTicket.SetItemText(0, j++, "09:12");
	m_listTicket.SetItemText(0, j++, "--");
	m_listTicket.SetItemText(0, j++, "--");
	m_listTicket.SetItemText(0, j++, "--");
	m_listTicket.SetItemText(0, j++, "--");
	m_listTicket.SetItemText(0, j++, "--");
	m_listTicket.SetItemText(0, j++, "--");
	m_listTicket.SetItemText(0, j++, "132");
	m_listTicket.SetItemText(0, j++, "--");
	m_listTicket.SetItemText(0, j++, "1024");
	m_listTicket.SetItemText(0, j++, "��");
	m_listTicket.SetItemText(0, j++, "--");
	m_listTicket.SetItemText(0, j++, "1"); //*/
	
}

void CGetTicketDlg::OnClickListMan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	int idx = m_listMan.GetSelectionMark();
	if (idx == -1)
	{
		return;
	}	
	m_iSelMan = idx;
	m_strName = m_listMan.GetItemText(idx, 0);
	m_strVerifyCode = m_listMan.GetItemText(idx, 1);
	SetDlgItemText(IDC_EDIT_NAME, m_strName);
	SetDlgItemText(IDC_EDIT_VERIFYCODE, m_strVerifyCode);

	*pResult = 0;
}



void CGetTicketDlg::OnBtnRunning() 
{
	// TODO: Add your control notification handler code here
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	if (m_bRunning)
	{
		SetDlgItemText(IDC_BTN_RUNNING, "��ʼ");
		m_bRunning = FALSE;		
		SetEvent(m_hdWaitCode); //�˳�ԭ���߳�
		return;
	}	

	if (!UpdateData())
	{
		return;
	}
	
	// ��ʼ�� Ŀ�ĵ�
	if (m_strStartCity.IsEmpty() || m_strEndCity.IsEmpty())
	{
		AfxMessageBox("��ʼ�ػ�Ŀ�ĵز���Ϊ��");
		return;
	}
	startcity = m_strStartCity;
	endcity = m_strEndCity;
	startcitycode = mapstrcity[startcity];
	endcitycode = mapstrcity[endcity];
	if (startcitycode.empty() || endcitycode.empty())
	{
		AfxMessageBox("��ʼ���л���Ŀ�ĳ�����������");
		return;
	}

	// ��������
	if (m_strStartDate.IsEmpty())
	{
		AfxMessageBox("�������ڲ���Ϊ��");
		return;
	}
	startdate = m_strStartDate;

	// ����ʱ��
	if (m_strStartTime.IsEmpty())
	{
		AfxMessageBox("����ʱ�䲻��Ϊ��");
		return;
	}
	startTime = m_strStartTime;	
	replace(startTime, ":", "%3A");

	// ����
	trainCodeText = m_strTrainNo;

	// ���� ���֤����
	if (m_strName.IsEmpty() || m_strVerifyCode.IsEmpty())
	{
		AfxMessageBox("��Ʊ�����������֤����Ϊ��");
		return;
	}

	// Ʊ��
	if (m_nTicketType == 0)
	{
		AfxMessageBox("��ѡ��Ʊ��");
		return;
	}

	// ��ϯ
	if (m_nSeatType == 0)
	{
		usseatidx = 10;
		usselseat = 0;
	}
	else
	{
		usseatidx = m_nSeatType - 1;
		usselseat = m_nSeatType - 1;
		strseattext = pszseattext[m_nSeatType - 1];
	}

	// ����������Ϣ
	CMemory_Conf::instance()->write_conf_str("����", "������", m_strStartCity);
	CMemory_Conf::instance()->write_conf_str("����", "Ŀ�ĵ�", m_strEndCity);
	CMemory_Conf::instance()->write_conf_str("����", "��������", m_strStartDate);
	CMemory_Conf::instance()->write_conf_str("����", "����ʱ��", m_strStartTime);
	CMemory_Conf::instance()->write_conf_str("����", "����", m_strTrainNo);
	CMemory_Conf::instance()->write_conf_str("����", "����", m_strName);
	CMemory_Conf::instance()->write_conf_str("����", "���֤", m_strVerifyCode);
	CMemory_Conf::instance()->write_conf_str("����", "�ֻ���", m_strMobile);
	CMemory_Conf::instance()->write_conf_int("����", "Ʊ��", m_nTicketType);
	CMemory_Conf::instance()->write_conf_int("����", "��ϯ", m_nSeatType);
	CMemory_Conf::instance()->write_conf_int("����", "ˢƱ", m_uiTmGetTicket);
	CMemory_Conf::instance()->write_conf_int("����", "�ᵥ", m_uiTmSubmitTicket);
	CMemory_Conf::instance()->save_conf();
	
	m_bRunning = TRUE;	
	SetDlgItemText(IDC_BTN_RUNNING, "ֹͣ");
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(FALSE);
	m_nGetTicketTimes = 0;
	AfxBeginThread(GetTickets, this);
}

UINT CGetTicketDlg::GetPassernger(LPVOID lpVoid)
{
	Json::Reader jread;
	Json::Value jvalue;
	Json::Value jlist;
	Json::Value jlistitem;
	int i,j,k,m,n,icount;
	int iret = 0;
	bool bret = false;
	CQMap cmap;
	string strret;
	string strack;
	string strtmp;
	char sztmp[MAX_BUFFER_BLOCK + 1] = {0};

	CGetTicketDlg* pT = (CGetTicketDlg* )lpVoid;
	
	map<string, TRAIN_PASSENGER_INFO> mappassengers; // ֤������=>������Ϣ
	TRAIN_PASSENGER_INFO stper_passenger; // ������Ϣ
	TRAIN_PASSENGER_LIST trainpassengers;
	
	memset(&trainpassengers, 0, sizeof(TRAIN_PASSENGER_LIST));
	pT->m_listMan.DeleteAllItems();
	//cmap.cmd(_12306_GETTRAIN_PASSENGER);
	iret = creq->http_post("/otn/passengers/query", "pageIndex=1&pageSize=50", strack);
	// ��ȡ�˿�����
	/*strack = getmidstr(strack, "var passengers=", ";");
	strack = str_replace(strack, "'", "\"");
	strack = str_replace(strack, "\\u", "\u");

	// Unicode to Ascii
	char szbuff[4096] = {0};
	unicode_to_utf(strack, szbuff, 4096);
	strack = szbuff;
	CHandleCode::UTF8ToGBK(strack);

	strack = str_format("{\"passengerJson\":%s}", strack.c_str());*/
	if (jread.parse(strack, jvalue))
	{
		if (!jvalue["data"].isNull())
		{
			jlist = jvalue["data"]["datas"];
			if (jlist.isArray())
			{
				pT->AddInfo("���ڻ�ȡ��ϵ��....");
				icount = jlist.size();
				for (int i = 0; i < icount && i < MAX_TRAIN_PASSENGER; ++i)
				{
					
					jlistitem = jlist[i];
					++trainpassengers.uscount;
					strncpy(trainpassengers.stpassenger[i].szname, jlistitem["passenger_name"].asCString(), MAX_PASSENGER_NAME);
					trainpassengers.stpassenger[i].ustype = jlistitem["passenger_type"].toInt();
					strncpy(trainpassengers.stpassenger[i].szmobile, jlistitem["mobile_no"].asCString(), MAX_MOBILE_LEN);
					strncpy(trainpassengers.stpassenger[i].szidtype, jlistitem["passenger_id_type_code"].asCString(), MAX_TRAIN_NO); // ֤������
					strncpy(trainpassengers.stpassenger[i].szidno, jlistitem["passenger_id_no"].asCString(), MAX_PASSENGER_IDNO);
					
					/*printf("%d. %s %s %s %s %s\n"
						, trainpassengers.uscount
						, trainpassengers.stpassenger[i].szname
						, pszpassenger[trainpassengers.stpassenger[i].ustype - 1]
						, trainpassengers.stpassenger[i].szidtype
						, trainpassengers.stpassenger[i].szidno
						, trainpassengers.stpassenger[i].szmobile); //*/
					pT->m_listMan.InsertItem(i, trainpassengers.stpassenger[i].szname);
					pT->m_listMan.SetItemText(i, 1, trainpassengers.stpassenger[i].szidno);
					
					memcpy(&stper_passenger, &trainpassengers.stpassenger[i], sizeof(stper_passenger));
					mappassengers[trainpassengers.stpassenger[i].szidno] = stper_passenger;
					memset(&stper_passenger, 0, sizeof(stper_passenger));
				}
			}			
		}
	}
	pT->AddInfo("�ɹ���ȡ %d λ��ϵ��", mappassengers.size());	
	return 0;
}

UINT CGetTicketDlg::GetTickets(LPVOID lpVoid)
{
	Json::Reader jread;
	Json::Value jvalue;
	Json::Value jlist;
	Json::Value jlistitem;
	int i,j,k,m,n,icount;
	int iret = 0;
	bool bret = false;
	CQMap cmap;	
	string strret;
	string strack;
	string strtmp;
	char sztmp[MAX_BUFFER_BLOCK + 1] = {0};

	CGetTicketDlg* pt = (CGetTicketDlg* )lpVoid;
	
while (pt->m_bRunning)
{
	if (pt->m_nGetTicketTimes > 0)
	{
		WaitForSingleObject(pt->m_hdWaitCode, pt->m_uiTmGetTicket * 1000);
	}	
	if (pt->m_bRunning == FALSE)
	{
		break;
	}
		// TODO: Add your message handler code here and/or call default
	// ˢƱ
	// ��ѯ���δ��� train_no->id�ݲ�ʵ�֣�ֱ��ͨ����ѯ�����г�����ɸѡ
	//https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=queryststrainall
	//date=2013-10-03&fromstation=SZQ&tostation=HYQ&starttime=00%3A00--24%3A00
	//[{"end_station_name":"�人","end_time":"11:52","id":"6i000G100201","start_station_name":"���ڱ�","start_time":"07:00","value":"G1002"},{}]

	// ��ӡ��ѯ����
	++pt->m_nGetTicketTimes;
	pt->AddInfo("ȷ����Ϣ��\r\n������: %s Ŀ�ĵ�: %s �������ڣ�%s ����ʱ�䣺%s\r\n"
		"��������: %s\r\n"
		"�˿�������%-8s ���֤��%s\r\n"
		"��ϯ��%s\r\n"
		"��ʼ���е�%d�β�ѯ"
		, startcity.c_str(), endcity.c_str(), startdate.c_str(), pt->m_strStartTime
		, (trainCodeText.empty() ? "�г�" : trainCodeText.c_str())
		, pt->m_strName, pt->m_strVerifyCode
		, (strseattext.empty() ? "����" : strseattext.c_str())
		, pt->m_nGetTicketTimes);
	
	/*cmap.clear();
	cmap.cmd(_12306_LOAD_DATA);	
	cmap["method"] = "qt"; // step 1
	cmap["path"] = str_format("&orderRequest.train_date=%s&orderRequest.from_station_telecode=%s"
		"&orderRequest.to_station_telecode=%s&orderRequest.train_no=%s&trainPassType=%s&trainClass=%s"
		"&includeStudent=%s&seatTypeAndNum=%s&orderRequest.start_time_str=%s"
		, startdate.c_str(), startcitycode.c_str(), endcitycode.c_str(), train_no.c_str()
		, trainPassType.c_str(), trainClass.c_str(), includeStudent.c_str(), seatTypeAndNum.c_str(), startTime.c_str());
	iret = creq->parse(cmap, strack);*/

	strtmp = str_format("/otn/leftTicket/query?leftTicketDTO.train_date=%s&leftTicketDTO.from_station=%s&leftTicketDTO.to_station=%s&purpose_codes=%s"
		, startdate.c_str(), startcitycode.c_str(), endcitycode.c_str(), includeStudent.c_str());
	iret = creq->http_get(strtmp, strack);
	
	//write_to_file(g_strapppath + "\\qt.html", strack, false);	
	
	/*if (strack.empty())
	{
		cmap["method"] = "queryLeftTicket"; // step 2
		iret = creq->parse(cmap, strack);
	}*/
	//write_to_file(g_strapppath + "\\queryLeftTicket.html", strack, false);
	
	pt->m_listTicket.DeleteAllItems();
	pt->m_listTicket.ShowScrollBar(SB_VERT, FALSE);

	if (strack.empty())
	{
		pt->AddInfo("δ��ѯ���κ���Ϣ�������ѯ�����Ƿ���ȷ");
		continue;
	}

	// parse json data
	if (!jread.parse(strack, jvalue))
	{
		pt->AddInfo("��ѯ�����쳣");
		continue;
	}

	// ���������쳣


	// ��������
	QUERY_DATA_LIST stqtrains;
	memset(&stqtrains, 0, sizeof(stqtrains));
	jlist = jvalue["data"];
	if (jlist.isArray())
	{		
		icount = jlist.size();
		for (i = 0; i < icount && i < MAX_LOAD_DATA; ++i)
		{
			jlistitem = jlist[i]["queryLeftNewDTO"];
			++stqtrains.uscount;
			strncpy(stqtrains.stlist[i].sztrain_no, jlistitem["station_train_code"].asCString(), MAX_TRAIN_NO); // 
			
			strncpy(stqtrains.stlist[i].szstartcity, jlistitem["from_station_name"].asCString(), MAX_CITY_LEN); // 
			stqtrains.stlist[i].bstart = strcmp(jlistitem["start_station_name"].asCString(), jlistitem["from_station_name"].asCString()) == 0 ? 1 : 0; // 
			strncpy(stqtrains.stlist[i].szstarttime, jlistitem["start_time"].asCString(), MAX_TIME_LEN); // 
			
			strncpy(stqtrains.stlist[i].szendcity, jlistitem["to_station_name"].asCString(), MAX_CITY_LEN); // 
			stqtrains.stlist[i].bend = strcmp(jlistitem["end_station_name"].asCString(), jlistitem["to_station_name"].asCString()) == 0 ? 1 : 0; // 
			strncpy(stqtrains.stlist[i].szendtime, jlistitem["arrive_time"].asCString(), MAX_TIME_LEN); // 

			strncpy(stqtrains.stlist[i].szusetime, jlistitem["lishi"].asCString(), MAX_TIME_LEN); // 
			
			k = 0; // 
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["swz_num"].asCString());
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["tz_num"].asCString());
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["zy_num"].asCString());
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["ze_num"].asCString());
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["gr_num"].asCString());
			
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["rw_num"].asCString());
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["yw_num"].asCString());
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["rz_num"].asCString());
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["yz_num"].asCString());
			
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["wz_num"].asCString());
			strcpy(stqtrains.stlist[i].szTicket[k++], jlistitem["qt_num"].asCString());

			if (stricmp("Y", jlistitem["canWebBuy"].asCString()) == 0)
			{
				stqtrains.stlist[i].bsubmit = 1;
				strncpy(stqtrains.stlist[i].szsubmitcode, (jlist[i]["secretStr"]).asCString(), SUBMIT_CODE_LEN);
			}

			pt->m_listTicket.InsertItem(i, stqtrains.stlist[i].sztrain_no);
			pt->m_listTicket.SetItemText(i, 1, str_format("%s%-6s(%s)", stqtrains.stlist[i].bstart ? "Y " : "  "
				, stqtrains.stlist[i].szstartcity, stqtrains.stlist[i].szstarttime).c_str());
			pt->m_listTicket.SetItemText(i, 2, str_format("%s%-6s(%s)", stqtrains.stlist[i].bend ? "Y " : "  "
				, stqtrains.stlist[i].szendcity, stqtrains.stlist[i].szendtime).c_str());
			pt->m_listTicket.SetItemText(i, 3, stqtrains.stlist[i].szusetime);
			for (n = 0; n < TRAIN_TICKET_TYPE; ++n)
			{
				pt->m_listTicket.SetItemText(i, 3 + n + 1, stqtrains.stlist[i].szTicket[n]);
			}
			pt->m_listTicket.SetItemText(i, 3 + n + 1, stqtrains.stlist[i].bsubmit ? "Y" : "N");
		}
	}

	if (1)
	{
		continue;
	}
	

	/*if (strack == "-10")
	{
		// ���µ�½
		AfxMessageBox("����û�е�¼�����뿪ҳ���ʱ����������¼ϵͳ����ˢ��ҳ��");
		pt->alertexit();
	}
	if (strack == "-1")
	{
		AfxMessageBox("������æ�����ز�ѯ����ʧ�ܣ�");	
		continue;
	}
	else if (strack.substr(0, 2) == "-2")
	{
		pt->AddInfo(strack.substr(3).c_str());
		write_to_file(strerrorfile, strack);
		continue;
	}
	else
	{
		strack = str_replace(strack, "&nbsp;", "");
		strack = str_replace(strack, "<br>", ",");
	}*/

	// -- begin ����ֶ� --
	/*int itraincount = 0;
	vector<string> vectraininfo;
	QUERY_DATA_LIST stqtrains;
	memset(&stqtrains, 0, sizeof(stqtrains));
	itraincount = str_explode(strack, "\\n", vectraininfo);
	if (itraincount > 0)
	{
		//printf("���� (ʼ)��վ(ʱ��) (��)��վ(ʱ��) ��ʱ ������ �ص��� һ���� ������ �߼����� ���� Ӳ�� ���� Ӳ�� ���� ���� ��Ʊ\n");
		vector<string> vectrain;
		for (i = 0; i < itraincount && i < MAX_LOAD_DATA; ++i)
		{
			vectrain.clear();
			j = str_explode(vectraininfo[i], ",", vectrain);
			if (j < 19) continue;
			++stqtrains.uscount;
			strncpy(stqtrains.stlist[i].sztrain_no, getmidstr(vectrain[1], "onStopOut()'>", "<").c_str(), MAX_TRAIN_NO);
			if (vectrain[2][0] == '<') // ��վ
			{
				stqtrains.stlist[i].bstart = 1;
				strncpy(stqtrains.stlist[i].szstartcity, vectrain[2].substr(vectrain[2].find(">") + 1).c_str(), MAX_CITY_LEN);
			}
			else
			{
				strncpy(stqtrains.stlist[i].szstartcity, vectrain[2].c_str(), MAX_CITY_LEN);
			}
			strncpy(stqtrains.stlist[i].szstarttime, vectrain[3].c_str(), MAX_TIME_LEN);
			if (vectrain[4][0] == '<') // ��վ
			{
				stqtrains.stlist[i].bend = 1;
				strncpy(stqtrains.stlist[i].szendcity, vectrain[4].substr(vectrain[4].find(">") + 1).c_str(), MAX_CITY_LEN);
			}
			else
			{
				strncpy(stqtrains.stlist[i].szendcity, vectrain[4].c_str(), MAX_CITY_LEN);
			}
			strncpy(stqtrains.stlist[i].szendtime, vectrain[5].c_str(), MAX_TIME_LEN);
			strncpy(stqtrains.stlist[i].szusetime, vectrain[6].c_str(), MAX_TIME_LEN);
			m =  6 + TRAIN_TICKET_TYPE;
			for (n = m - TRAIN_TICKET_TYPE + 1, k = 0; n <= m; ++n, ++k) // Ʊ����Ϣ
			{
				j = vectrain[n][0];
				if (j == '-')
				{
					strcpy(stqtrains.stlist[i].szTicket[k], "--");
				}
				else if (j == '<') //<font color='#008800'>��</font>,<font color='darkgray'>��</font>
				{
					strcpy(stqtrains.stlist[i].szTicket[k], getmidstr(vectrain[n], ">", "<").c_str());
				}
				else
				{
					strncpy(stqtrains.stlist[i].szTicket[k], vectrain[n].c_str(), TICKET_SIGN_LEN);
				}
			}
			if (vectrain[n].find("onclick") != string::npos) // �����οɷ�Ʊ
			{
				stqtrains.stlist[i].bsubmit = 1;
				strncpy(stqtrains.stlist[i].szsubmitcode, getmidstr(vectrain[n], "getSelected('", "')").c_str(), SUBMIT_CODE_LEN);
			}

			// ���� (ʼ)����(09:00) (��)����(19:00) ��ʱ ������ �ص��� һ���� ������ �߼����� ���� Ӳ�� ���� Ӳ�� ���� ���� ��Ʊ
			printf("%5s %s%-6s(%s) %s%-6s(%s) %s"
				, stqtrains.stlist[i].sztrain_no
				, stqtrains.stlist[i].bstart ? "(ʼ)" : "    "
				, stqtrains.stlist[i].szstartcity
				, stqtrains.stlist[i].szstarttime
				, stqtrains.stlist[i].bend ? "(��)" : "    "
				, stqtrains.stlist[i].szendcity
				, stqtrains.stlist[i].szendtime
				, stqtrains.stlist[i].szusetime);
			for (n = 0; n < TRAIN_TICKET_TYPE; ++n)
			{
				printf(" %2s", stqtrains.stlist[i].szTicket[n]);
			}
			if (stqtrains.stlist[i].bsubmit)
			{
				printf(" ��Ʊ");
			}
			printf("\n"); //						
		}
	}*/
	// -- end ����ֶ� --

	// ��ʾ������
	if (stqtrains.uscount > MAX_TICKET_LENGTHS)
	{
		pt->m_listTicket.ShowScrollBar(SB_VERT, TRUE);
	}	

	// �г�������
	if (stqtrains.uscount > 0)
	{
		// ɸѡָ�����Σ����ȿɶ�Ʊ����γ���(��ȫƥ��ת��д)
		for (i = 0; i < stqtrains.uscount; ++i)
		{
			if (!stqtrains.stlist[i].bsubmit)
			{
				continue;
			}
			if (trainCodeText.empty() || stricmp(stqtrains.stlist[i].sztrain_no, trainCodeText.c_str()) == 0)
			{
				// ƥ����λ Ĭ��Ӳ�� usseatidx = 8 // ѡ����ϯ��Ʊ����ʣ�������
				if (usseatidx == 10) // ��Ʊ�Ͷ�
				{
					for (j = 0; j < TICKET_SIGN_LEN; ++j)
					{
						if (strcmp(stqtrains.stlist[i].szTicket[j], "��") == 0  || atoi(stqtrains.stlist[i].szTicket[j]) > 0)
						{
							usselseat = j;
							break;
						}
					}
				}

				if (strcmp(stqtrains.stlist[i].szTicket[usselseat], "��") == 0  || atoi(stqtrains.stlist[i].szTicket[usselseat]) > 0)
				{
					// ƥ�����
					//if (strcmp(stqtrains.stlist[i].szstartcity, startcity.c_str()) == 0 && strcmp(stqtrains.stlist[i].szendcity, endcity.c_str()) == 0)
					{
						pt->AddInfo("��%s����%s=>%s����%s - %s����%s����%s��%s, ��ʼԤ��..\n"
							, stqtrains.stlist[i].sztrain_no
							, stqtrains.stlist[i].szstartcity, stqtrains.stlist[i].szendcity
							, stqtrains.stlist[i].szstarttime, stqtrains.stlist[i].szendtime
							, stqtrains.stlist[i].szusetime
							, pszseattext[usselseat]
							, atoi(stqtrains.stlist[i].szTicket[usselseat]) > 0 ? (string(stqtrains.stlist[i].szTicket[usselseat]) + "��").c_str() : "����");
						break;
					}					
				}
			}
		}
		if (i != stqtrains.uscount)
		{
			// ƥ�䵽���λ��߲�ѯ����Ʊ��ת��Ԥ��ҳ��
			vector<string> getorder;			
			n = sizeof(pszorderparam)/sizeof(char*);
			m = str_explode(stqtrains.stlist[i].szsubmitcode, "#", getorder);
			// if (m < n) 14����Ҫ����
			cmap.clear();
			cmap.cmd(_12306_PREPARE_ORDER_DATA);
			for (j = 0; j < m; ++j)
			{
				cmap.set(pszorderparam[j], getorder[j]);
			}
			string strsc = stqtrains.stlist[i].szstartcity;
			string strec = stqtrains.stlist[i].szendcity;
			cmap["train_date"] = startdate;
			cmap["seattype_num"] = "";
			cmap["include_student"] = includeStudent;
			cmap["from_station_telecode_name"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(strsc));
			cmap["to_station_telecode_name"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(strec));
			cmap["round_train_date"] = startdate;
			cmap["round_start_time_str"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(startTime));
			cmap["single_round_type"] = "1"; // ����
			cmap["train_pass_type"] = trainPassType;
			cmap["train_class_arr"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(trainClass));
			cmap["start_time_str"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(startTime));
			cmap["lishi"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(cmap["lishi"]));
			cmap["train_start_time"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(cmap["train_start_time"]));
			cmap["arrive_time"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(cmap["arrive_time"]));
			cmap["from_station_name"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(strsc));
			cmap["to_station_name"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(strec));
			cmap["mmStr"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(cmap["mmStr"]));
			cmap["NTEzODczOQ%3D%3D"] = "NTI4MDE0MWVjMTZmY2YxOQ%3D%3D";
			cmap["myversion"] = "undefined";
			strtmp = str_format("station_train_code=%s&train_date=%s&seattype_num=&from_station_telecode=%s"
				"&to_station_telecode=%s&include_student=%s&from_station_telecode_name=%s&to_station_telecode_name=%s"
				"&round_train_date=%s&round_start_time_str=%s&single_round_type=%s&train_pass_type=%s"
				"&train_class_arr=%s&start_time_str=%s&lishi=%s&train_start_time=%s"
				"&trainno4=%s&arrive_time=%s&from_station_name=%s&to_station_name=%s"
				"&from_station_no=%s&to_station_no=%s&ypInfoDetail=%s"
				"&mmStr=%s"
				"&locationCode=%s"
				, cmap["station_train_code"].c_str(), cmap["train_date"].c_str(), cmap["from_station_telecode"].c_str()
				, cmap["to_station_telecode"].c_str(), includeStudent.c_str(), cmap["from_station_telecode_name"].c_str(), cmap["to_station_telecode_name"].c_str()
				, cmap["round_train_date"].c_str(), cmap["round_start_time_str"].c_str(), cmap["single_round_type"].c_str(), cmap["train_pass_type"].c_str()
				, cmap["train_class_arr"].c_str(), cmap["start_time_str"].c_str(), cmap["lishi"].c_str(), cmap["train_start_time"].c_str()
				, cmap["trainno4"].c_str(), cmap["arrive_time"].c_str(), cmap["from_station_name"].c_str(), cmap["to_station_name"].c_str()
				, cmap["from_station_no"].c_str(), cmap["to_station_no"].c_str(), cmap["ypInfoDetail"].c_str()
				, cmap["mmStr"].c_str()
				, cmap["locationCode"].c_str());
			//std::cout << strtmp << std::endl;
			cmap["data"] = strtmp;
			iret = creq->parse(cmap, strack);
			//write_to_file(g_strapppath + "\\submitOrder.html", strack, false);
			pt->AddInfo("��ת��Ʊҳ�桾%s��", strack.empty() ? "�ɹ�" : "ʧ��");
			
			// ��תԤ��ҳ��
			cmap.cmd(_12306_PASSENGER_ACTION_INIT);			
			iret = creq->parse(cmap, strack);
			if (strack.empty())
			{
				pt->AddInfo("���ض�Ʊҳ��ʧ��");
				continue;
			}
			
			// ��ȡ strack ����, ��ӡƱ����Ϣ: ʵʱ�仯�Ľ����ο�
			strret = getmidstr(strack, "<table", "</table>"); // ƱԴ��Ϣ
			strret = str_replace(strret, " ", "");
			strret = str_replace(strret, "\t", "");
			strret = getmidstr(strret, "</td></tr><tr><td>", "</td></tr><tr><td");
			strret = str_replace(strret, "</td><td>", "  ");
			pt->AddInfo("ƱԴ��Ϣ��%s\r\n          ������Ʊ��Ϣ��ʱ�����仯�������ο�", strret.c_str());
			
			// ��֤: ��֯Post����
			// ��ӡ�˿���Ϣ��ȷ���������ύ------------
			cmap.clear();
			cmap["checkbox0"] = "0&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y"
				"&textfield=%E4%B8%AD%E6%96%87%E6%88%96%E6%8B%BC%E9%9F%B3%E9%A6%96%E5%AD%97%E6%AF%8D";
			strret = getmidstr(strack, "<form name=\"save_passenger_single\"", "</form>");
			//write_to_file(g_strapppath + "\\�ύ����ҳ������.txt", strret, false);
			map<string,string> mappm;
			icount = str_getformsubmit(strret, mappm, pszsubmitconfirm, sizeof(pszsubmitconfirm) / sizeof(char* ));
			/*for (map<string,string>::const_iterator it = mappm.begin(); it != mappm.end(); ++it)
			{
				cout << it->first << " = " << it->second << endl;
			}*/
			cmap.add(mappm);
			// Ŀǰ֧�ֶ�Ʊһ�� Ӳ��[Ĭ��] ����
			cmap["passenger_1_cardno"] = pt->m_strVerifyCode;//strpassenger_cardno;
			cmap["passenger_1_cardtype"] = "1";//mappassengers[strpassenger_cardno].szidtype; �ڶ������֤
			cmap["passenger_1_mobileno"] = pt->m_strMobile;//mappassengers[strpassenger_cardno].szmobile;
			cmap["passenger_1_name"] = pt->m_strName;//mappassengers[strpassenger_cardno].szname;
			cmap["passenger_1_seat"] = pszseattype[usselseat]; // ��ϯ
			cmap.set("passenger_1_ticket", pt->m_nTicketType/*mappassengers[strpassenger_cardno].ustype */); // ����Ʊ

			strtmp = str_format("%s,0,%s,%s,%s,%s,%s,Y", cmap["passenger_1_seat"].c_str()
				, cmap["passenger_1_ticket"].c_str()
				, cmap["passenger_1_name"].c_str()
				, cmap["passenger_1_cardtype"].c_str()
				, cmap["passenger_1_cardno"].c_str()
				, cmap["passenger_1_mobileno"].c_str());
			cmap["passengerTickets"] = strtmp;

			strtmp = str_format("%s,%s,%s", cmap["passenger_1_name"].c_str()
				, cmap["passenger_1_cardtype"].c_str()
				, cmap["passenger_1_cardno"].c_str());
			cmap["oldPassengers"] = strtmp;

			cmap["orderRequest.reserve_flag"] = "A";
			cmap["tFlag"] = "dc";
			// ����
			cmap["orderRequest.start_time"] = CHandleCode::UrlEncode(cmap["orderRequest.start_time"]);
			cmap["orderRequest.end_time"] = CHandleCode::UrlEncode(cmap["orderRequest.end_time"]);
			cmap["orderRequest.from_station_name"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(strsc));
			cmap["orderRequest.to_station_name"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(strec));
			cmap["passengerTickets"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(cmap["passengerTickets"]));
			cmap["passenger_1_name"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(cmap["passenger_1_name"]));
			cmap["oldPassengers"] = CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(cmap["oldPassengers"]));	

			// Ԥ��ҳ�濪ʼ��Ʊ����
			time_t tm_current = time(NULL);
			time_t tm_space = tm_current;
			do 
			{
				do
				{
					if (!jvalue["errMsg"].isNull())
					{
						//������ܴ������⣬�����ԣ�
						if (strcmp(jvalue["errMsg"].asCString(), "������ܴ������⣬�����ԣ�") == 0)
						{
							pt->AddInfo("ʧ�ܣ��ᵥ���죬���������ܾ��ˣ����ʧ�����޸��ᵥʱ��...");
						}
						else
						{
							pt->AddInfo("ʧ�ܣ�%s", jvalue["errMsg"].asCString());
						}						
					}
					// ��ȡ��Ʊҳ����֤��
					pt->OnBtnRcode(); // ��ȡ��֤��				
					
					// �ȴ�������֤�����ִ��
					tm_space = time(NULL);
					WaitForSingleObject(pt->m_hdWaitCode, INFINITE);
					if (!pt->m_bRunning) // ��Ʊ�����⣬����ˢƱ����Ҫ�˳��߳�
					{
						pt->AddInfo("�˳�ˢƱ");
						return 0;
					}					
					
					cmap.set("randCode", strrandcode);	// ��֤��
					// �淶��֯
					strtmp = str_format("org.apache.struts.taglib.html.TOKEN=%s&leftTicketStr=%s&textfield=%s"
						"&checkbox0=0&orderRequest.train_date=%s&orderRequest.train_no=%s&orderRequest.station_train_code=%s&orderRequest.from_station_telecode=%s"
						"&orderRequest.to_station_telecode=%s&orderRequest.seat_type_code=&orderRequest.ticket_type_order_num=&orderRequest.bed_level_order_num=%s"
						"&orderRequest.start_time=%s&orderRequest.end_time=%s&orderRequest.from_station_name=%s&orderRequest.to_station_name=%s&orderRequest.cancel_flag=%s"
						"&orderRequest.id_mode=%s&passengerTickets=%s&oldPassengers=%s&passenger_1_seat=%s&passenger_1_ticket=%s&passenger_1_name=%s&passenger_1_cardtype=%s"
						"&passenger_1_cardno=%s&passenger_1_mobileno=%s&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y"
						"&oldPassengers=&checkbox9=Y&randCode=%s&orderRequest.reserve_flag=A&tFlag=dc" // 23��
						, cmap["org.apache.struts.taglib.html.TOKEN"].c_str(), cmap["leftTicketStr"].c_str(), "%E4%B8%AD%E6%96%87%E6%88%96%E6%8B%BC%E9%9F%B3%E9%A6%96%E5%AD%97%E6%AF%8D", cmap["orderRequest.train_date"].c_str(), cmap["orderRequest.train_no"].c_str()
						, cmap["orderRequest.station_train_code"].c_str(), cmap["orderRequest.from_station_telecode"].c_str(), cmap["orderRequest.to_station_telecode"].c_str(), cmap["orderRequest.bed_level_order_num"].c_str()
						, cmap["orderRequest.start_time"].c_str(), cmap["orderRequest.end_time"].c_str(), cmap["orderRequest.from_station_name"].c_str(), cmap["orderRequest.to_station_name"].c_str(), cmap["orderRequest.cancel_flag"].c_str()
						, cmap["orderRequest.id_mode"].c_str(), cmap["passengerTickets"].c_str(), cmap["oldPassengers"].c_str(), cmap["passenger_1_seat"].c_str(), cmap["passenger_1_ticket"].c_str(), cmap["passenger_1_name"].c_str()
						, cmap["passenger_1_cardtype"].c_str(), cmap["passenger_1_cardno"].c_str(), cmap["passenger_1_mobileno"].c_str()
						, cmap["randCode"].c_str());

					//pt->AddInfo("�ύ����:[%s]", strtmp.c_str());
					tm_current = time(NULL);
					if (tm_current - tm_space < pt->m_uiTmSubmitTicket) // �����ύ̫�콫���ܾ�
					{
						_sleep((pt->m_uiTmSubmitTicket - (tm_current - tm_space)) * 1000);						
					}
					tm_space = time(NULL);

					// �ύ�˿���Ϣ
					cmap["data"] = strtmp;					
					pt->AddInfo("�ύ����...");
					cmap.cmd(_12306_CONFIRM_PASSENGER);					
					iret = creq->parse(cmap, strack);

				}while ((bret = jread.parse(strack, jvalue)) && jvalue["errMsg"].asString()!= "Y");
				if (bret == false)
				{
					pt->AddInfo("��֤���Ѿ�ʧЧ, ����������");
					continue;
				}
				
// 				if (jread.parse(strack, jvalue))
// 				{			
					/*if (jvalue["errMsg"].asString() != "Y")
					{
						// ��֤�����: �����ų���������, ����Ʊ�Ѿ����꣬ ��Ҫ������֤��
						pt->AddInfo(jvalue["errMsg"].asCString());
						continue;
					}
					else*/ if (jvalue["checkHuimd"].asString() == "N")
					{
						// �Բ���������ȡ���������࣬���ս����ܼ����������Ķ�Ʊ����
						pt->AddInfo("%s ������������Ʊ", jvalue["msg"].asCString());
						pt->alertexit();
					}
					else if (jvalue["check608"].asString() == "N")
					{
						// ����Ϊʵ�����г���ʵ��һ��һ��һ֤һƱ�ƣ�
						pt->AddInfo("%s\r\n������˿���Ϣ", jvalue["msg"].asCString());
						pt->alertexit();
					}
					else
					{
						// �õ���Ʊ��ˮ�ţ��ύ��ˮ�ţ��ȴ�ϵͳ������ϯ
						cmap.cmd(_12306_GET_QUEUE_COUNT);
						cmap["path"] = str_format("&train_date=%s&train_no=%s&station=%s&seat=%s&from=%s&to=%s&ticket=%s"
							, cmap["orderRequest.train_date"].c_str(), cmap["orderRequest.train_no"].c_str()
							, cmap["orderRequest.station_train_code"].c_str(), cmap["passenger_1_seat"].c_str()
							, cmap["orderRequest.from_station_telecode"].c_str()
							, cmap["orderRequest.to_station_telecode"].c_str(), cmap["leftTicketStr"].c_str());
						iret = creq->parse(cmap, strack);
						// {"countT":0,"count":1,"ticket":"1*****32741*****00333*****0010","op_1":false,"op_2":false}
						// cout << strack << endl;
						if (jread.parse(strack, jvalue))
						{
							// ϵͳ���ܳ�Ʊʧ�ܣ���������ȥ��Ʊ��ֱ���û���������4λ��֤�������ˢƱ
							string strticketinfo = "�𾴵��ÿͣ������г���ѡ���ϯ��������Ʊ";
							strticketinfo += gettickinfo(jvalue["ticket"].asCString(), cmap["passenger_1_seat"]);
							strticketinfo += ",";
							if (jvalue["op_2"].asBool())
							{
								strticketinfo += "Ŀǰ�Ŷ������Ѿ�������Ʊ����������ѡ������ϯ��򳵴Σ��ش����ѡ�";
								pt->AddInfo(strticketinfo.c_str());
								continue;
							}
							else
							{
								if (jvalue["countT"].asInt() > 0)
								{
									strticketinfo += "Ŀǰ�ų�����";
									strticketinfo += jvalue["countT"].asCString();
									strticketinfo += "�ˣ�";
								}
								strticketinfo += "�ش����ѡ�";
								pt->AddInfo(strticketinfo.c_str());
							}
							//cout << "��ȷ�϶�����Ϣ�Ƿ���ȷ������ȷ������ȷ������ϵͳ��Ϊ���������ϯλ��" << endl;

							// ȷ�϶���
							cmap.cmd(_12306_CONFRIM_QUEUE);
							// ������֯
							strtmp = str_format("org.apache.struts.taglib.html.TOKEN=%s&leftTicketStr=%s&textfield=%s"
								"&checkbox0=0&orderRequest.train_date=%s&orderRequest.train_no=%s&orderRequest.station_train_code=%s&orderRequest.from_station_telecode=%s"
								"&orderRequest.to_station_telecode=%s&orderRequest.seat_type_code=&orderRequest.ticket_type_order_num=&orderRequest.bed_level_order_num=%s"
								"&orderRequest.start_time=%s&orderRequest.end_time=%s&orderRequest.from_station_name=%s&orderRequest.to_station_name=%s&orderRequest.cancel_flag=%s"
								"&orderRequest.id_mode=%s&passengerTickets=%s&oldPassengers=%s&passenger_1_seat=%s&passenger_1_ticket=%s&passenger_1_name=%s&passenger_1_cardtype=%s"
								"&passenger_1_cardno=%s&passenger_1_mobileno=%s&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y"
								"&oldPassengers=&checkbox9=Y&randCode=%s&orderRequest.reserve_flag=A" // 23��
								, cmap["org.apache.struts.taglib.html.TOKEN"].c_str(), cmap["leftTicketStr"].c_str(), "%E4%B8%AD%E6%96%87%E6%88%96%E6%8B%BC%E9%9F%B3%E9%A6%96%E5%AD%97%E6%AF%8D", cmap["orderRequest.train_date"].c_str(), cmap["orderRequest.train_no"].c_str()
								, cmap["orderRequest.station_train_code"].c_str(), cmap["orderRequest.from_station_telecode"].c_str(), cmap["orderRequest.to_station_telecode"].c_str(), cmap["orderRequest.bed_level_order_num"].c_str()
								, cmap["orderRequest.start_time"].c_str(), cmap["orderRequest.end_time"].c_str(), cmap["orderRequest.from_station_name"].c_str(), cmap["orderRequest.to_station_name"].c_str(), cmap["orderRequest.cancel_flag"].c_str()
								, cmap["orderRequest.id_mode"].c_str(), cmap["passengerTickets"].c_str(), cmap["oldPassengers"].c_str(), cmap["passenger_1_seat"].c_str(), cmap["passenger_1_ticket"].c_str(), cmap["passenger_1_name"].c_str()
								, cmap["passenger_1_cardtype"].c_str(), cmap["passenger_1_cardno"].c_str(), cmap["passenger_1_mobileno"].c_str()
								, cmap["randCode"].c_str());

							//pt->AddInfo("ȷ�϶���:[%s]", strtmp.c_str());
							tm_current = time(NULL);
							if (tm_current - tm_space < pt->m_uiTmSubmitTicket) // �����ύ̫�콫���ܾ�
							{
								_sleep((pt->m_uiTmSubmitTicket - (tm_current - tm_space)) * 1000);								
							} 
							tm_space = time(NULL);;

							cmap["data"] = strtmp;
							pt->AddInfo("ȷ�϶���...");
							iret = creq->parse(cmap, strack);
							if (jread.parse(strack, jvalue) && jvalue["errMsg"].asString() != "Y")
							{
								continue;
							}


							pt->AddInfo("�ȴ�ϵͳ����ϯλ...");
							try
							{
								// �ȴ�ϵͳ���䡾ϯλ��
								icount = 0;
								cmap.cmd(_12306_queryOrderWaitTime);
								iret = creq->parse(cmap, strack); // ��ѯϯλ����	
								//pt->AddInfo("��ѯϯλ���䣺%s", strack.c_str()); //{"tourFlag":"dc","waitTime":-1,"waitCount":0,"orderId":"E722753062","requestId":5798676501947326674,"count":0}
								if (jread.parse(strack, jvalue))
								{
									if (!jvalue["requestId"].isNull())
									{
										pt->AddInfo("ϵͳ����ϯλ�ɹ������ڲ�ѯ����...");
										pt->AddInfo("������ˮ�ţ�%lld", jvalue["requestId"].asDouble());
										pt->AddInfo("��ϲ����Ʊ�ɹ�������45�����ڵ�½ https://dynamic.12306.cn/otsweb/ ����");
									}
									else
									{
										pt->AddInfo("��ѯϯλʧ�ܣ�%s", strack.c_str());
									}
								}
								else
								{
									throw -1;
								}
								
								/*/ ��ѯ��������
								iret = creq->parse(cmap, strack); // ��ѯ����
								pt->AddInfo("���ƶ�Ʊ�ɹ�����ѯ����: %s", strack.c_str()); // {"tourFlag":"dc","waitTime":4,"waitCount":0,"requestId":5798679846929932498,"count":-1}							
								if (jread.parse(strack, jvalue))
								{
									if (jvalue["waitTime"].asInt() > 0 && !jvalue["requestId"].isNull())
									{
										pt->AddInfo("��ϲ����Ʊ�ɹ�����ˮ�ţ�%s�����½֧����", jvalue["requestId"].asCString());
										pt->alertexit();
									}
																	
								}
								else 
								{
									throw -2;
								}	//*/
							}
							catch(...)
							{
								pt->AddInfo("δ֪�쳣:[%s]", strack.c_str());
							}
							pt->alertexit();
						}
					}
// 				}
// 				else
// 				{
// 					pt->AddInfo("�쳣����(�޷�����������): ��֤���Ѿ�ʧЧ");
// 				}
			} while (1);
		}
		else
		{
			pt->AddInfo("δ��ѯ�����賵Ʊ��5s�������ѯ");
		}
	}
	else
	{
		pt->AddInfo("δ��ѯ���κγ��Σ���ȷ������Ϣ�Ƿ���ȷ��5s�������ѯ");
	}	
}
	pt->AddInfo("�˳���ѯ");
	return 0;
}

void CGetTicketDlg::OnBtnRcode() 
{
	// TODO: Add your control notification handler code here
	if (!m_bRunning)
	{
		return;
	}
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(FALSE); // �޷�����
	m_cbtn.EnableWindow(FALSE); // �޷����	
	SetDlgItemText(IDC_EDIT_RCODE, "������");
	bool bget = false;
	do
	{
		if (bget = creq->GetVertifyImg(pszpasgeimg, strRCodePath))
		{
			AddInfo("��ȡ��֤��ɹ�����������֤�룡");			
			break;
		}					
	}while(!bget);	
	m_cbtn.Reflush();
	m_cbtn.EnableWindow(TRUE);	
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(TRUE);
	SetDlgItemText(IDC_EDIT_RCODE, "");	
	ShowWindow(SW_SHOWNORMAL);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetTimer(1, 500, NULL);
}

void CGetTicketDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(1);
	GetDlgItem(IDC_EDIT_RCODE)->SetFocus();
	CDialog::OnTimer(nIDEvent);
}

void CGetTicketDlg::OnBtnGetticket() 
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_RCODE, m_strRCode);
	if (m_strRCode.IsEmpty() || m_strRCode.GetLength() < 4)
	{
		AfxMessageBox("��������֤��");
		return;
	}
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	strrandcode = m_strRCode;
	SetEvent(m_hdWaitCode);
}

void CGetTicketDlg::OnChangeEditRcode() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_RCODE, m_strRCode);
	if (m_strRCode.GetLength() == 4)
	{
		OnBtnGetticket();
	}
}

BOOL CGetTicketDlg::PreTranslateMessage(MSG* pMsg) 
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
			OnBtnRunning();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
