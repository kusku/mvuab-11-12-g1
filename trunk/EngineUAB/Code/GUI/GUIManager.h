//----------------------------------------------------------------------------------
// CWindows class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el pintado y update de una ventana de la GUI. Esta ventana es simplemente una agrupacion
// de GuiElements que se lee al inicio de la aplicacion de un fichero .xml
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_GUI_MANAGER_H
#define INC_GUI_MANAGER_H

#include <d3dx9.h>
#include <string>
#include <map>
#include "Math/MathUtils.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Controls\TextBox.h"

//---Forward Declarations---
class CPointerMouse;
class CGuiElement;
class CGUIWindow;
class CRenderManager;
class CFontManager;
class CScriptManager;
//--------------------------

//----------Declaracion de nuevos tipos------------------------------------
typedef enum EtypeTransitionEffect 
{
  TE_SHADOW = 0,
  TE_FADE_TO_BLACK,
  TE_SHADOW_OFF
};
struct TransitionEffect
{
	TransitionEffect::TransitionEffect()
		: m_bDoEffect(false)
		, m_fTransitionTime(0.f)
		, m_eType(TE_SHADOW)
		, m_sWindowsName("")
		, m_fTimeCounter(0.f)
		, m_bActiveWindows(false)
	{
	}

	bool							m_bDoEffect;
	float							m_fTransitionTime;
	float							m_fTimeCounter;
	std::string						m_sWindowsName;
	bool							m_bActiveWindows;
	EtypeTransitionEffect			m_eType;

};
//---------------------------------------------------------------------------

class CGUIManager
{
public:
	// Init and End protocols
	CGUIManager(const Vect2i& resolution);
	virtual ~CGUIManager() {Done();}  


	bool				Init						( const std::string& initGuiXML );
	void				Done						();
	bool				IsOk						() const		{ return m_bIsOk; }

	void				Update						( float elapsedTime );
	void				Render						( CRenderManager *renderManager, CFontManager* fontManager );
	void				RenderPointerMouse			( CRenderManager *renderManager, CFontManager* fontManager );

	bool				LoadGuiFiles				( const std::string& pathWindows );
	bool				ReloadGuiFiles				();

	void				SetScreenResolution			( const Vect2i& resolution );
	void				SetMessageBox				( const std::string& text );
	void				ActiveWindows				( const std::string& inNameWindow );
	void				PushWindows					( const std::string& inNameWindow );
	void				PopWindows					();
	void				ActiveWindowsWithEffect		( const std::string& inNameWindow, EtypeTransitionEffect type, float transitionTime );

	bool				IsVisibleMessage			()const			{ assert(m_TextBox); return m_TextBox->IsVisible(); }
    std::string         GetCurrentWindow            ()				{ return m_sCurrentWindows; }
	void				SetVisiblePointerMouse		( bool flag )	{ m_bVisiblePointerMouse = flag; }
		
	//----CScriptRegister interface--------------------------------------------
	//virtual void						RegisterFunctions				(CScriptManager* scriptManager);

	//-------Funciones para modificar los GuiElements--------------------------
    void				SetActiveGuiElement				( const std::string& inNameGuiElement, bool flag);
	bool                GetVisibleGuiElement			( const std::string& inNameGuiElement);
	void				SetVisibleGuiElement			( const std::string& inNameGuiElement, bool flag);

	bool				GetProgressBarValue				( const std::string& inNameGuiElement, float& outValue);
	bool				SetProgressBarValue				( const std::string& inNameGuiElement, float inValue);

	bool				NextBlockInRadioBox				( const std::string& inNameRadioBox);
	bool				PrevBlockInRadioBox				( const std::string& inNameRadioBox);

	std::string			GetButtonCheckInRadioBox		( const std::string& inNameRadioBox);
	void				SetButtonCheckInRadioBox		( const std::string& inNameRadioBox, const std::string& button);

	void				SetStateCheckButton 			( const std::string& inCheckButtonName, bool state);
	bool				GetStateCheckButton 			( const std::string& inCheckButtonName);

	void				SetStateSlider					(const std::string& inSliderName, float amount);
	float				GetStateSlider					(const std::string& inSliderName);

	void				SetLiteralInStaticText			(const std::string& inStaticText, const std::string& lit);
		
	void				SetEditableTextBox				(const std::string& inEditableTextName, const std::string& text);
	std::string			GetEditableTextBox				(const std::string& inEditableTextName);
	
	void				SetImage						(const std::string& inImageName, const std::string& activeImage);
	std::string			GetImage						(const std::string& inImageName);
	void				PlayImage						(const std::string& inImageName, float timePerImage, bool loop);
    void                SetAlphaImage					(const std::string& inImageName, float _Alpha);
    void                FadeOutImage					(const std::string& inImageName, float startTime, float fadePerSecond);

    CGuiElement*        GetGUIElement					(const std::string& inNameGuiElement);
		//-----------------------------------------------------------------------------------------------------------//

private:
	void				Release							();
	void				RenderTransitionEffect			(CRenderManager *renderManager);
	bool				UpdateTransitionEffect			(float elapsedTime);

private:
	Vect2i									m_ScreenResolution;
	bool									m_bIsOk;
	std::map<std::string, CGUIWindow*>		m_WindowsMap;
	std::map<std::string, CGuiElement*>		m_ElementsMap;
	std::string								m_sCurrentWindows;
	std::vector<std::string>				m_PrevWindows;
	bool									m_bVisiblePointerMouse;
	bool									m_bRenderError;
	bool									m_bUpdateError;
	CPointerMouse*							m_PointerMouse;
	CTextBox*								m_TextBox;
	bool									m_bLoadedGuiFiles;
	std::string								m_sLastLoadpathGUI_XML;
	TransitionEffect						m_sTransitionEffect;
	bool									m_bFirstUpdate;
};

#endif //INC_GUI_MANAGER_H
