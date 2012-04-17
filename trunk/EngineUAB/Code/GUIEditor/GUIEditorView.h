
// GUIEditorView.h: interfaz de la clase CGUIEditorView
//

#pragma once

#include "MyPicture.h"

class CEngine;
class CGUIEditorProcess;

class CGUIEditorView : public CView
{
protected: // Crear sólo a partir de serialización
	CGUIEditorView();
	DECLARE_DYNCREATE(CGUIEditorView)

// Atributos
public:
	CGUIEditorDoc* GetDocument() const;

// Operaciones
public:

// Reemplazos
public:
	virtual void OnDraw(CDC* pDC);  // Reemplazado para dibujar esta vista
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementación
public:
	virtual ~CGUIEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CMyPicture			m_Picture;
	CGUIEditorProcess	*m_pProcess;

	bool		m_bFirstTime;

private:
	//Métodos
	void CalculateWindow();

// Funciones de asignación de mensajes generadas
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

	BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // Versión de depuración en GUIEditorView.cpp
inline CGUIEditorDoc* CGUIEditorView::GetDocument() const
   { return reinterpret_cast<CGUIEditorDoc*>(m_pDocument); }
#endif

