
// ParticleEditorView.cpp: implementación de la clase CParticleEditorView
//

#include "stdafx.h"
// Se pueden definir SHARED_HANDLERS en un proyecto ATL implementando controladores de vista previa, miniatura
// y filtro de búsqueda, y permiten compartir código de documentos con ese proyecto.
#ifndef SHARED_HANDLERS
#include "ParticleEditor.h"
#endif

#include "ParticleEditorDoc.h"
#include "ParticleEditorView.h"
#include "ParticleEditorProcess.h"

#include "Particles\ParticleEmitterManager.h"
#include "EngineManager.h"
#include "Engine.h"
#include "Scripting\ScriptManager.h"
#include "Utils\Active.h"
#include "Base.h"
#include "Core.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParticleEditorView

IMPLEMENT_DYNCREATE(CParticleEditorView, CView)

BEGIN_MESSAGE_MAP(CParticleEditorView, CView)
	ON_COMMAND(ID_POINT, &CParticleEditorView::OnPointEmitter)
	ON_COMMAND(ID_LINE, &CParticleEditorView::OnLineEmitter)
	ON_COMMAND(ID_RING, &CParticleEditorView::OnRingEmitter)
	ON_COMMAND(ID_BOX, &CParticleEditorView::OnBoxEmitter)
	ON_COMMAND(ID_SPHERE, &CParticleEditorView::OnSphereEmitter)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Construcción o destrucción de CParticleEditorView

CParticleEditorView::CParticleEditorView()
	: m_bFirstTime(true)
	, m_PictureRect(0, 0, 0, 0)
{
	//-----------Per a detectar Memory Leaks-------------------------
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc (324475);
	//---------------------------------------------------------------//

	CEngineManager::GetInstance()->GetEngine()->LoadConfigXML("./Data/XML/engine.xml");
}

CParticleEditorView::~CParticleEditorView()
{
	CHECKED_DELETE(m_pProcess);
}

BOOL CParticleEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificar aquí la clase Window o los estilos cambiando
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Dibujo de CParticleEditorView

void CParticleEditorView::OnDraw(CDC* /*pDC*/)
{
	CParticleEditorDoc* pDoc = GetDocument();
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

void CParticleEditorView::CalculateWindow()
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

	m_pProcess = new CParticleEditorProcess();
	CEngineManager::GetInstance()->GetEngine()->SetProcess(m_pProcess);

	CEngineManager::GetInstance()->GetEngine()->Init( m_Picture.m_hWnd );
	CORE->GetScriptManager()->RunCode("load_basics()");
	CORE->GetScriptManager()->RunCode("load_data()");
	CORE->SetProcess( m_pProcess );
	CORE->SetGameMode(true);

	m_bFirstTime = false;
}

void CParticleEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CParticleEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

//------------------------------------------
// Buttons
//------------------------------------------
void CParticleEditorView::OnPointEmitter()
{
	CParticleEditorProcess::DisableAllEmitters();
	CORE->GetParticleEmitterManager()->GetResource("point_emitter_editor")->SetActive(true);
}

void CParticleEditorView::OnLineEmitter()
{
	CParticleEditorProcess::DisableAllEmitters();
	CORE->GetParticleEmitterManager()->GetResource("line_emitter_editor")->SetActive(true);
}

void CParticleEditorView::OnRingEmitter()
{
	CParticleEditorProcess::DisableAllEmitters();
	CORE->GetParticleEmitterManager()->GetResource("ring_emitter_editor")->SetActive(true);
}

void CParticleEditorView::OnBoxEmitter()
{
	CParticleEditorProcess::DisableAllEmitters();
	CORE->GetParticleEmitterManager()->GetResource("box_emitter_editor")->SetActive(true);
}

void CParticleEditorView::OnSphereEmitter()
{
	CParticleEditorProcess::DisableAllEmitters();
	CORE->GetParticleEmitterManager()->GetResource("sphere_emitter_editor")->SetActive(true);
}

//------------------------------------------
// Diagnósticos de CParticleEditorView
//------------------------------------------
#ifdef _DEBUG
void CParticleEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CParticleEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CParticleEditorDoc* CParticleEditorView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CParticleEditorDoc)));
	return (CParticleEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// Controladores de mensaje de CParticleEditorView
