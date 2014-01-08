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
//                                程序变量
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

// Param  常量
const unsigned int uigetday = 20; // 20天内订票
const unsigned int uiTicketSpace = 3; // 提交订单和确认订单时间间隔：提交过快将返回网络可能存在问题，请重试！
const unsigned int uireflushms = 5000; // 查询间隔
const string strfixuserinfo = "夏良才"; // 指定用户可以使用该工具
map<string, string> mapstrcity; // 城市信息 城市：代号

string strRCodePath = "\\randCode.jpg"; // 验证码
string strsignfile = "\\pem_12306.pem";	// 证书
string strerrorfile = "\\error.txt";
string strinifile = "\\mytrain.ini";
string stralterapp = "RightDownNotify.exe";
const char* pszloginimg = "/otn/passcodeNew/getPassCodeNew?module=login&rand=sjrand&0."; // 登陆验证码
const char* pszpasgeimg = "/otn/passcodeNew/getPassCodeNew?module=passenger&rand=randp&0."; // 订票验证码
const char* pszautoimgpath = "/otn/passcodeNew/getPassCodeNew.do?module=login&rand=sjrand&0."; // 自动提交订单
const char* pszseattext[] = {"商务座", "特等座", "一等座", "二等座", "高级软卧","软卧", "硬卧", "软座", "硬座", "无座", "其他"}; // 座席
const char* pszseattype[] = {"9", "P", "M", "O", "", "4", "3", "2", "1", "1", ""}; // 座席代号
const char* pszpassenger[] = {"成人票", "儿童票", "学生票", "残军票"}; // 1|2|3|4
const char* pszpassenger_cardtext[] = {"二代身份证", "一代身份证", "港澳通行证", "台湾通行证", "护照"}; //1|2|C|G|B
const char* pszpassenger_cardno[] = {"1", "2", "C", "G", "B"}; // 证件代号
const char* pszorderparam[] = {"station_train_code", "lishi", "train_start_time", "trainno4", "from_station_telecode"
, "to_station_telecode", "arrive_time", "from_station_telecode_name", "to_station_telecode_name", "from_station_no"
, "to_station_no", "ypInfoDetail", "mmStr", "locationCode"}; // 转到预定页面提交字段
const char* pszsubmitconfirm[] = {"org.apache.struts.taglib.html.TOKEN", "leftTicketStr", "orderRequest.train_date"
, "orderRequest.train_no", "orderRequest.station_train_code", "orderRequest.from_station_telecode"
, "orderRequest.to_station_telecode", "orderRequest.seat_type_code", "orderRequest.ticket_type_order_num"
, "orderRequest.bed_level_order_num", "orderRequest.start_time", "orderRequest.end_time", "orderRequest.from_station_name"
		, "orderRequest.to_station_name", "orderRequest.cancel_flag", "orderRequest.id_mode"}; // 订单提交页面各字段

/************************************************************************/


/************************************************************************/
//                               订票人购票信息
// 查询条件: 单程
string startcity = "";
string startcitycode =""; // 出发城市 station_name.txt
string endcity = "";
string endcitycode = ""; // 目的城市	
string trainCodeText = ""; // 车次
string train_no = ""; // 车次代号
string trainPassType = "QB"; // 车路 : 全部|始发|过路
string trainClass = "QB#D#Z#T#K#QT#"; // 车型：全部|动车|Z字头|T字头|K字头|其他
string includeStudent = "ADULT"; // 包含学生票
string seatTypeAndNum = ""; // 
string startdate = ""; // 出发日期
string startTime = "00:00--24:00";	// 出发时间
// end 查询条件

// 乘客信息: 已经加载乘客信息直接获取
string strseattext = "硬座";
ushort usseatidx = 10; // 座席位置
ushort usselseat = 0;
string strpassenger_cardno = "";
// end 乘客信息
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
		strret += i2str(seat_1) + "张";
	}
	if (seat_2 > 0) {
		strret += ",无座";
		strret += i2str(seat_2) + "张";
	}
	return strret;
}



/////////////////////////////////////////////////////////////////////////////
// CGetTicketDlg dialog

#define MAX_TICKET_LENGTHS 32

void CGetTicketDlg::alertexit()
{
	creq->https_close_connect();
	AfxMessageBox("程序运行结束，点击确认退出。。。");	
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
	
	// 初始化
	m_cbtn.SubclassWindow(GetDlgItem(IDC_BTN_RCODE)->m_hWnd);
	InitParam();
	InitManList();
	InitTicketList();
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(FALSE);

	m_hdWaitCode = CreateEvent(NULL, FALSE, FALSE, _T("RandCode"));

	// 获取乘客
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
	
	// 写日志
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
	CMemory_Conf::instance()->get_conf_str("配置", "出发地", "", sztmp, MAX_BUFFER_BLOCK);
	m_strStartCity = sztmp;
	CMemory_Conf::instance()->get_conf_str("配置", "目的地", "", sztmp, MAX_BUFFER_BLOCK);
	m_strEndCity = sztmp;

	
	for (int i = 0; i < uigetday; ++i)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_STARTDATE))->AddString(date("Y-m-d", i).c_str());
	}
	CMemory_Conf::instance()->get_conf_str("配置", "出发日期", date("Y-m-d").c_str(), sztmp, MAX_BUFFER_BLOCK);
	m_strStartDate = sztmp;
	if (m_strStartDate.IsEmpty() || m_strStartDate.CompareNoCase(date("Y-m-d").c_str()) == -1)
	{
		m_strStartDate = date("Y-m-d").c_str();
	}

	CMemory_Conf::instance()->get_conf_str("配置", "出发时间", "00:00--24:00", sztmp, MAX_BUFFER_BLOCK);
	m_strStartTime = sztmp;
	if (m_strStartTime.IsEmpty())
	{
		m_strStartTime = "00:00--24:00";
	}
	

	CMemory_Conf::instance()->get_conf_str("配置", "车次", "", sztmp, MAX_BUFFER_BLOCK);
	m_strTrainNo = sztmp;

	CMemory_Conf::instance()->get_conf_str("配置", "姓名", "", sztmp, MAX_BUFFER_BLOCK);
	m_strName = sztmp;
	CMemory_Conf::instance()->get_conf_str("配置", "身份证", "", sztmp, MAX_BUFFER_BLOCK);
	m_strVerifyCode = sztmp;
	m_nTicketType = CMemory_Conf::instance()->get_conf_int("配置", "票种", 1);

	m_nSeatType = CMemory_Conf::instance()->get_conf_int("配置", "座席", 1);	
	if (m_nSeatType > 0)
	{
		usseatidx = m_nSeatType - 1;
		usselseat = m_nSeatType - 1;
	}

	CMemory_Conf::instance()->get_conf_str("配置", "手机号", "", sztmp, MAX_BUFFER_BLOCK);
	m_strMobile = sztmp;

	m_uiTmGetTicket = CMemory_Conf::instance()->get_conf_int("配置", "刷票", 5);
	m_uiTmSubmitTicket = CMemory_Conf::instance()->get_conf_int("配置", "提单", 3);

	replace(trainClass, "#", "%23");

	UpdateData(FALSE);
}

void CGetTicketDlg::InitManList()
{
	LONG lStyle; 
	lStyle = GetWindowLong(m_listMan.m_hWnd, GWL_STYLE);//获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位 
	lStyle |= LVS_REPORT; //设置style 	
	SetWindowLong(m_listMan.m_hWnd, GWL_STYLE, lStyle);//设置style 
	DWORD dwStyle = m_listMan.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件 
	m_listMan.SetExtendedStyle(dwStyle); //设置扩展风格 
	
	//m_listMan.InsertColumn(0, _T("Y/N"), LVCFMT_CENTER, 30, 0);
	m_listMan.InsertColumn(0, _T("姓名"), LVCFMT_LEFT, 50, 0);
	m_listMan.InsertColumn(1, _T("二代身份证"), LVCFMT_LEFT, 126, 0);

	/*/ test data
	m_listMan.InsertItem(0, "张三");
	m_listMan.SetItemText(0, 1, "433423199303131413");

	m_listMan.InsertItem(1, "李四");
	m_listMan.SetItemText(1, 1, "433423199303131413"); //*/
}

void CGetTicketDlg::InitTicketList()
{
	LONG lStyle; 
	lStyle = GetWindowLong(m_listTicket.m_hWnd, GWL_STYLE);//获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位 
	lStyle |= LVS_REPORT; //设置style 
	lStyle |= LVS_NOSCROLL; //隐藏滚动条
	SetWindowLong(m_listTicket.m_hWnd, GWL_STYLE, lStyle);//设置style 
	DWORD dwStyle = m_listTicket.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl） 
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件 
	m_listTicket.SetExtendedStyle(dwStyle); //设置扩展风格 
	//m_listTicket.ShowScrollBar(SB_HORZ, FALSE); // 隐藏水平滚动条
	
	int i = 0;
	m_listTicket.InsertColumn(i++, _T("车次"), LVCFMT_LEFT, 40, 0);
	m_listTicket.InsertColumn(i++, _T("(始)发站(时间)"), LVCFMT_CENTER, 104, 0);
	m_listTicket.InsertColumn(i++, _T("(始)到站(时间)"), LVCFMT_CENTER, 104, 0);
	m_listTicket.InsertColumn(i++, _T("历时"), LVCFMT_CENTER, 42, 0);
	m_listTicket.InsertColumn(i++, _T("商务座"), LVCFMT_CENTER, 50, 0);
	m_listTicket.InsertColumn(i++, _T("特等座"), LVCFMT_CENTER, 50, 0);
	m_listTicket.InsertColumn(i++, _T("一等座"), LVCFMT_CENTER, 50, 0);
	m_listTicket.InsertColumn(i++, _T("二等座"), LVCFMT_CENTER, 50, 0);
	m_listTicket.InsertColumn(i++, _T("高级软卧"), LVCFMT_CENTER, 60, 0);
	m_listTicket.InsertColumn(i++, _T("软卧"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("硬卧"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("软座"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("硬座"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("无座"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("其他"), LVCFMT_CENTER, 36, 0);
	m_listTicket.InsertColumn(i++, _T("购票"), LVCFMT_CENTER, 36, 0);
	
	/*/ test data
	int j = 1;
	m_listTicket.InsertItem(0, _T("K9122"));
	m_listTicket.SetItemText(0, j++, "(始) 深圳 21:43");
	m_listTicket.SetItemText(0, j++, "(终) 衡阳 06:21");
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
	m_listTicket.SetItemText(0, j++, "有");
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
		SetDlgItemText(IDC_BTN_RUNNING, "开始");
		m_bRunning = FALSE;		
		SetEvent(m_hdWaitCode); //退出原有线程
		return;
	}	

	if (!UpdateData())
	{
		return;
	}
	
	// 起始地 目的地
	if (m_strStartCity.IsEmpty() || m_strEndCity.IsEmpty())
	{
		AfxMessageBox("起始地或目的地不能为空");
		return;
	}
	startcity = m_strStartCity;
	endcity = m_strEndCity;
	startcitycode = mapstrcity[startcity];
	endcitycode = mapstrcity[endcity];
	if (startcitycode.empty() || endcitycode.empty())
	{
		AfxMessageBox("起始城市或者目的城市输入有误");
		return;
	}

	// 出发日期
	if (m_strStartDate.IsEmpty())
	{
		AfxMessageBox("出发日期不能为空");
		return;
	}
	startdate = m_strStartDate;

	// 出发时间
	if (m_strStartTime.IsEmpty())
	{
		AfxMessageBox("出发时间不能为空");
		return;
	}
	startTime = m_strStartTime;	
	replace(startTime, ":", "%3A");

	// 车次
	trainCodeText = m_strTrainNo;

	// 姓名 身份证号码
	if (m_strName.IsEmpty() || m_strVerifyCode.IsEmpty())
	{
		AfxMessageBox("购票姓名或者身份证号码为空");
		return;
	}

	// 票种
	if (m_nTicketType == 0)
	{
		AfxMessageBox("请选择票种");
		return;
	}

	// 座席
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

	// 保存所有信息
	CMemory_Conf::instance()->write_conf_str("配置", "出发地", m_strStartCity);
	CMemory_Conf::instance()->write_conf_str("配置", "目的地", m_strEndCity);
	CMemory_Conf::instance()->write_conf_str("配置", "出发日期", m_strStartDate);
	CMemory_Conf::instance()->write_conf_str("配置", "出发时间", m_strStartTime);
	CMemory_Conf::instance()->write_conf_str("配置", "车次", m_strTrainNo);
	CMemory_Conf::instance()->write_conf_str("配置", "姓名", m_strName);
	CMemory_Conf::instance()->write_conf_str("配置", "身份证", m_strVerifyCode);
	CMemory_Conf::instance()->write_conf_str("配置", "手机号", m_strMobile);
	CMemory_Conf::instance()->write_conf_int("配置", "票种", m_nTicketType);
	CMemory_Conf::instance()->write_conf_int("配置", "座席", m_nSeatType);
	CMemory_Conf::instance()->write_conf_int("配置", "刷票", m_uiTmGetTicket);
	CMemory_Conf::instance()->write_conf_int("配置", "提单", m_uiTmSubmitTicket);

	char szfileconn[4096 + 1] = {0};
	int iconn = 4096;
	CEncrypt_Data ce;
	CMemory_Conf::instance()->get_str(szfileconn, iconn);
	ce.encode(szfileconn, strlen(szfileconn), szfileconn, &iconn);
	ce.save(strinifile.c_str(), szfileconn, iconn);

	//CMemory_Conf::instance()->save_conf();
	
	m_bRunning = TRUE;	
	SetDlgItemText(IDC_BTN_RUNNING, "停止");
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
	
	map<string, TRAIN_PASSENGER_INFO> mappassengers; // 证件号码=>个人信息
	TRAIN_PASSENGER_INFO stper_passenger; // 个人信息
	TRAIN_PASSENGER_LIST trainpassengers;
	
	memset(&trainpassengers, 0, sizeof(TRAIN_PASSENGER_LIST));
	pT->m_listMan.DeleteAllItems();
	//cmap.cmd(_12306_GETTRAIN_PASSENGER);
	iret = creq->http_post("/otn/passengers/query", "pageIndex=1&pageSize=50", strack);
	// 提取乘客数据
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
				pT->AddInfo("正在获取联系人....");
				icount = jlist.size();
				for (int i = 0; i < icount && i < MAX_TRAIN_PASSENGER; ++i)
				{
					
					jlistitem = jlist[i];
					++trainpassengers.uscount;
					strncpy(trainpassengers.stpassenger[i].szname, jlistitem["passenger_name"].asCString(), MAX_PASSENGER_NAME);
					trainpassengers.stpassenger[i].ustype = jlistitem["passenger_type"].toInt();
					strncpy(trainpassengers.stpassenger[i].szmobile, jlistitem["mobile_no"].asCString(), MAX_MOBILE_LEN);
					strncpy(trainpassengers.stpassenger[i].szidtype, jlistitem["passenger_id_type_code"].asCString(), MAX_TRAIN_NO); // 证件类型
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
	pT->AddInfo("成功获取 %d 位联系人", mappassengers.size());	
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
	string strtoken;
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
	// 刷票
	// 查询车次代号 train_no->id暂不实现，直接通过查询出所有车次再筛选
	//https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=queryststrainall
	//date=2013-10-03&fromstation=SZQ&tostation=HYQ&starttime=00%3A00--24%3A00
	//[{"end_station_name":"武汉","end_time":"11:52","id":"6i000G100201","start_station_name":"深圳北","start_time":"07:00","value":"G1002"},{}]

	// 打印查询条件
	++pt->m_nGetTicketTimes;
	pt->AddInfo("确认信息：\r\n出发地: %s 目的地: %s 出发日期：%s 出发时间：%s\r\n"
		"出发车次: %s\r\n"
		"乘客姓名：%-8s 身份证：%s\r\n"
		"座席：%s\r\n"
		"开始进行第%d次查询"
		, startcity.c_str(), endcity.c_str(), startdate.c_str(), pt->m_strStartTime
		, (trainCodeText.empty() ? "有车" : trainCodeText.c_str())
		, pt->m_strName, pt->m_strVerifyCode
		, (strseattext.empty() ? "有座" : strseattext.c_str())
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
		pt->AddInfo("未查询到任何信息，请检查查询条件是否正确");
		continue;
	}

	// parse json data
	if (!jread.parse(strack, jvalue))
	{
		pt->AddInfo("查询数据异常");
		continue;
	}

	// 后续处理异常
	/*if (strack == "-10")
	{
		// 重新登陆
		AfxMessageBox("您还没有登录或者离开页面的时间过长，请登录系统或者刷新页面");
		pt->alertexit();
	}
	if (strack == "-1")
	{
		AfxMessageBox("服务器忙，加载查询数据失败！");	
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

	// 正常流程
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
			
			strncpy(stqtrains.stlist[i].sztrain_nos, jlistitem["train_no"].asCString(), MAX_TRAIN_NO); //

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

		pt->AddInfo("共查询到%d车次", stqtrains.uscount);
	}
	// 显示滚动条
	if (stqtrains.uscount > MAX_TICKET_LENGTHS)
	{
		pt->m_listTicket.ShowScrollBar(SB_VERT, TRUE);
	}	
		
	if (1)
	{
		continue;
	}
	
	if (pt->m_bRunning == FALSE)
	{
		break;
	}	
	

	// 有车次数据
	if (stqtrains.uscount > 0)
	{
		// 筛选指定车次：优先可订票、其次车次(完全匹配转大写)
		for (i = 0; i < stqtrains.uscount; ++i)
		{
			if (!stqtrains.stlist[i].bsubmit)
			{
				continue;
			}
			if (trainCodeText.empty() || stricmp(stqtrains.stlist[i].sztrain_no, trainCodeText.c_str()) == 0)
			{
				// 匹配座位 默认硬座 usseatidx = 8 // 选定座席有票或者剩余多少张
				if (usseatidx == 10) // 有票就订
				{
					for (j = 0; j < TICKET_SIGN_LEN; ++j)
					{
						if (strcmp(stqtrains.stlist[i].szTicket[j], "有") == 0  || atoi(stqtrains.stlist[i].szTicket[j]) > 0)
						{
							usselseat = j;
							break;
						}
					}
				}

				if (strcmp(stqtrains.stlist[i].szTicket[usselseat], "有") == 0  || atoi(stqtrains.stlist[i].szTicket[usselseat]) > 0)
				{
					// 匹配城市
					//if (strcmp(stqtrains.stlist[i].szstartcity, startcity.c_str()) == 0 && strcmp(stqtrains.stlist[i].szendcity, endcity.c_str()) == 0)
					{
						pt->AddInfo("【%s】【%s=>%s】【%s - %s】【%s】【%s】%s, 开始预定..\n"
							, stqtrains.stlist[i].sztrain_no
							, stqtrains.stlist[i].szstartcity, stqtrains.stlist[i].szendcity
							, stqtrains.stlist[i].szstarttime, stqtrains.stlist[i].szendtime
							, stqtrains.stlist[i].szusetime
							, pszseattext[usselseat]
							, atoi(stqtrains.stlist[i].szTicket[usselseat]) > 0 ? (string(stqtrains.stlist[i].szTicket[usselseat]) + "张").c_str() : "充足");
						break;
					}					
				}
			}
		}
		if (i != stqtrains.uscount)
		{
			// 自动提交
			// 输入验证码			
			string strpassenger;
			string stroldpassenger;
			time_t tm_current = time(NULL);
			time_t tm_space = tm_current;

			strpassenger = str_format("%s,0,%d,%s,%s,%s,%s,Y"
				, pszseattype[usselseat], pt->m_nTicketType, pt->m_strName, "1"/*证件类型：身份证*/, pt->m_strVerifyCode, pt->m_strMobile);
			stroldpassenger = str_format("%s,%s,%s"
				,  pt->m_strName, "1"/*证件类型：身份证*/, pt->m_strVerifyCode);

			strtmp = str_format("secretStr=%s&train_date=%s&tour_flag=dc&purpose_codes=%s"
					"&query_from_station_name=%s&query_to_station_name=%s&&cancel_flag=2"
					"&bed_level_order_num=000000000000000000000000000000"
					"&passengerTicketStr=%s"
					"&oldPassengerStr=%s,1_"
					, stqtrains.stlist[i].szsubmitcode, startdate.c_str(), includeStudent.c_str()
					, startcity.c_str(), endcity.c_str()
					, CHandleCode::GBKToUTF8(strpassenger).c_str()
					, CHandleCode::GBKToUTF8(stroldpassenger).c_str());
			strack = "";
			while (strack.empty())
			{			
				creq->http_post("/otn/confirmPassenger/autoSubmitOrderRequest", strtmp, strack);
			}
			if (!jread.parse(strack, jvalue))
			{
				pt->AddInfo("自动订票请求失败");
				continue;
			}

			if (!jvalue["data"].isNull() && !jvalue["data"]["result"])
			{
				strtoken = jvalue["data"]["result"].asString();
			}
			if (strtoken.empty())
			{
				pt->AddInfo("自动订票请求返回数据异常");
				continue;
			}

			string strtriandate;
			vector<string> VecTok;
			str_explode(strtoken, "#", VecTok);
				
			// train_date=Wed+Jan+8+17%3A33%3A21+UTC%2B0800+2014&
			strtmp = str_format("train_no=%s&stationTrainCode=%s&seatType=%s"
				"&fromStationTelecode=%s&toStationTelecode=%s&leftTicket=%s&purpose_codes=%s&_json_att="
				, stqtrains.stlist[i].sztrain_nos, stqtrains.stlist[i].sztrain_no, pszseattype[usselseat]
				, startcitycode.c_str(), endcitycode.c_str(), VecTok[2].c_str(), includeStudent);
			strack = "";
			while (strack.empty())
			{
				creq->http_post("/otn/confirmPassenger/getQueueCountAsync", strtmp, strack);
			}
			// {"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"count":"25","ticket":"O007450376M0099500619019950015","op_2":"false","countT":"0","op_1":"true"},"messages":[],"validateMessages":{}}
			
			tm_space = time(NULL);
			icount = 0;
			do
			{	
				++icount > 1 ? pt->AddInfo("验证码错误，请重新输入") : "";
				pt->OnBtnRcode(); // 获取验证码
				WaitForSingleObject(pt->m_hdWaitCode, INFINITE);
				if (!pt->m_bRunning) // 此票不满意，重新刷票，需要退出线程
				{
					pt->AddInfo("退出刷票");
					return 0;
				}
				
				// 效验验证码
				strack = "";
				while (strack.empty())
				{
					strtmp = str_format("randCode=%s&rand=sjrand&_json_att=&REPEAT_SUBMIT_TOKEN=%s"
						, pt->m_strRCode, strtoken.c_str());
					creq->http_post("/otn/passcodeNew/checkRandCodeAnsyn", strtmp, strack);
					_sleep(100);
				}				
			}while (strack.find("\"data\":\"Y\"") == string::npos);

			// /otn/confirmPassenger/confirmSingleForQueueAsys
			strtmp = str_format("passengerTicketStr=%s"
				, CHandleCode::UrlEncode(CHandleCode::GBKToUTF8(stroldpassenger)));
			strack = "";
			while (strack.empty())
			{
				creq->http_post("/otn/confirmPassenger/confirmSingleForQueueAsys", strtmp, strack);
			}



			// -- begin 手动提交 --
			/*
			strtmp = str_format("secretStr=%s&train_date=%s&back_train_date=%s&tour_flag=dc&purpose_codes=%s&query_from_station_name=%s&query_to_station_name=%s&undefined"
				, stqtrains.stlist[i].szsubmitcode, startdate.c_str(), startdate.c_str(), includeStudent.c_str(), stqtrains.stlist[i].szstartcity, stqtrains.stlist[i].szendcity);
			creq->http_post("/otn/leftTicket/submitOrderRequest", strtmp, strack);
			if (strack.empty() || (jread.parse(strack, jvalue) && !jvalue["status"].asBool()))
			{
				pt->AddInfo("点击预定失败");
				continue;
			}

			creq->http_post("/otn/confirmPassenger/initDc", "_json_att=", strack);
			if (strack.empty() || (strtoken = getmidstr(strack, "globalRepeatSubmitToken = '", "'")).empty())
			{
				pt->AddInfo("获取预定页面异常");
				continue;
			}

			// 输入验证码
			time_t tm_current = time(NULL);
			time_t tm_space = tm_current;

			tm_space = time(NULL);
			icount = 0;
			do
			{	
				++icount > 1 ? pt->AddInfo("验证码错误，请重新输入") : "";
				pt->OnBtnRcode(); // 获取验证码
				WaitForSingleObject(pt->m_hdWaitCode, INFINITE);
				if (!pt->m_bRunning) // 此票不满意，重新刷票，需要退出线程
				{
					pt->AddInfo("退出刷票");
					return 0;
				}

				// 效验验证码
				strack = "";
				while (strack.empty())
				{
					strtmp = str_format("randCode=%s&rand=randp&_json_att=&REPEAT_SUBMIT_TOKEN=%s"
					, pt->m_strRCode, strtoken.c_str());
					creq->http_post("/otn/passcodeNew/checkRandCodeAnsyn", strtmp, strack);
					_sleep(100);
				}				
			}while (strack.find("\"data\":\"Y\"") == string::npos)
			// continue submit second times after success.
			
			//train_date=Wed+Jan+8+00:00:00+UTC+0800+2014
			strtmp = str_format("train_date=%s&train_no=%s&stationTrainCode=%s&seatType=%d"
			"&fromStationTelecode=%s&toStationTelecode=%s&leftTicket=O007450413M0099500599019950014&purpose_codes=00&_json_att=&REPEAT_SUBMIT_TOKEN=%s")
			*/			
			

			// 预定页面开始订票程序			
			do 
			{
				do
				{
					if (!jvalue["errMsg"].isNull())
					{
						//网络可能存在问题，请重试！
						if (strcmp(jvalue["errMsg"].asCString(), "网络可能存在问题，请重试！") == 0)
						{
							pt->AddInfo("失败：提单过快，被铁道部拒绝了！多次失败请修改提单时间...");
						}
						else
						{
							pt->AddInfo("失败：%s", jvalue["errMsg"].asCString());
						}						
					}
					// 获取订票页面验证码
					pt->OnBtnRcode(); // 获取验证码				
					
					// 等待输入验证码继续执行
					tm_space = time(NULL);
					WaitForSingleObject(pt->m_hdWaitCode, INFINITE);
					if (!pt->m_bRunning) // 此票不满意，重新刷票，需要退出线程
					{
						pt->AddInfo("退出刷票");
						return 0;
					}					
					
					cmap.set("randCode", strrandcode);	// 验证码
					// 规范组织
					strtmp = str_format("org.apache.struts.taglib.html.TOKEN=%s&leftTicketStr=%s&textfield=%s"
						"&checkbox0=0&orderRequest.train_date=%s&orderRequest.train_no=%s&orderRequest.station_train_code=%s&orderRequest.from_station_telecode=%s"
						"&orderRequest.to_station_telecode=%s&orderRequest.seat_type_code=&orderRequest.ticket_type_order_num=&orderRequest.bed_level_order_num=%s"
						"&orderRequest.start_time=%s&orderRequest.end_time=%s&orderRequest.from_station_name=%s&orderRequest.to_station_name=%s&orderRequest.cancel_flag=%s"
						"&orderRequest.id_mode=%s&passengerTickets=%s&oldPassengers=%s&passenger_1_seat=%s&passenger_1_ticket=%s&passenger_1_name=%s&passenger_1_cardtype=%s"
						"&passenger_1_cardno=%s&passenger_1_mobileno=%s&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y"
						"&oldPassengers=&checkbox9=Y&randCode=%s&orderRequest.reserve_flag=A&tFlag=dc" // 23个
						, cmap["org.apache.struts.taglib.html.TOKEN"].c_str(), cmap["leftTicketStr"].c_str(), "%E4%B8%AD%E6%96%87%E6%88%96%E6%8B%BC%E9%9F%B3%E9%A6%96%E5%AD%97%E6%AF%8D", cmap["orderRequest.train_date"].c_str(), cmap["orderRequest.train_no"].c_str()
						, cmap["orderRequest.station_train_code"].c_str(), cmap["orderRequest.from_station_telecode"].c_str(), cmap["orderRequest.to_station_telecode"].c_str(), cmap["orderRequest.bed_level_order_num"].c_str()
						, cmap["orderRequest.start_time"].c_str(), cmap["orderRequest.end_time"].c_str(), cmap["orderRequest.from_station_name"].c_str(), cmap["orderRequest.to_station_name"].c_str(), cmap["orderRequest.cancel_flag"].c_str()
						, cmap["orderRequest.id_mode"].c_str(), cmap["passengerTickets"].c_str(), cmap["oldPassengers"].c_str(), cmap["passenger_1_seat"].c_str(), cmap["passenger_1_ticket"].c_str(), cmap["passenger_1_name"].c_str()
						, cmap["passenger_1_cardtype"].c_str(), cmap["passenger_1_cardno"].c_str(), cmap["passenger_1_mobileno"].c_str()
						, cmap["randCode"].c_str());

					//pt->AddInfo("提交订单:[%s]", strtmp.c_str());
					tm_current = time(NULL);
					if (tm_current - tm_space < pt->m_uiTmSubmitTicket) // 订单提交太快将被拒绝
					{
						_sleep((pt->m_uiTmSubmitTicket - (tm_current - tm_space)) * 1000);						
					}
					tm_space = time(NULL);

					// 提交乘客信息
					cmap["data"] = strtmp;					
					pt->AddInfo("提交订单...");
					cmap.cmd(_12306_CONFIRM_PASSENGER);					
					iret = creq->parse(cmap, strack);

				}while ((bret = jread.parse(strack, jvalue)) && jvalue["errMsg"].asString()!= "Y");
				if (bret == false)
				{
					pt->AddInfo("验证码已经失效, 请重新输入");
					continue;
				}
				
// 				if (jread.parse(strack, jvalue))
// 				{			
					/*if (jvalue["errMsg"].asString() != "Y")
					{
						// 验证码错误: 或者排除人数过多, 或者票已经售完， 需要更新验证码
						pt->AddInfo(jvalue["errMsg"].asCString());
						continue;
					}
					else*/ if (jvalue["checkHuimd"].asString() == "N")
					{
						// 对不起，由于您取消次数过多，今日将不能继续受理您的订票请求！
						pt->AddInfo("%s 请明天再来订票", jvalue["msg"].asCString());
						pt->alertexit();
					}
					else if (jvalue["check608"].asString() == "N")
					{
						// 本车为实名制列车，实行一日一车一证一票制！
						pt->AddInfo("%s\r\n请更换乘客信息", jvalue["msg"].asCString());
						pt->alertexit();
					}
					else
					{
						// 得到订票流水号，提交流水号，等待系统分配座席
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
							// 系统可能出票失败，继续挤进去抢票，直到用户输入少于4位验证码则继续刷票
							string strticketinfo = "尊敬的旅客，本次列车您选择的席别尚有余票";
							strticketinfo += gettickinfo(jvalue["ticket"].asCString(), cmap["passenger_1_seat"]);
							strticketinfo += ",";
							if (jvalue["op_2"].asBool())
							{
								strticketinfo += "目前排队人数已经超过余票张数，请您选择其他席别或车次，特此提醒。";
								pt->AddInfo(strticketinfo.c_str());
								continue;
							}
							else
							{
								if (jvalue["countT"].asInt() > 0)
								{
									strticketinfo += "目前排除人数";
									strticketinfo += jvalue["countT"].asCString();
									strticketinfo += "人，";
								}
								strticketinfo += "特此提醒。";
								pt->AddInfo(strticketinfo.c_str());
							}
							//cout << "请确认订单信息是否正确，如正确请点击“确定”，系统将为您随机分配席位。" << endl;

							// 确认订单
							cmap.cmd(_12306_CONFRIM_QUEUE);
							// 重新组织
							strtmp = str_format("org.apache.struts.taglib.html.TOKEN=%s&leftTicketStr=%s&textfield=%s"
								"&checkbox0=0&orderRequest.train_date=%s&orderRequest.train_no=%s&orderRequest.station_train_code=%s&orderRequest.from_station_telecode=%s"
								"&orderRequest.to_station_telecode=%s&orderRequest.seat_type_code=&orderRequest.ticket_type_order_num=&orderRequest.bed_level_order_num=%s"
								"&orderRequest.start_time=%s&orderRequest.end_time=%s&orderRequest.from_station_name=%s&orderRequest.to_station_name=%s&orderRequest.cancel_flag=%s"
								"&orderRequest.id_mode=%s&passengerTickets=%s&oldPassengers=%s&passenger_1_seat=%s&passenger_1_ticket=%s&passenger_1_name=%s&passenger_1_cardtype=%s"
								"&passenger_1_cardno=%s&passenger_1_mobileno=%s&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y&oldPassengers=&checkbox9=Y"
								"&oldPassengers=&checkbox9=Y&randCode=%s&orderRequest.reserve_flag=A" // 23个
								, cmap["org.apache.struts.taglib.html.TOKEN"].c_str(), cmap["leftTicketStr"].c_str(), "%E4%B8%AD%E6%96%87%E6%88%96%E6%8B%BC%E9%9F%B3%E9%A6%96%E5%AD%97%E6%AF%8D", cmap["orderRequest.train_date"].c_str(), cmap["orderRequest.train_no"].c_str()
								, cmap["orderRequest.station_train_code"].c_str(), cmap["orderRequest.from_station_telecode"].c_str(), cmap["orderRequest.to_station_telecode"].c_str(), cmap["orderRequest.bed_level_order_num"].c_str()
								, cmap["orderRequest.start_time"].c_str(), cmap["orderRequest.end_time"].c_str(), cmap["orderRequest.from_station_name"].c_str(), cmap["orderRequest.to_station_name"].c_str(), cmap["orderRequest.cancel_flag"].c_str()
								, cmap["orderRequest.id_mode"].c_str(), cmap["passengerTickets"].c_str(), cmap["oldPassengers"].c_str(), cmap["passenger_1_seat"].c_str(), cmap["passenger_1_ticket"].c_str(), cmap["passenger_1_name"].c_str()
								, cmap["passenger_1_cardtype"].c_str(), cmap["passenger_1_cardno"].c_str(), cmap["passenger_1_mobileno"].c_str()
								, cmap["randCode"].c_str());

							//pt->AddInfo("确认订单:[%s]", strtmp.c_str());
							tm_current = time(NULL);
							if (tm_current - tm_space < pt->m_uiTmSubmitTicket) // 订单提交太快将被拒绝
							{
								_sleep((pt->m_uiTmSubmitTicket - (tm_current - tm_space)) * 1000);								
							} 
							tm_space = time(NULL);;

							cmap["data"] = strtmp;
							pt->AddInfo("确认订单...");
							iret = creq->parse(cmap, strack);
							if (jread.parse(strack, jvalue) && jvalue["errMsg"].asString() != "Y")
							{
								continue;
							}


							pt->AddInfo("等待系统分配席位...");
							try
							{
								// 等待系统分配【席位】
								icount = 0;
								cmap.cmd(_12306_queryOrderWaitTime);
								iret = creq->parse(cmap, strack); // 查询席位分配	
								//pt->AddInfo("查询席位分配：%s", strack.c_str()); //{"tourFlag":"dc","waitTime":-1,"waitCount":0,"orderId":"E722753062","requestId":5798676501947326674,"count":0}
								if (jread.parse(strack, jvalue))
								{
									if (!jvalue["requestId"].isNull())
									{
										pt->AddInfo("系统分配席位成功，正在查询订单...");
										pt->AddInfo("订单流水号：%lld", jvalue["requestId"].asDouble());
										pt->AddInfo("恭喜，订票成功！请在45分钟内登陆 https://dynamic.12306.cn/otsweb/ 付款");
									}
									else
									{
										pt->AddInfo("查询席位失败：%s", strack.c_str());
									}
								}
								else
								{
									throw -1;
								}
								
								/*/ 查询【订单】
								iret = creq->parse(cmap, strack); // 查询订单
								pt->AddInfo("估计订票成功，查询订单: %s", strack.c_str()); // {"tourFlag":"dc","waitTime":4,"waitCount":0,"requestId":5798679846929932498,"count":-1}							
								if (jread.parse(strack, jvalue))
								{
									if (jvalue["waitTime"].asInt() > 0 && !jvalue["requestId"].isNull())
									{
										pt->AddInfo("恭喜，订票成功！流水号：%s，请登陆支付。", jvalue["requestId"].asCString());
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
								pt->AddInfo("未知异常:[%s]", strack.c_str());
							}
							pt->alertexit();
						}
					}
// 				}
// 				else
// 				{
// 					pt->AddInfo("异常错误(无法解析的数据): 验证码已经失效");
// 				}
			} while (1);
		}
		else
		{
			pt->AddInfo("未查询到所需车票，5s后继续查询");
		}
	}
	else
	{
		pt->AddInfo("未查询到任何车次，请确认填信息是否正确，5s后继续查询");
	}	
}
	pt->AddInfo("退出查询");
	return 0;
}

void CGetTicketDlg::OnBtnRcode() 
{
	// TODO: Add your control notification handler code here
	if (!m_bRunning)
	{
		return;
	}
	GetDlgItem(IDC_EDIT_RCODE)->EnableWindow(FALSE); // 无法输入
	m_cbtn.EnableWindow(FALSE); // 无法点击	
	SetDlgItemText(IDC_EDIT_RCODE, "加载中");
	bool bget = false;
	do
	{
		if (bget = creq->GetVertifyImg(pszautoimgpath, strRCodePath))
		{
			AddInfo("获取验证码成功：请输入验证码！");			
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
		AfxMessageBox("请输入验证码");
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
