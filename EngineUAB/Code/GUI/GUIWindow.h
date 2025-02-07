//----------------------------------------------------------------------------------
// CGUIWindow class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el pintado y update de una ventana de la GUI. Esta ventana es simplemente una agrupacion
// de GuiElements que se lee al inicio de la aplicacion de un fichero .xml
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_GUI_WINDOW_H
#define INC_GUI_WINDOW_H

#include "GuiElement.h"
#include <string>
#include <vector>
#include <map>
#include "Math/MathUtils.h"


//---Forward Declarations---
class CTexture;
class CGUIButton;
class CGUICheckButton;
class CXMLTreeNode;
class CTextureManager;
class CGUISlider;
class CGUIDialogBox;
class CGUIEditableTextBox;
class CGUIRadioBox;
class CGUIImage;
class CGUIAnimatedImage;
class CGUIProgressBar;
class CGUIStaticText;
//--------------------------

class CGUIWindow
{

public:
	CGUIWindow(): m_sLuaCode_OnKeyDown (""), m_uInputKeyDown(0), m_sLuaCode_OnUpdateWindows(""), m_sWindowsName(""),
							m_sLuaCode_OnLoadWindows(""), m_sLuaCode_OnSaveWindows(""), m_pCurrentSelectedElement(NULL) {}
	virtual ~CGUIWindow();

	void	Render						(CRenderManager *renderManager, CFontManager* fm);
	void	Update						(CInputManager* intputManager, float elapsedTime);
	bool	AddGuiElement				(CGuiElement * inGuiElement);
	bool	ReleaseGuiElement			(const std::string & name);
	void	SetName						(const std::string& name) {m_sWindowsName = name;}

	const std::string& GetName			() const		{ return m_sWindowsName; }

	//--- PARSERFILE FUNCTION ---
	bool	LoadXML						(const std::string &xmlGuiFile, const Vect2i& screenResolution);
	void	RegisterElements			(std::map<std::string,CGuiElement*>& elements);
	void	LoadWindows					();
	void	SaveWindows					();

	CGuiElement*	GetElement			( const std::string &name );
	CGuiElement*	GetElementById		( uint32 id ) const	{ return m_GuiElementsVector[id]; }
	uint32			GetNumElements		() const	{ return m_GuiElementsVector.size(); }

	CGuiElement*	GetCurrentSelectedElement () const		{ return m_pCurrentSelectedElement; }

	const std::string&	GetOnLoadWindows	() const		{ return m_sLuaCode_OnLoadWindows; }
	const std::string&	GetOnSaveWindows	() const		{ return m_sLuaCode_OnSaveWindows; }
	const std::string&	GetOnUpdateWindows	() const		{ return m_sLuaCode_OnUpdateWindows; }

	void				SetOnLoadWindows	( const std::string &lua )		{ m_sLuaCode_OnLoadWindows = lua; }
	void				SetOnSaveWindows	( const std::string &lua )		{ m_sLuaCode_OnSaveWindows = lua; }
	void				SetOnUpdateWindows	( const std::string &lua )		{ m_sLuaCode_OnUpdateWindows = lua; }

	void			MoveElementToFront		(CGuiElement* element);
	void			MoveElementForward		(CGuiElement* element);
	void			MoveElementBack			(CGuiElement* element);
	void			MoveElementToBack		(CGuiElement* element);

	void			ClearSelectElements		();

private:
	void	IsKeyDown					(CInputManager* intputManager);	

	void	LoadButton					(CGUIButton** button, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	LoadCheckButton				(CGUICheckButton** checkButton, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	LoadSlider					(CGUISlider** slider, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	LoadDialogBox				(CGUIDialogBox** dialogBox, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	LoadEditableTextBox			(CGUIEditableTextBox** editableTextBox, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	LoadRadioBox				(CGUIRadioBox** radioBox, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	_LoadImage					(CGUIImage** image, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	LoadAnimatedImage			(CGUIAnimatedImage** image_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	LoadProgressBar				(CGUIProgressBar** progressBar, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);
	void	LoadStaticText				(CGUIStaticText** staticText, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm);

private:
	std::vector<CGuiElement*>			m_GuiElementsVector;
	std::string							m_sLuaCode_OnSaveWindows;
	std::string							m_sLuaCode_OnLoadWindows;
	std::string							m_sLuaCode_OnKeyDown;
	std::string							m_sLuaCode_OnUpdateWindows;
	uint32								m_uInputKeyDown;
	std::string							m_sWindowsName;
	CGuiElement*						m_pCurrentSelectedElement;
};

#endif //INC_GUI_WINDOW_H