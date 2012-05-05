
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Ventana de CViewTree

class CViewTree : public CTreeCtrl
{
// Construcción
public:
	CViewTree();

// Invalidaciones
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementación
public:
	virtual ~CViewTree();

protected:
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	DECLARE_MESSAGE_MAP()
};
