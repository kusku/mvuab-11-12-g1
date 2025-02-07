//----------------------------------------------------------------------------------
// CGuiElement class
// Author: Enric Vergara
//
// Description:
// Clase Base con los datos y funcionalidad com�n a todos los elementos de la GUI.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_GUI_ELEMENT_H
#define INC_GUI_ELEMENT_H

#include <string>
#include <vector>

#include "Math/MathUtils.h"
#include "Math/Matrix44.h"
#include "Assert.h"

//---Forward Declarations---
class CRenderManager;
class CFontManager;
class CInputManager;
//--------------------------

class CGuiElement
{
public:
	typedef enum {	BUTTON = 0, CHECKBUTTON, STATIC_TEXT, SLIDER, IMAGE, EDITABLE_TEXT_BOX,
									PROGRESS_BAR, DIALOG_BOX, RADIO_BOX, ANIMATED_IMAGE} TypeGuiElement;

	typedef enum {  EDITPOS, EDITSIZE, DISABLE } TypeEditMode;

public:
	CGuiElement( uint32 windowsHeight, uint32 windowsWidth, float height_percent, float width_percent, 
						Vect2f position_percent, TypeGuiElement type, std::string lit = "",  uint32 textHeightOffset = 0,
						uint32 textWidthOffset = 0, bool isVisible = true, bool isActive = true );
	
	virtual ~CGuiElement() {/*Nothing*/;}
	
	virtual void			Render					(CRenderManager* render, CFontManager* fm);
	virtual void			RenderText				(CRenderManager* render, CFontManager* fm);
	virtual void			Update					(CInputManager* input, float elapsedTime);
	virtual void			OnClickedChild			(const std::string& name) = 0;
	
	void					RenderGUIEditor			(CRenderManager* render);

	void					OnLoadValue				();
	void					OnSaveValue				();

	void					SetOnLoadValueAction	(const std::string& inAction) {m_sLuaCode_OnLoadValue = inAction;}
	void					SetOnSaveValueAction	(const std::string& inAction) {m_sLuaCode_OnSaveValue = inAction;}


	void					CalculatePosMouse		(const Vect2i& mousePosition);

	bool					IsOver					() const 							{return m_bIsOver;}
	bool					IsInside				() const 							{return m_bIsInside;}
	bool					IsVisible				() const 							{return m_bIsVisible;}
	bool					IsActive				() const 							{return m_bIsActive;}

	void					SetIsSelected			( bool selected )					{ m_bRenderSelected = selected; }
	bool					GetIsSelected			() const							{ return m_bRenderSelected; }

	void					SetVisible				(bool flag) 						{m_bIsVisible = flag;}
	void					SetActive				(bool flag) 						{m_bIsActive = flag;}

	uint32					GetWidth				() const 							{return m_uWidth;}
	float					GetWidthPercent			() const 							{return m_fWidthPercent;}
	uint32					GetHeight				() const 							{return m_uHeight;}
	float					GetHeightPercent		() const 							{return m_fHeightPercent;}

	Vect2i					GetPosition				() const 							{return m_Position;}
	const Vect2f&			GetPositionPercent		() const 							{return m_PositionPercent;}

	void					SetWindowsWidth			(uint32	w);
	void					SetWindowsHeight		(uint32	h);
	void					SetHeightPercent		(float height_percent);
	void					SetWidthPercent			(float width_percent);
	virtual void			SetPositionPercent		(const Vect2f& pos);
	virtual void			SetPosition				(const Vect2i& pos);

	void					SetLiteral				(const std::string& lit)			{m_sLiteral = lit;}
	void					SetTextHeightOffset		(uint32 textHeightOffset)			{m_uTextHeightOffset =textHeightOffset;}
	void					SetTextWidthOffset		(uint32 textWidthOffset)			{m_uTextWidthOffset =textWidthOffset;}
	const std::string&		GetLiteral				()									{return m_sLiteral;}
	uint32					GetTextHeightOffset		()									{return m_uTextHeightOffset;}
	uint32					GetTextWidthOffset		()									{return m_uTextWidthOffset;}

	void					SetRenderForGUIEditor	( bool render )						{ m_bRenderForGUIEditor = render; }
	bool					GetRenderForGUIEditor	() const							{ return m_bRenderForGUIEditor; }

	void					SetParent				(CGuiElement* parent)				 {assert(parent); m_pParent = parent;}
	bool					AddChild				(CGuiElement* child);
	bool					RemoveChild				(CGuiElement* child);


	void					GainFocus				() 									{m_bFocus = true;}
	void					LoseFocus				() 									{m_bFocus = false;}
	bool					HasFocus				() const							{return m_bFocus;}

	void					SetName					(const std::string& name)			{m_sName = name;}
	const std::string&		GetName					() const							{return m_sName;}
	TypeGuiElement			GetType					() const {return m_eType;}

	void					SetID					( const std::string &id )			{ m_sId = id; }
	const std::string&		GetID					() const							{ return m_sId; }

	const std::string&		GetOnLoad				() const							{ return m_sLuaCode_OnLoadValue; }
	const std::string&		GetOnSave				() const							{ return m_sLuaCode_OnSaveValue; }

protected:

	float						m_fWidthPercent;					//% del Ancho del GuiElement respecto el size del Window
	uint32						m_uWidth;									//Ancho en pixeles del GuiElement
	float						m_fHeightPercent;					//% de Altura del GuiElement respecto el size del Window
	uint32						m_uHeight;								//Altura en pixeles del GuiElement
	uint32						m_uWindowsWidth;					//Ancho en pixeles de la windows que contiene a este GuiElement
	uint32						m_uWindowsHeight;					//Altura en pixeles de la windows que contiene a este GuiElement
	Vect2f						m_PositionPercent;				//Posici�n del GuiElement (x,y) en % respecto el size del Window
	Vect2i						m_Position;								//Posici�n del GuiElement (x,y) en pixeles
	bool						m_bIsVisible;							//Indica si el elemento es o no visible
	bool						m_bIsActive;							//Indica si el elemento est� o no activado
	bool						m_bIsOver;								//Indica si el puntero del mouse es la primera vez que ha entrado
	bool						m_bIsInside;							//Indica si el puntero del mouse est� dentro
	bool						m_bFocus;								//Indica que es el guielement que tiene el foco dentro de la windows actual
	bool						m_bRenderSelected;			//Indica si se tiene que renderizar un cuadro de seleccionado para el GUIEditor
	bool						m_bRenderForGUIEditor;		//Indica si se tiene que renderizar la informaci�n para el GUIEDitor
	std::string					m_sId;						//Identificador �nico
	std::string					m_sName;									//Identificador del GuiElement
	std::string					m_sLuaCode_OnLoadValue;		//Codigo LUA que ha de ejecutar cuando se entre por primera vez en la ventana que contiene al GuiElement
	std::string					m_sLuaCode_OnSaveValue;		//Codigo LUA que ha de ejecutar cuando se salga de la ventana que contiene al GuiElement
	std::vector<CGuiElement*>	m_Children;								//GuiElements que estan atachados a este.
	CGuiElement*				m_pParent;								//GuiElements al que esta atachado este.
	TypeEditMode				m_eEditMode;							//Indica si esta en estado de edicion o navegacion.
	TypeGuiElement				m_eType;									//Indica el tipo de GuiElement que sera (button, checkbutton...)
	uint32						m_z;											//Indice de profundidad en la que se pintara el elemento
	std::string					m_sLiteral;
	uint32						m_uTextHeightOffset;
	uint32						m_uTextWidthOffset;
};

#endif //INC_GUI_ELEMENT_H
