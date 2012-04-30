
#pragma once

#include "ViewTree.h"
#include <map>
#include <string>

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// Construcción
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// Atributos
protected:
	typedef std::map<std::string, std::map<std::string, HTREEITEM>> TElementsWindow;

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

	std::map<std::string, HTREEITEM>	m_WindowsMap;
	TElementsWindow						m_ElementsWindowMap;					

	HTREEITEM							m_TreeSelected;
	std::string							m_WindowSelected;

protected:
	void FillFileView();

// Implementación
public:
	virtual ~CFileView();

private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Release();
	void ReloadAll();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	
	DECLARE_MESSAGE_MAP()
};

