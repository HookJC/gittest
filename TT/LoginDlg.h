#if !defined(AFX_LOGINDLG_H__366F999B_9B78_48B3_8292_2CE07766D64F__INCLUDED_)
#define AFX_LOGINDLG_H__366F999B_9B78_48B3_8292_2CE07766D64F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

#include "RCodeBtn.h"

class CLoginDlg : public CDialog
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor

	string m_strSession;

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGIN_DIALOG };
	CString	m_strRCode;
	CString	m_strUserName;
	CString	m_strPassWord;
	//}}AFX_DATA
	
	void SetCookieClass(LPVOID lpVoid);

	static UINT ThreadLoadInit(LPVOID lpVoid);

private:
	CRCodeBtn m_cbtn;
	BOOL LoginTicket();
	BOOL m_bLogin;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnBtnRcode();
	afx_msg void OnChangeEditRcode();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDLG_H__366F999B_9B78_48B3_8292_2CE07766D64F__INCLUDED_)
