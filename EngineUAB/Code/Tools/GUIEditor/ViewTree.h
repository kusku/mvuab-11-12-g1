
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Ventana de CViewTree

class CViewTree : public CTreeCtrl
{
// Construcci�n
public:
	CViewTree();

// Invalidaciones
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementaci�n
public:
	virtual ~CViewTree();

protected:
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	DECLARE_MESSAGE_MAP()
};
