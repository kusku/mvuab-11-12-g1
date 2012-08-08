
// ParticleEditor.cpp : define los comportamientos de las clases para la aplicaci�n.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ParticleEditor.h"
#include "MainFrm.h"

#include "ParticleEditorDoc.h"
#include "ParticleEditorView.h"

#include "Engine.h"
#include "EngineProcess.h"
#include "EngineManager.h"
#include "ParticleEditorProcess.h"
#include "HWNDManager.h"
#include "defines.h"
#include "Core.h"
#include "Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParticleEditorApp

BEGIN_MESSAGE_MAP(CParticleEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CParticleEditorApp::OnAppAbout)
	// Comandos de documento est�ndar basados en archivo
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_NEW, &CParticleEditorApp::OnNewSystem)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// Construcci�n de CParticleEditorApp

CParticleEditorApp::CParticleEditorApp()
{
	m_bHiColorIcons = TRUE;

	m_bStarting = true;

	// TODO: reemplace la cadena de identificador de aplicaci�n siguiente por una cadena de identificador �nico; el formato
	// recomendado para la cadena es NombreCompa��a.NombreProducto.Subproducto.Informaci�nDeVersi�n
	SetAppID(_T("ParticleEditor.AppID.NoVersion"));

	// TODO: agregar aqu� el c�digo de construcci�n,
	// Colocar toda la inicializaci�n importante en InitInstance

	CEngine *engine = new CEngine();
	CEngineManager::GetInstance()->SetEngine(engine);
}

// El �nico objeto CParticleEditorApp

CParticleEditorApp theApp;


// Inicializaci�n de CParticleEditorApp

BOOL CParticleEditorApp::InitInstance()
{
	CWinAppEx::InitInstance();


	// Inicializar bibliotecas OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// Se necesita AfxInitRichEdit2() para usar el control RichEdit	
	// AfxInitRichEdit2();

	// Inicializaci�n est�ndar
	// Si no utiliza estas caracter�sticas y desea reducir el tama�o
	// del archivo ejecutable final, debe quitar
	// las rutinas de inicializaci�n espec�ficas que no necesite
	// Cambie la clave del Registro en la que se almacena la configuraci�n
	// TODO: debe modificar esta cadena para que contenga informaci�n correcta
	// como el nombre de su compa��a u organizaci�n
	SetRegistryKey(_T("Aplicaciones generadas con el Asistente para aplicaciones local"));
	LoadStdProfileSettings(4);  // Cargar opciones de archivo INI est�ndar (incluidas las de la lista MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Registrar las plantillas de documento de la aplicaci�n. Las plantillas de documento
	//  sirven como conexi�n entre documentos, ventanas de marco y vistas
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CParticleEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // Ventana de marco SDI principal
		RUNTIME_CLASS(CParticleEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Analizar l�nea de comandos para comandos Shell est�ndar, DDE, Archivo Abrir
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Enviar comandos especificados en la l�nea de comandos. Devolver� FALSE si
	// la aplicaci�n se inici� con los modificadores /RegServer, /Register, /Unregserver o /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Se ha inicializado la �nica ventana; mostrarla y actualizarla
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// Llamar a DragAcceptFiles s�lo si existe un sufijo
	//  En una aplicaci�n SDI, esto debe ocurrir despu�s de ProcessShellCommand
	return TRUE;
}

int CParticleEditorApp::ExitInstance()
{
	//TODO: controlar recursos adicionales que se hayan podido agregar
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// Controladores de mensaje de CParticleEditorApp


// Cuadro de di�logo CAboutDlg utilizado para el comando Acerca de

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Datos del cuadro de di�logo
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

// Implementaci�n
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Comando de la aplicaci�n para ejecutar el cuadro de di�logo
void CParticleEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// M�todos para cargar o guardar personalizaciones de  CParticleEditorApp

void CParticleEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CParticleEditorApp::LoadCustomState()
{
}

void CParticleEditorApp::SaveCustomState()
{
}

// Controladores de mensaje de CParticleEditorApp


void CParticleEditorApp::OnNewSystem()
{
	if( m_bStarting )
	{
		CWinAppEx::OnFileNew();
		m_bStarting = false;
	}
	else
	{
		CString *name = new CString("");
		PostMessage(CHWNDManager::GetInstance()->GetHWNDFiles(), WM_ADD_NEW_SYSTEM, (WPARAM)name, 0); 
		m_bStarting = false;
	}
}

