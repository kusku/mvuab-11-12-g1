//----------------------------------------------------------------------------------
// CProgressBar class
// Author: Enric Vergara
//
// Description:
// A progress bar is a component in a graphical user interface used to convey the progress of a 
// task, such as a download or file transfer.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PROGRESS_BAR_H
#define INC_PROGRESS_BAR_H

#include "GuiElement.h"
#include "Math/Color.h"
#include "Math/MathUtils.h"


//---Forward Declarations---
class CTexture;
//--------------------------

class CGUIProgressBar: public CGuiElement
{

public:
	CGUIProgressBar( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
						const Vect2f position_percent, std::string lit="", uint32 textHeightOffset=0, uint32 textWidthOffset=0, 
						bool isVisible = true, bool isActive = true);

	virtual ~CGUIProgressBar(){/*NOTHING*/;}

	//---------------CGuiElement Interface----------------------
	virtual void		Render						(CRenderManager *renderManager, CFontManager* fm);
	virtual void		Update						(CInputManager* intputManager, float elapsedTime);
	virtual void		OnClickedChild				(const std::string& name) {/*NOTHING*/;}


	//---------------CProgressBar Interface----------------------
	void				SetTextures					(CTexture* background, CTexture* progress);
	void				SetColors					(const CColor& background, const CColor& progress);
	void				SetFont						(uint32 fontID = 0, const CColor& textColor = colBLACK);
	void				SetProgress					(float progress);
	float				GetProgress					() const							{return m_fProgress;}
	void				OnComplete					();
	void				SetOnComplete				(const std::string& inAction)		{m_sLuaCode_OnComplete = inAction;}

	uint32				GetFontID					() const				{ return m_uFontID; }
	const CColor&		GetBackGroundColor			() const				{ return m_BackGroundColor; }
	const CColor&		GetProgressColor			() const				{ return m_ProgressColor; }
	const CColor&		GetTextColor				() const				{ return m_TextColor; }
	CTexture*			GetBackgroundTexture		() const				{ return m_pBackgroundTexture; }
	CTexture*			GetProgressTexture			() const				{ return m_pProgressTexture; }
	const std::string&	GetOnComplete				() const				{ return m_sLuaCode_OnComplete; }

private:
	CTexture*			m_pBackgroundTexture;
	CTexture*			m_pProgressTexture;
	CColor				m_BackGroundColor;
	CColor				m_ProgressColor;
	float				m_fProgress;
	uint32				m_uFontID;
	CColor				m_TextColor;
	std::string			m_sText;
	float				m_fCountTime;
	float				m_fTimeToUpdate;
	std::string			m_sLuaCode_OnComplete;
};

#endif //INC_PROGRESS_BAR_H