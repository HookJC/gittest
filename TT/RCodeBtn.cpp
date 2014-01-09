// RCodeBtn.cpp : implementation file
//

#include "stdafx.h"
#include "GetTicket.h"
#include "RCodeBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCodeBtn

CRCodeBtn::CRCodeBtn()
{
	m_funLoadAtlImage = NULL;
	m_hModule = ::LoadLibrary((g_strapppath + "\\AtlImage.dll").c_str());
}

CRCodeBtn::~CRCodeBtn()
{
	if (!m_hModule)
	{
		FreeLibrary(m_hModule);
	}
}


BEGIN_MESSAGE_MAP(CRCodeBtn, CButton)
	//{{AFX_MSG_MAP(CRCodeBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRCodeBtn message handlers

void CRCodeBtn::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PreSubclassWindow();

	ModifyStyle(0, BS_OWNERDRAW);
}

void CRCodeBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	// 绘制验证码
	if(!m_hModule)
	{
		AfxMessageBox("缺少动态链接库:AtlImage.dll");
		m_hModule=NULL;
	}
	
	m_funLoadAtlImage = (LoadImageFunc)::GetProcAddress(m_hModule, "LoadAtlImage");
	if (!m_funLoadAtlImage)
	{
		AfxMessageBox("Function Miss : LoadAtlImage");
		m_funLoadAtlImage=NULL;
	}
	
	HBITMAP hBitmap = m_funLoadAtlImage((g_strapppath + "\\RandCode.jpg").c_str());
	if (!hBitmap)
	{
		return;
	}

	CDC* pDC = GetDC();
	CRect rect;
	pDC->GetClipBox(&rect);
	CBitmap* pbitmap = CBitmap::FromHandle(hBitmap);
	DIBSECTION ds;  
	BITMAPINFOHEADER &bminfo = ds.dsBmih;  
	pbitmap->GetObject( sizeof(ds), &ds ); 
	int cx=bminfo.biWidth; //得到图像宽度 
	int cy=bminfo.biHeight; //得到图像高度
	
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);		
	memDC.SelectObject(pbitmap);
	
	//pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, cx, cy, SRCCOPY);
	pDC->BitBlt(0, 0, cx, cy, &memDC, 0, 0, SRCCOPY);
	pbitmap->DeleteObject();
	memDC.DeleteDC();
}

void CRCodeBtn::Reflush()
{
	Invalidate(FALSE);
}

