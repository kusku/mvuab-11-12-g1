#if !defined(AFX_MYPICTURE_H__DDB812F2_70F2_4553_AC61_C8659B918BFD__INCLUDED_)
#define AFX_MYPICTURE_H__DDB812F2_70F2_4553_AC61_C8659B918BFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPicture.h : header file
//

#include "Math\Vector2.h"

/////////////////////////////////////////////////////////////////////////////
// CMyPicture window

class CMyPicture : public CStatic
{
// Construction
public:
	CMyPicture();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPicture)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyPicture();

private:
	float m_R;
	bool m_bIsLMouseDown;

	Vect2i m_LButtonPosition;
	Vect2i m_ContextMenuPosition;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPicture)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPICTURE_H__DDB812F2_70F2_4553_AC61_C8659B918BFD__INCLUDED_)
