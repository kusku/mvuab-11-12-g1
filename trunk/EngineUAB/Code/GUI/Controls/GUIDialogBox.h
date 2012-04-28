//----------------------------------------------------------------------------------
// CDialogBox class
// Author: Enric Vergara
//
// Description:
// Dialog boxes may be defined to help the user navigate complicated choices or to give the user information. The second
// example below implements several dialog boxes. Possible dialog boxes include:
//	* error
//	* help
//	* input
//	* question
//	* file read/write
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_DIALOG_BOX_H
#define INC_DIALOG_BOX_H


#include "GuiElement.h"
#include "GUIButton.h"
#include "Math/Color.h"
#include "Math/MathUtils.h"

//---Forward Declarations---
class CTexture;
//--------------------------
class CGUIDialogBox: public CGuiElement
{

public:
	CGUIDialogBox(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
							const Vect2f position_percent,  float buttonWidthPercent, float buttonHeightPercent,
							std::string lit="", uint32 textHeightOffset=0, uint32 textWidthOffset=0, 
							bool isVisible = true, bool isActive = true);
	
	virtual ~CGUIDialogBox() {/*NOTHING*/;}

	//---------------CGuiElement Interface----------------------
	virtual void	Render						(CRenderManager *renderManager, CFontManager* fm);
	virtual void	Update						(CInputManager* intputManager, float elapsedTime);
	virtual void	OnClickedChild				(const std::string& name);
	virtual void	SetPosition					(const Vect2i& pos);
	virtual void	SetPositionPercent			(const Vect2f& pos);

	//---------------CDialogBox Interface----------------------
	void			SetCloseButtonTextures		(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated);
	void			SetMoveButtonTextures		(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated);
	void			SetDialogTexture			(CTexture* background);	
	void			SetCloseButtonColors		(const CColor& normal, const CColor& over, const CColor& clicked, const CColor& deactivated);
	void			SetMoveButtonColors			(const CColor& normal, const CColor& over, const CColor& clicked, const CColor& deactivated);
	void			SetDialogColors				(const CColor& background);	

	CTexture*		GetNormalButtonMove			() const		{ return m_ButtonMove.GetNormalTexture(); }
	CTexture*		GetOverButtonMove			() const		{ return m_ButtonMove.GetOverTexture(); }
	CTexture*		GetClickedButtonMove		() const		{ return m_ButtonMove.GetClickedTexture(); }
	CTexture*		GetDeactivatedButtonMove	() const		{ return m_ButtonMove.GetDeactivatedTexture(); }

	CTexture*		GetNormalButtonClose		() const		{ return m_ButtonClose.GetNormalTexture(); }
	CTexture*		GetOverButtonClose			() const		{ return m_ButtonClose.GetOverTexture(); }
	CTexture*		GetClickedButtonClose		() const		{ return m_ButtonClose.GetClickedTexture(); }
	CTexture*		GetDeactivatedButtonClose	() const		{ return m_ButtonClose.GetDeactivatedTexture(); }

	CTexture*		GetBackgroundTexture		() const		{ return m_pBackGroundTexture; }

	float			GetButtonHeight				() const		{ return m_fButtonHeight; }
	float			GetButtonWidth				() const			{ return m_fButtonWidth; }

private:
	bool			m_bDialogClicked;
	CGUIButton		m_ButtonClose;
	CGUIButton		m_ButtonMove;
	CTexture*		m_pBackGroundTexture;
	CColor			m_BackGroundColor;
	bool			m_bStart_to_Move;
	Vect2i			m_PreviousPosMouse;
	
protected:
	float			m_fButtonHeight;	// Es el % del mHeight
	float			m_fButtonWidth;		// Es el % del mWidth;

};

#endif //INC_DIALOG_BOX_H