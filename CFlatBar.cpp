// FlatBar.cpp 
////////////////////////////////////////////////////////////////
// If this code works, it was written by Suresh.
// If not, I don't know who wrote it.
//
#include "StdAfx.h"
#include "CFlatBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////
// CFlatToolBar--does flat tool bar in MFC.
//
IMPLEMENT_DYNAMIC(CFlatToolBar, CToolBar)

BEGIN_MESSAGE_MAP(CFlatToolBar, CToolBar)
   ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

////////////////
// Load override modifies the style after loading toolbar.
//
BOOL CFlatToolBar::LoadToolBar(LPCTSTR lpszResourceName)
{
   if (!CToolBar::LoadToolBar(lpszResourceName))
      return FALSE;
   ModifyStyle(0, TBSTYLE_FLAT); // make it flat
   return TRUE;
}

//////////////////
// MFC doesn't handle moving a TBSTYLE_FLAT toolbar correctly.
// The simplest way to fix it is to repaint the old rectangle and
// toolbar itself whenever the toolbar moves.
// 
void CFlatToolBar::OnWindowPosChanging(LPWINDOWPOS lpwp)
{
   CToolBar::OnWindowPosChanging(lpwp);

// #define ILLUSTRATE_DISPLAY_BUG        //  remove comment to see the bug
#ifndef ILLUSTRATE_DISPLAY_BUG

   if (!(lpwp->flags & SWP_NOMOVE)) {   //  if moved:
      CRect rc;                         //  Fill rectangle with..
      GetWindowRect(&rc);               //  ..my (toolbar) rectangle.
      CWnd* pParent = GetParent();      //  get parent (dock bar/frame) win..
      pParent->ScreenToClient(&rc);     //  .. and convert to parent coords

      // Ask parent window to paint the area beneath my old location.
      // Typically, this is just solid grey.
      //
      pParent->InvalidateRect(&rc);     //  paint old rectangle

      // Now paint my non-client area at the new location.
      // This is the extra bit of border space surrounding the buttons.
      // Without this, you will still have a partial display bug (try it!)
      //
      PostMessage(WM_NCPAINT);
   }
#endif
}