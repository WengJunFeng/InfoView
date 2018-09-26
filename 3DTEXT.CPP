/////////////////////////////////////////////////////////////////////////////
// 3DTextButton control
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3DText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3dText

BEGIN_MESSAGE_MAP(C3dText, CButton)
	//{{AFX_MSG_MAP(C3dText)     
	ON_WM_DRAWITEM()     
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAPEND_MESSAGE_MAP()
END_MESSAGE_MAP()

void C3dText::PreSubclassWindow() 
{
	SetButtonStyle(GetButtonStyle() | BS_OWNERDRAW);
}

/////////////////////////////////////////////////////////////////////////////
// C3dText message handlers

void C3dText::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);     
	ASSERT_VALID(pDC);
	CRect rectClient = lpDrawItemStruct->rcItem;
	Draw(pDC,rectClient, lpDrawItemStruct->itemState);
}

void C3dText::Draw(CDC* pDC, const CRect& rect, UINT state) 
{
	CString text;
	GetWindowText(text);
	int l = text.GetLength();
	CRect rectClient = rect;     // get font from control
	CFont* pFont = GetFont();

	// ensure we have a valid height and width and select the font
	LOGFONT logfont;
	pFont->GetObject(sizeof(LOGFONT),&logfont);
	if (logfont.lfHeight == 0)
	logfont.lfHeight		= 20;
	logfont.lfWidth			= 0;     // 0 so it will be calculated
	logfont.lfWeight		= 1000;
	logfont.lfEscapement	= logfont.lfOrientation = 0;

	CFont tryfont; 
	VERIFY(tryfont.CreateFontIndirect(&logfont));
	CFont* pFontOld = pDC->SelectObject(&tryfont);

	// get the control size and adjust font width & height accordingly
	if (m_bUse3D)
	{
		rectClient.DeflateRect(3,3);
	}

	CSize textSizeClient = pDC->GetTextExtent(text,l);

	if (rectClient.Width()*textSizeClient.cy > rectClient.Height()*textSizeClient.cx)
	{
		logfont.lfHeight = ::MulDiv(logfont.lfHeight,rectClient.Height(),textSizeClient.cy);
	} 
	else 
	{
		logfont.lfHeight = ::MulDiv(logfont.lfHeight,rectClient.Width(),textSizeClient.cx);     
	}

	logfont.lfHeight--; // fudge factor

	if (m_bUse3D)
	{
		rectClient.InflateRect(3,3);
	}

	// create adjusted font and select
	CFont font; 
	font.CreateFontIndirect(&logfont);

	pDC->SelectObject(&font);
	textSizeClient = pDC->GetTextExtent(text,l);

	int minx		= rectClient.left+(rectClient.Width()-textSizeClient.cx)/2;
	int miny		= rectClient.top+(rectClient.Height()-textSizeClient.cy)/2;
	int oldBkMode	= pDC->SetBkMode(TRANSPARENT);

	COLORREF textcol = ::GetSysColor((state & ODS_FOCUS) ? COLOR_GRAYTEXT: COLOR_BTNTEXT);
	COLORREF oldTextColor = pDC->SetTextColor(textcol);

	int cx = minx;
	int cy = miny;  
	if (m_bUse3D) 
	{
		int s = (state & ODS_SELECTED) ? -1 : +1;
		cx += 3; cy += 3;

		// draw 3D highlights
		pDC->SetTextColor(::GetSysColor(COLOR_3DDKSHADOW));
		pDC->TextOut(cx-s*2,cy+s*2,text);
		pDC->TextOut(cx+s*2,cy-s*2,text);
		pDC->TextOut(cx+s*2,cy+s*2,text);
		pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
		pDC->TextOut(cx+s*1,cy-s*2,text);
		pDC->TextOut(cx-s*2,cy+s*1,text);
		pDC->TextOut(cx-s*2,cy-s*2,text);
		pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
		pDC->TextOut(cx-s*1,cy+s*1,text);
		pDC->TextOut(cx+s*1,cy-s*1,text);
		pDC->TextOut(cx+s*1,cy+s*1,text);
		pDC->SetTextColor(::GetSysColor(COLOR_3DLIGHT));
		pDC->TextOut(cx,cy-s*1,text);          
		pDC->TextOut(cx-s*1,cy,text);
		pDC->TextOut(cx-s*1,cy-s*1,text);          
		pDC->SetTextColor(textcol);
	}     

	// draw the text     
	pDC->TextOut(cx,cy,text);     

	// restore DC
	pDC->SetTextColor(oldTextColor);     
	pDC->SetBkMode(oldBkMode);
	pDC->SelectObject(pFontOld);
}

BOOL C3dText::OnEraseBkgnd(CDC*) 
{
	return true;    // we don't do any erasing...
}