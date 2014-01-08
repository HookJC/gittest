// GetTicketDlg.h : header file
//

#if !defined(AFX_GETTICKETDLG_H__2474A688_325D_4658_831F_18CF115BC59F__INCLUDED_)
#define AFX_GETTICKETDLG_H__2474A688_325D_4658_831F_18CF115BC59F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGetTicketDlg dialog

#include "RCodeBtn.h"

class CGetTicketDlg : public CDialog
{
// Construction
public:
	CGetTicketDlg(CWnd* pParent = NULL);	// standard constructor

	CRCodeBtn m_cbtn;

	void alertexit();

// Dialog Data
	//{{AFX_DATA(CGetTicketDlg)
	enum { IDD = IDD_GETTICKET_DIALOG };
	CEdit	m_cInfo;
	CListCtrl	m_listTicket;
	CListCtrl	m_listMan;
	CMonthCalCtrl	m_cDate;
	int		m_nSeatType;
	CString	m_strEndCity;
	CString	m_strRCode;
	CString	m_strStartCity;
	CString	m_strTrainNo;
	CString	m_strName;
	CString	m_strVerifyCode;
	int		m_nTicketType;
	CString	m_strStartDate;
	CString	m_strMobile;
	UINT	m_uiTmGetTicket;
	UINT	m_uiTmSubmitTicket;
	CString	m_strStartTime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetTicketDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:	
	void AddInfo(const char* fmt, ...);

	static UINT GetPassernger(LPVOID lpVoid); // 获取联系人	
	
	BOOL m_bRunning; // 查询购票信息
	HANDLE m_hdWaitCode; // 等待验证码输入
	ulong m_nGetTicketTimes;
	static UINT GetTickets(LPVOID lpVoid);

private:
	void InitManList();
	void InitTicketList();

	void InitParam();

	


	int m_iSelMan;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGetTicketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClickListMan(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnRunning();
	afx_msg void OnBtnRcode();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnGetticket();
	afx_msg void OnChangeEditRcode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETTICKETDLG_H__2474A688_325D_4658_831F_18CF115BC59F__INCLUDED_)
