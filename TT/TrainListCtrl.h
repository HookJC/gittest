#if !defined(AFX_TRAINLISTCTRL_H__F3B534D5_2AF5_4528_B54E_384EE35AEADD__INCLUDED_)
#define AFX_TRAINLISTCTRL_H__F3B534D5_2AF5_4528_B54E_384EE35AEADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrainListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrainListCtrl window

class CTrainListCtrl : public CListCtrl
{
// Construction
public:
	CTrainListCtrl();

	CString m_strMatchRedColor;
	CString m_strMatchBlueColor;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrainListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrainListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTrainListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAINLISTCTRL_H__F3B534D5_2AF5_4528_B54E_384EE35AEADD__INCLUDED_)
