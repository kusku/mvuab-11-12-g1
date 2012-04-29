
// GUIEditorView.cpp: implementación de la clase CGUIEditorView
//

#include "stdafx.h"
// Se pueden definir SHARED_HANDLERS en un proyecto ATL implementando controladores de vista previa, miniatura
// y filtro de búsqueda, y permiten compartir código de documentos con ese proyecto.
#ifndef SHARED_HANDLERS
#include "GUIEditor.h"
#endif

#include "GUIEditorDoc.h"
#include "GUIEditorView.h"

#include "OutputWnd.h"

#include "Engine.h"
#include "GUIEditorProcess.h"
#include "Base.h"
#include "defines.h"
#include "EngineManager.h"
#include "InOut\XMLManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGUIEditorView

IMPLEMENT_DYNCREATE(CGUIEditorView, CView)

BEGIN_MESSAGE_MAP(CGUIEditorView, CView)
	// Comandos de impresión estándar
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGUIEditorView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_SAVE, &CGUIEditorView::OnSaveData)
	ON_COMMAND(ID_FILE_SAVE_AS, &CGUIEditorView::OnSaveData)
	//ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Construcción o destrucción de CGUIEditorView

CGUIEditorView::CGUIEditorView()
	: m_bFirstTime(true)
	, m_PictureRect(0, 0, 0, 0)
{
	
	//-----------Per a detectar Memory Leaks-------------------------
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc (324475);
	//---------------------------------------------------------------//

	CEngineManager::GetInstance()->GetEngine()->LoadConfigXML("./Data/XML/engine.xml");
}

CGUIEditorView::~CGUIEditorView()
{
}

BOOL CGUIEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificar aquí la clase Window o los estilos cambiando
	//  CREATESTRUCT cs
	
	return CView::PreCreateWindow(cs);
}

// Dibujo de CGUIEditorView

void CGUIEditorView::OnDraw(CDC* /*pDC*/)
{
	CGUIEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if( m_bFirstTime )
	{
		CalculateWindow();
	}

	InvalidateRect(NULL,FALSE);

	CEngineManager::GetInstance()->GetEngine()->Update();
	CEngineManager::GetInstance()->GetEngine()->Render();
}

void CGUIEditorView::CalculateWindow()
{
	CRect rect;
	GetClientRect(rect);
	rect.left = 100;
	rect.bottom = 480;
	rect.right = 740;
	
	m_PictureRect = rect;

	m_Picture.DestroyWindow();
	BOOL isOk = m_Picture.Create(LPCTSTR(""), SS_BLACKFRAME | SS_NOTIFY | SS_SUNKEN | WS_BORDER, rect, this);
	m_Picture.ShowWindow(SW_SHOW);
	m_Picture.UpdateWindow();

	CEngineManager::GetInstance()->GetEngine()->Init( m_Picture.m_hWnd );
	m_bFirstTime = false;
}

// Impresión de CGUIEditorView
void CGUIEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGUIEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Preparación predeterminada
	return DoPreparePrinting(pInfo);
}

void CGUIEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: agregar inicialización adicional antes de imprimir
}

void CGUIEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: agregar limpieza después de imprimir
}

void CGUIEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGUIEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CGUIEditorView::OnSaveData()
{
	CString nom;
	FILE* file = NULL;

	//  Obrir diàleg d'escriptura de fitxer (fitxers *.MNT)
	CFileDialog openExportXML (FALSE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
		_T("XML Files(*.xml)|*.xml|All Files (*.*)|*.*||"));;

	if (openExportXML.DoModal() != IDOK)
		return;                 // stay with old data file
    else
	   {nom=openExportXML.GetPathName();}

	nom = nom + CString(".xml");
	// Conversió de la variable CString nom a la variable char *nomfitx, compatible amb la funció carregar3DS
	char * nomfitx = (char *)(LPCTSTR)nom;
	std::string l_szName = nomfitx;

	CXMLManager::SaveFiles( l_szName );
}

// Diagnósticos de CGUIEditorView

#ifdef _DEBUG
void CGUIEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CGUIEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGUIEditorDoc* CGUIEditorView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUIEditorDoc)));
	return (CGUIEditorDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CGUIEditorView::PreTranslateMessage(MSG* pMsg)
{
	return CView::PreTranslateMessage(pMsg);
}

// Controladores de mensaje de CGUIEditorView
