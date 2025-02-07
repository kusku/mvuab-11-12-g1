
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "GUIEditor.h"
#include "HWNDManager.h"
#include "defines.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "Core.h"
#include "Base.h"
#include <sstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
	: m_TreeSelected(NULL)
	, m_WindowSelected("")
{
}

CFileView::~CFileView()
{
	Release();
}

void CFileView::Release()
{
	m_WindowsMap.clear();

	TElementsWindow::iterator l_It = m_ElementsWindowMap.begin();
	TElementsWindow::iterator l_ItEnd = m_ElementsWindowMap.end();
	for( ; l_It != l_ItEnd; ++l_It)
	{
		(*l_It).second.clear();
	}

	m_ElementsWindowMap.clear();
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Controladores de mensajes de CWorkspaceBar

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Crear vista:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("No se pudo crear la vista de archivos\n");
		return -1;      // No se pudo crear
	}

	// Cargar im�genes de vista:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Bloqueado */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// Todos los comandos se enrutar�n a trav�s de este control, no del marco primario:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Rellenar con algunos datos de vista de �rbol est�tica (c�digo ficticio, nada especial)
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();

	CHWNDManager::GetInstance()->SetHWNDFiles( m_hWnd );
}

void CFileView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("Windows"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_WindowsMap[ "Windows" ] = hRoot;

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("Main"), 0, 0, hRoot);
	m_WindowsMap[ "Main" ] = hSrc;

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
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

			//Mira si existe el elemento seleccionado en el mapa como elemento de GUI
			m_TreeSelected = NULL;
			m_WindowSelected = "";
			TElementsWindow::iterator l_ItWindow = m_ElementsWindowMap.begin();
			TElementsWindow::iterator l_ItEndWindow = m_ElementsWindowMap.end();
			for(; l_ItWindow != l_ItEndWindow; ++l_ItWindow)
			{
				std::map<std::string, HTREEITEM>::iterator l_ItElem = l_ItWindow->second.begin();
				std::map<std::string, HTREEITEM>::iterator l_ItEndElem = l_ItWindow->second.end();
				for(; l_ItElem != l_ItEndElem; ++l_ItElem)
				{
					if( l_ItElem->second == hTreeItem )
					{
						m_WindowSelected = l_ItWindow->first;
						m_TreeSelected = hTreeItem;
						break;
					}
				}

				if( m_TreeSelected != NULL )
				{
					break;
				}
			}

		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("Propiedades..."));

}

void CFileView::OnFileNew()
{
	HTREEITEM hRoot = m_WindowsMap["Windows"];
	
	std::stringstream os;
	os << m_WindowsMap.size();
	std::string name = "Window_" + os.str();
		
	HTREEITEM hSrc = m_wndFileView.InsertItem(_T(name.c_str()), 0, 0, hRoot);
	m_WindowsMap[ name ] = hSrc;

	m_wndFileView.Expand(hSrc, TVE_EXPAND);

	std::string window_name = name;
	CORE->GetGUIManager()->AddWindows( window_name );
}

void CFileView::OnFileOpenWith()
{
	// TODO: Agregue aqu� el c�digo del controlador de comando
}

void CFileView::OnDummyCompile()
{
	// TODO: Agregue aqu� el c�digo del controlador de comando
}

void CFileView::OnEditClear()
{
	if( m_TreeSelected != NULL )
	{
		CString text = m_wndFileView.GetItemText( m_TreeSelected );
		m_wndFileView.DeleteItem( m_TreeSelected );

		std::string name_element = (LPCTSTR)text;
		CORE->GetGUIManager()->GetWindow( m_WindowSelected )->ReleaseGuiElement(name_element);

		m_ElementsWindowMap[m_WindowSelected].erase(name_element);

		m_TreeSelected = NULL;
		m_WindowSelected = "";
	}
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // contexto de dispositivo para dibujar

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Bloqueado */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

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

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::ReloadAll()
{
	m_wndFileView.DeleteAllItems();
	CElementManager::GetInstance()->SetWindowToAdd("");
	CElementManager::GetInstance()->SetElementToAdd(NONE);
	Release();

	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("Windows"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	m_WindowsMap[ "Windows" ] = hRoot;

	CGUIManager *l_pManager = CORE->GetGUIManager();
	CGUIManager::TWindowsMap l_Windows = l_pManager->GetAllWindows();

	CGUIManager::TWindowsMap::iterator l_It = l_Windows.begin();
	CGUIManager::TWindowsMap::iterator l_ItEnd = l_Windows.end(); 

	for(; l_It != l_ItEnd; ++l_It)
	{
		std::string window_name = l_It->first;
		HTREEITEM hSrc = m_wndFileView.InsertItem(_T(window_name.c_str()), 0, 0, hRoot);
		m_WindowsMap[ window_name ] = hSrc;

		uint16 count = l_It->second->GetNumElements();
		for(uint16 j=0; j<count; ++j)
		{
			CGuiElement *l_pElement = l_It->second->GetElementById(j);
			l_pElement->SetRenderForGUIEditor(true);
			std::string element_name = l_pElement->GetName();

			HTREEITEM l_Child = m_wndFileView.InsertItem(_T( element_name.c_str() ), 1, 1, hSrc);

			m_ElementsWindowMap[window_name][element_name] = l_Child;
		}

		m_wndFileView.Expand(hSrc, TVE_EXPAND);
	}

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
}

BOOL CFileView::PreTranslateMessage(MSG* pMsg)
{
	switch( pMsg->message )
	{
	case WM_ADD_ELEMENT_FILE:	//A�ade el elemento a la lista
		{
			CGUIManager *l_pGUIManager = CORE->GetGUIManager();

			std::string l_WindowName = l_pGUIManager->GetCurrentWindow();

			//Split window name
			size_t pos = l_WindowName.rfind(".");
			l_WindowName = l_WindowName.substr(0, pos);

			HTREEITEM l_Tree = m_WindowsMap[ l_WindowName ];

			std::string l_szName = (char*)pMsg->wParam;

			HTREEITEM l_Child = m_wndFileView.InsertItem(_T( l_szName.c_str() ), 1, 1, l_Tree);
			m_wndFileView.Expand(l_Tree, TVE_EXPAND);

			m_ElementsWindowMap[ l_WindowName][ l_szName ] = l_Child;

			break;
		}
	case WM_UPDATE_FILE_DATA:
		{
			CString* name = (CString*)pMsg->wParam;
			char nameChar[40];
			strcpy_s(nameChar, *name);
			std::string l_szElement = nameChar;

			CString *newname = (CString*)pMsg->lParam;
			char newName[40];
			strcpy_s(newName,*newname );
			std::string l_szNewName = newName;

			delete name;
			delete newname;

			std::string l_szWindow = CORE->GetGUIManager()->GetCurrentWindow();

			size_t pos = l_szWindow.rfind(".");
			l_szWindow = l_szWindow.substr(0, pos);

			HTREEITEM l_Item = m_ElementsWindowMap[l_szWindow][l_szElement];
			if( l_Item != NULL )
			{
				m_wndFileView.SetItemText( l_Item, l_szNewName.c_str() );
			}

			break;
		}
	case WM_ADD_WINDOW:
		{
			OnFileNew();
			break;
		}
	case WM_LBUTTON_DOWN:
		{
			HTREEITEM item = (HTREEITEM)pMsg->wParam;

			std::map<std::string, HTREEITEM>::iterator l_It = m_WindowsMap.begin();
			std::map<std::string, HTREEITEM>::iterator l_ItEnd = m_WindowsMap.end();
			while(l_It != l_ItEnd)
			{
				if( l_It->second == item )
				{
					CElementManager::GetInstance()->SetWindowToAdd( l_It->first );
					break;
				}
				++l_It;
			}
			break;
		}
	case WM_CHANGE_WINDOW_NAME:
		{
			CString* name = (CString*)pMsg->wParam;
			char nameChar[40];
			strcpy_s(nameChar, *name);
			std::string current_name = nameChar;

			CString *newname = (CString*)pMsg->lParam;
			strcpy_s(nameChar, *newname);
			std::string new_name = nameChar;

			if( m_WindowsMap.find(current_name) != m_WindowsMap.end() )
			{
				HTREEITEM item  = m_WindowsMap[current_name];
				if( item != NULL )
				{
					m_wndFileView.SetItemText( item, new_name.c_str() );

					m_WindowsMap.erase(current_name);
					m_WindowsMap[new_name] = item;
				}

				std::map<std::string, HTREEITEM> l_Elements = m_ElementsWindowMap[current_name];
				m_ElementsWindowMap.erase(current_name);
				m_ElementsWindowMap[new_name] = l_Elements;
			}
			

			CHECKED_DELETE(name);
			CHECKED_DELETE(newname);
			break;
		}
	case WM_RELOAD_FILE_LIST:
		{
			ReloadAll();
			break;
		}
	}

	return CDockablePane::PreTranslateMessage(pMsg);
}
