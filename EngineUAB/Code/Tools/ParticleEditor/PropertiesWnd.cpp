
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "ParticleEditor.h"
#include "Properties\PropertiesSaver.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Controladores de mensajes de CResourceViewBar

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Crear cuadro combinado:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("No se pudo crear el cuadro combinado Propiedades\n");
		return -1;      // no se pudo crear
	}

	m_wndObjectCombo.AddString(_T("Aplicación"));
	m_wndObjectCombo.AddString(_T("Ventana Propiedades"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("No se pudo crear la cuadrícula Propiedades \n");
		return -1;      // no se pudo crear
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Bloqueado */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Bloqueado */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// Todos los comandos se enrutarán a través de este control, no del marco primario:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Agregue aquí el código del controlador de comando
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Agregue aquí el código del controlador de interfaz de usuario de la actualización del comando
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Agregue aquí el código del controlador de comando
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Agregue aquí el código del controlador de interfaz de usuario de la actualización del comando
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pProperty = NULL;

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pColor = new CMFCPropertyGridProperty(_T("Color"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Minimum"), 0, TRUE);
	CMFCPropertyGridProperty *l_pRed	= new CMFCPropertyGridProperty(_T("Red"), (_variant_t)0, _T("Red channel for the minimum color."));
	CMFCPropertyGridProperty *l_pGreen	= new CMFCPropertyGridProperty(_T("Green"), (_variant_t)0, _T("Green channel for the minimum color."));
	CMFCPropertyGridProperty *l_pBlue	= new CMFCPropertyGridProperty(_T("Blue"), (_variant_t)0, _T("Blue channel for the minimum color."));
	CMFCPropertyGridProperty *l_pAlpha	= new CMFCPropertyGridProperty(_T("Alpha"), (_variant_t)255, _T("Alpha channel for the minimum color."));
	l_pRed->EnableSpinControl(TRUE, 0, 255);
	l_pGreen->EnableSpinControl(TRUE, 0, 255);
	l_pBlue->EnableSpinControl(TRUE, 0, 255);
	l_pAlpha->EnableSpinControl(TRUE, 0, 255);
	l_pProperty->AddSubItem(l_pRed);
	l_pProperty->AddSubItem(l_pGreen);
	l_pProperty->AddSubItem(l_pBlue);
	l_pProperty->AddSubItem(l_pAlpha);
	l_pColor->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Maximum"), 0, TRUE);
	l_pRed		= new CMFCPropertyGridProperty(_T("Red"), (_variant_t)0, _T("Red channel for the maximum color."));
	l_pGreen	= new CMFCPropertyGridProperty(_T("Green"), (_variant_t)0, _T("Green channel for the maximum color."));
	l_pBlue		= new CMFCPropertyGridProperty(_T("Blue"), (_variant_t)0, _T("Blue channel for the maximum color."));
	l_pAlpha	= new CMFCPropertyGridProperty(_T("Alpha"), (_variant_t)255, _T("Alpha channel for the maximum color."));
	l_pRed->EnableSpinControl(TRUE, 0, 255);
	l_pGreen->EnableSpinControl(TRUE, 0, 255);
	l_pBlue->EnableSpinControl(TRUE, 0, 255);
	l_pAlpha->EnableSpinControl(TRUE, 0, 255);
	l_pProperty->AddSubItem(l_pRed);
	l_pProperty->AddSubItem(l_pGreen);
	l_pProperty->AddSubItem(l_pBlue);
	l_pProperty->AddSubItem(l_pAlpha);
	l_pColor->AddSubItem(l_pProperty);
	m_wndPropList.AddProperty(l_pColor);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pDuration = new CMFCPropertyGridProperty(_T("Duration"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Duration"), (_variant_t)1.0f, _T("Duration of the particle."));
	l_pDuration->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Randomness"), (_variant_t)1.0f, _T("Duration randomness of the particle."));
	l_pDuration->AddSubItem(l_pProperty);
	m_wndPropList.AddProperty(l_pDuration);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pGeneral = new CMFCPropertyGridProperty(_T("General"));
	static const TCHAR l_Filter[] = _T("JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|BMP(*.bmp)|*.bmp|TGA(*.tga)|*.tga|DDS(*.dds)|*.dds|Todos los archivos(*.*)|*.*||");
	l_pGeneral->AddSubItem(new CMFCPropertyGridFileProperty(_T("Texture"), TRUE, _T(""), _T("png"), 0, l_Filter, _T("Specify the texture.")));
	l_pProperty = new CMFCPropertyGridProperty(_T("Quantity"), (_variant_t)300, _T("Maximum of particles for emitter."));
	l_pProperty->EnableSpinControl(TRUE, 0, 10000);
	l_pGeneral->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Blend"), _T("NonPremultiplied"), _T("Type of blend."));
	l_pProperty->AddOption(_T("DefaultState"));
	l_pProperty->AddOption(_T("NonPremultiplied"));
	l_pProperty->AddOption(_T("Additive"));
	l_pGeneral->AddSubItem(l_pProperty);
	m_wndPropList.AddProperty(l_pGeneral);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pSpeed = new CMFCPropertyGridProperty(_T("Rotate Speed"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Minimum"), (_variant_t)0.0f, _T("Duration of the particle."));
	l_pSpeed->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Maximum"), (_variant_t)1.0f, _T("Duration randomness of the particle."));
	l_pSpeed->AddSubItem(l_pProperty);
	m_wndPropList.AddProperty(l_pSpeed);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pSize = new CMFCPropertyGridProperty(_T("Size"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Start"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)-1.0f, _T("Minimum start size.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)1.0f, _T("Maximum start size.")));
	l_pSize->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("End"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)-1.0f, _T("Minimum end size.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)1.0f, _T("Maximum end size.")));
	l_pSize->AddSubItem(l_pProperty);
	m_wndPropList.AddProperty(l_pSize);

	//---------------------------------------------------
	CMFCPropertyGridProperty *l_pVelocity = new CMFCPropertyGridProperty(_T("Velocity"));
	l_pProperty = new CMFCPropertyGridProperty(_T("Sensitivity"), (_variant_t)1.0f, _T("Sensitivity of the velocity."));
	l_pVelocity->AddSubItem(l_pProperty);	
	l_pProperty = new CMFCPropertyGridProperty(_T("Horizontal"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)-1.0f, _T("Minimum horizontal velocity.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)1.0f, _T("Maximum horizontal velocity.")));
	l_pVelocity->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Vertical"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Min"), (_variant_t)-1.0f, _T("Minimum vertical velocity.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Max"), (_variant_t)1.0f, _T("Maximum vertical velocity.")));
	l_pVelocity->AddSubItem(l_pProperty);
	l_pProperty = new CMFCPropertyGridProperty(_T("Gravity"), 0, TRUE);
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("X"), (_variant_t)0.0f, _T("Gravity in the X axis.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Y"), (_variant_t)0.0f, _T("Gravity in the Y axis.")));
	l_pProperty->AddSubItem(new CMFCPropertyGridProperty(_T("Z"), (_variant_t)0.0f, _T("Gravity in the Z axis.")));
	l_pVelocity->AddSubItem(l_pProperty);	
	l_pProperty = new CMFCPropertyGridProperty(_T("End"), (_variant_t)1.0f, _T("Final velocity."));
	l_pVelocity->AddSubItem(l_pProperty);	
	m_wndPropList.AddProperty(l_pVelocity);

	CPropertiesSaver::GetInstance()->SetProperties(&m_wndPropList);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}
