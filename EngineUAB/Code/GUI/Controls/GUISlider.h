//----------------------------------------------------------------------------------
// CSlider class
// Author: Enric Vergara
//
// Description:
//A slider is used to set a numerical value in a defined range. Clicking in the body of the slider moves the slider 
// about 10% by default. Clicking in the end-arrows moves it 1%. Dragging the bar changes the slider value.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_SLIDER_H
#define INC_SLIDER_H

#include "GuiElement.h"
#include "Math/Color.h"
#include "GUIButton.h"
#include "Math/MathUtils.h"

//---Forward Declarations---
class CTexture;
//--------------------------

class CGUISlider: public CGuiElement
{

public:
	CGUISlider( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
					const Vect2f position_percent,  float buttonWidthPercent, float buttonHeightPercent,
					float value = 0, std::string lit="", uint32 textHeightOffset=0, uint32 textWidthOffset=0,
					bool isVisible = true, bool isActive = true);

	virtual ~CGUISlider() {/*NOTHING*/;}


	//---------------CGuiElement Interface----------------------
	virtual void	Render						(CRenderManager		*renderManager, CFontManager* fm);
	virtual void	Update						(CInputManager* intputManager, float elapsedTime);
	virtual void	OnClickedChild				(const std::string& name) {/*NOTHING*/;}


	//---------------CSlider Interface---------------------------
	void			SetValue					(float value);
	float			GetValue					() const {return m_fValue;}
	void			SetOnClickedAction			(std::string & inAction );
	void			SetOnOverAction				(std::string & inAction );
	void			SetOnChangeValueAction		(std::string & inAction);
	void			SetButtonTextures			(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated);
	void			SetButtonColors				(const CColor& normal, const CColor& over, const CColor& clicked, const CColor& deactivated, float alpha = 1.f);
	void			SetBackGroundTexture		(CTexture* background);
	void			SetBackGroundColor			(const CColor& background, float alpha = 1.f);
	void			OnChangeValue				();
	void			SetWidthButton				( float width )		{ m_Button.SetWidthPercent(width); }
	void			SetHeightButton				( float height )	{ m_Button.SetHeightPercent(height); }	

	const std::string&	GetOnChangeValue			() const		{ return m_sLuaCode_OnChangeValue; }
	const std::string&	GetOnClickedAction			() const		{ return m_Button.GetOnClicked(); }
	const std::string&	GetOnOverAction				() const		{ return m_Button.GetOnOver(); }
	const CColor&		GetBackgroundColor			() const		{ return m_BackGroundColor; }
	CTexture*			GetBackgroundTexture		() const		{ return m_pBackGroundTexture; }
	CTexture*			GetNormalButtonTexture		() const		{ return m_Button.GetNormalTexture(); }
	CTexture*			GetOverButtonTexture		() const		{ return m_Button.GetOverTexture(); }
	CTexture*			GetClickedButtonTexture		() const		{ return m_Button.GetClickedTexture(); }
	CTexture*			GetDeactivatedButtonTexture	() const		{ return m_Button.GetDeactivatedTexture(); }
	float				GetButtonHeight				() const		{ return m_Button.GetHeightPercent(); }
	float				GetButtonWidth				() const		{ return m_Button.GetWidthPercent(); }

private:
	std::string		m_sLuaCode_OnChangeValue;
	CTexture*		m_pBackGroundTexture;
	CColor			m_BackGroundColor;
	float			m_fValue;									// entre [0-1]
	CGUIButton		m_Button;
	float			m_fButtonHeight;
	float			m_fButtonWidth;
	bool			m_bStart_to_Move;
	uint32			m_fPreviousPosMouseX;
	float			m_fPrevValue;
};

#endif //INC_SLIDER_H