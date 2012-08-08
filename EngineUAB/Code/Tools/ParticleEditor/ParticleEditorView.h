
// ParticleEditorView.h: interfaz de la clase CParticleEditorView
//

#pragma once

#include "MyPicture.h"
class CParticleEditorProcess;

class CParticleEditorView : public CView
{
protected: // Crear s�lo a partir de serializaci�n
	CParticleEditorView();
	DECLARE_DYNCREATE(CParticleEditorView)

// Atributos
public:
	CParticleEditorDoc* GetDocument() const;

// Operaciones
public:

// Reemplazos
public:
	virtual void OnDraw(CDC* pDC);  // Reemplazado para dibujar esta vista
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementaci�n
public:
	virtual ~CParticleEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CMyPicture			m_Picture;
	CParticleEditorProcess	*m_pProcess;

	bool		m_bFirstTime;
	CRect		m_PictureRect;

private:
	//M�todos
	void CalculateWindow();

// Funciones de asignaci�n de mensajes generadas
protected:
	afx_msg void OnPointEmitter();
	afx_msg void OnLineEmitter();
	afx_msg void OnRingEmitter();
	afx_msg void OnBoxEmitter();
	afx_msg void OnSphereEmitter();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnSaveData();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Versi�n de depuraci�n en ParticleEditorView.cpp
inline CParticleEditorDoc* CParticleEditorView::GetDocument() const
   { return reinterpret_cast<CParticleEditorDoc*>(m_pDocument); }
#endif

