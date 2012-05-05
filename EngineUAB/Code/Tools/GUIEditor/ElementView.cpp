
#include "stdafx.h"
#include "MainFrm.h"
#include "ElementView.h"
#include "Resource.h"
#include "GUIEditor.h"
#include "defines.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construcción/Destrucción
//////////////////////////////////////////////////////////////////////

CElementView::CElementView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;

	//Add Elements
	m_ElementsMap["Animated Image"] = NULL;
	m_ElementsMap["Button"] = NULL;
	m_ElementsMap["Check Button"] = NULL;
	m_ElementsMap["Dialog Box"] = NULL;
	m_ElementsMap["Editable Text Box"] = NULL;
	m_ElementsMap["Image"] = NULL;
	m_ElementsMap["Progress Bar"] = NULL;
	m_ElementsMap["Radio Box"] = NULL;
	m_ElementsMap["Slider"] = NULL;
	m_ElementsMap["Static Text"] = NULL;
}

CElementView::~CElementView()
{
	m_ElementsMap.clear();
}

BEGIN_MESSAGE_MAP(CElementView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Controladores de mensajes de CClassView

int CElementView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Crear vistas:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndElementView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("No se pudo crear la Vista de clases\n");
		return -1;      // no se pudo crear
	}

	// Cargar imágenes:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Bloqueado */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// Todos los comandos se enrutarán a través de este control, no del marco primario:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Rellenar con algunos datos de vista de árbol estática (código ficticio, nada especial)
	FillClassView();

	return 0;
}

void CElementView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CElementView::FillClassView()
{
	HTREEITEM hRoot = m_wndElementView.InsertItem(_T("Tools"), 0, 0);
	m_wndElementView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	
	std::map<std::string, HTREEITEM>::iterator l_It = m_ElementsMap.begin();
	std::map<std::string, HTREEITEM>::iterator l_End = m_ElementsMap.end();

	for(; l_It != l_End; ++l_It)
	{
		HTREEITEM item = m_wndElementView.InsertItem(_T( l_It->first.c_str() ), 1, 1, hRoot);
		l_It->second = item;
	}

	m_wndElementView.Expand(hRoot, TVE_EXPAND);
}

void CElementView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndElementView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Seleccionar elemento elegido:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CElementView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndElementView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CElementView::PreTranslateMessage(MSG* pMsg)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndElementView;
	switch(pMsg->message)
	{
	case WM_LBUTTON_DOWN:
		{
			HTREEITEM item = (HTREEITEM)pMsg->wParam;
			pWndTree->SelectItem(item);

			for (TElementsMap::iterator it = m_ElementsMap.begin(); it != m_ElementsMap.end(); ++it)
			{
				if (it->second == item)
				{
					CElementManager::GetInstance()->SetElementToAdd( String2Element(it->first) );
				}
			}
			break;
		}
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}

void CElementView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CElementView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CElementView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Agregar función de miembro..."));
}

void CElementView::OnClassAddMemberVariable()
{
	// TODO: Agregue aquí el código del controlador de comando
}

void CElementView::OnClassDefinition()
{
	// TODO: Agregue aquí el código del controlador de comando
}

void CElementView::OnClassProperties()
{
	// TODO: Agregue aquí el código del controlador de comando
}

void CElementView::OnNewFolder()
{
	AfxMessageBox(_T("Nueva carpeta..."));
}

void CElementView::OnPaint()
{
	CPaintDC dc(this); // contexto de dispositivo para dibujar

	CRect rectTree;
	m_wndElementView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CElementView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndElementView.SetFocus();
}

void CElementView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("No se puede cargar el mapa de bits: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndElementView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Bloqueado */);
}

TElement CElementView::String2Element( const std::string &element )
{
	if( element == "Animated Image" ) return ANIMATED_IMAGE;
	else if( element == "Button" ) return BUTTON;
	else if( element == "Check Button" ) return CHECK_BUTTON;
	else if( element == "Dialog Box" ) return DIALOG_BOX;
	else if( element == "Editable Text Box" ) return EDITABLE_TEXT_BOX;
	else if( element == "Image" ) return IMAGE;
	else if( element == "Pointer Mouse" ) return POINTER_MOUSE;
	else if( element == "Progress Bar" ) return PROGRESS_BAR;
	else if( element == "Radio Box" ) return RADIO_BOX;
	else if( element == "Slider" ) return SLIDER;
	else if( element == "Static Text" ) return STATIC_TEXT;
	else if( element == "Text Box" ) return TEXT_BOX;
	else return NONE;
}