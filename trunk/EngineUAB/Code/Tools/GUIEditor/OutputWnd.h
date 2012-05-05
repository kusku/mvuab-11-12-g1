
#pragma once

/////////////////////////////////////////////////////////////////////////////
// Ventana de COutputList

#include "Utils\Types.h"

class COutputList : public CListBox
{
// Construcción
public:
	COutputList();

// Implementación
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	//afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
// Construcción
public:
	COutputWnd();

	void UpdateFonts();
	
// Atributos
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputLog;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

	uint32 m_uNumOfLogs;

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void FillFindWindow();

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementación
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
};

