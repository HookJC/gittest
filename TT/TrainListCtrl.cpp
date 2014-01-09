// TrainListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "getticket.h"
#include "TrainListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrainListCtrl

CTrainListCtrl::CTrainListCtrl()
{
}

CTrainListCtrl::~CTrainListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTrainListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CTrainListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainListCtrl message handlers

void CTrainListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
    // Take the default processing unless we set this to something else below.
    *pResult = 0;
	CListCtrl& m_ListCtrl = *this;
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
    if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);   
		CString strTemp = m_ListCtrl.GetItemText(nItem, 0);
		strTemp = "#" + strTemp;
		strTemp += "#";

		if (!m_strMatchRedColor.IsEmpty() && m_strMatchRedColor.Find(strTemp) > -1)
		{
			pLVCD->clrText = RGB(255, 0, 0); //ºì
		}
		else
		{
			strTemp = m_ListCtrl.GetItemText(nItem, 15);
			if (m_strMatchBlueColor.Find(strTemp))
			{
				pLVCD->clrText   = RGB(0, 0, 255);
			}
			else
			{
				pLVCD->clrText = RGB(0, 0, 0); //ºÚ
			}
		}		
		
        *pResult = CDRF_DODEFAULT;
	}
}

