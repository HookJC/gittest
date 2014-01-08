#if !defined(AFX_RCODEBTN_H__01FCDE32_B73A_4F1B_B69F_D518C420D590__INCLUDED_)
#define AFX_RCODEBTN_H__01FCDE32_B73A_4F1B_B69F_D518C420D590__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCodeBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRCodeBtn window

typedef HBITMAP (*LoadImageFunc)(const char *pStrFileName);

class CRCodeBtn : public CButton
{
// Construction
public:
	CRCodeBtn();

	LoadImageFunc m_funLoadAtlImage;
	HMODULE m_hModule;

// Attributes
public:
	void Reflush();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCodeBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRCodeBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRCodeBtn)
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCODEBTN_H__01FCDE32_B73A_4F1B_B69F_D518C420D590__INCLUDED_)
