// GetTicket.h : main header file for the GETTICKET application
//

#if !defined(AFX_GETTICKET_H__963693A4_F9D7_4CD1_A98D_5DC6AD175B80__INCLUDED_)
#define AFX_GETTICKET_H__963693A4_F9D7_4CD1_A98D_5DC6AD175B80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGetTicketApp:
// See GetTicket.cpp for the implementation of this class
//

class CGetTicketApp : public CWinApp
{
public:
	CGetTicketApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetTicketApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGetTicketApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETTICKET_H__963693A4_F9D7_4CD1_A98D_5DC6AD175B80__INCLUDED_)
