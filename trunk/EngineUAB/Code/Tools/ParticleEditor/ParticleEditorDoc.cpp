
// ParticleEditorDoc.cpp: implementaci�n de la clase CParticleEditorDoc
//

#include "stdafx.h"
// Se pueden definir SHARED_HANDLERS en un proyecto ATL implementando controladores de vista previa, miniatura
// y filtro de b�squeda, y permiten compartir c�digo de documentos con ese proyecto.
#ifndef SHARED_HANDLERS
#include "ParticleEditor.h"
#endif

#include "ParticleEditorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CParticleEditorDoc

IMPLEMENT_DYNCREATE(CParticleEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CParticleEditorDoc, CDocument)
END_MESSAGE_MAP()


// Construcci�n o destrucci�n de CParticleEditorDoc

CParticleEditorDoc::CParticleEditorDoc()
{
	// TODO: agregar aqu� el c�digo de construcci�n �nico

}

CParticleEditorDoc::~CParticleEditorDoc()
{
}

BOOL CParticleEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: agregar aqu� c�digo de reinicio
	// (los documentos SDI volver�n a utilizar este documento)

	return TRUE;
}




// Serializaci�n de CParticleEditorDoc

void CParticleEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: agregar aqu� el c�digo de almacenamiento
	}
	else
	{
		// TODO: agregar aqu� el c�digo de carga
	}
}

#ifdef SHARED_HANDLERS

// Compatibilidad con miniaturas
void CParticleEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modifique este c�digo para dibujar los datos del documento
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Compatibilidad con controladores de b�squeda
void CParticleEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Establezca contenido de b�squeda a partir de los datos del documento. 
	// Las partes de contenido deben ir separadas por ";"

	// Por ejemplo:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CParticleEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Diagn�sticos de CParticleEditorDoc

#ifdef _DEBUG
void CParticleEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CParticleEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Comandos de CParticleEditorDoc
