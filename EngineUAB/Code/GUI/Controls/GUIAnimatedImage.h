//----------------------------------------------------------------------------------
// CSlider class
// Author: Andres
//
// Description:
// Animated Image..
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_ANIMATED_IMAGE_H
#define INC_ANIMATED_IMAGE_H

#include "RenderManager.h"
#include "GuiElement.h"
#include "Math/Color.h"
#include <map>
#include "Math/MathUtils.h"

//---Forward Declarations---
class CTexture;
//--------------------------

enum
{
  GUI_AI_NO_EFFECT = 0,
  GUI_AI_FADE_IN,
  GUI_AI_FADE_OUT
};

class CGUIAnimatedImage: public CGuiElement
{
private:
	typedef std::vector<CTexture*>	tTexturesVec;

public:
	CGUIAnimatedImage( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
					const Vect2f position_percent, std::string lit="", uint32 textHeightOffset=0, uint32 textWidthOffset=0, 
					bool isVisible = true, bool isActive = true);

	virtual ~CGUIAnimatedImage() {/*NOTHING*/;}

	//---------------CGuiElement Interface----------------------
	virtual void	Render							(CRenderManager *renderManager, CFontManager* fm);
	virtual void	Update							(CInputManager* intputManager, float elapsedTime);
	virtual void	OnClickedChild					(const std::string& name) {/*NOTHING*/;}


	//---------------CAnimatedImage Interface---------------------------
	void						AddFrame				(CTexture* texture);
	void						PlayAnimation			(float timePerImage, bool loop=false, bool playForward=true, int16 finalFrame=-1);
	void						SetFlip					(ETypeFlip flip)		{m_eFlip = flip;}
	ETypeFlip					GetFlip					() const				{return m_eFlip;}
	bool						IsQuadrant				() const				{return m_bIsQuadrant;}
	void						SetQuadrant				(bool flag)				{m_bIsQuadrant = flag;}
	void						SetBackGround			(bool flag)				{m_bIsBackGround=flag;}
	int16						NumFrames               ()						{ return m_VecTextures.size(); }
	void						GoToFrame               (int16 frame)			{ m_sCurrentFrame = frame; }
	void						GotoAndStop             (int16 frame);
	void						FadeOut                 (float startTime, float fadePerSecond);
	void						SetAlpha                (float _Alpha)			{ m_fAlpha = _Alpha; }

	void						SetDefaultTexture		( CTexture* texture )	{ m_pDefaultTexture = texture; }
	CTexture*					GetDefaultTexture		() const				{ return m_pDefaultTexture; }
	CTexture*					GetTexture				(uint32 id) const		{ return m_VecTextures[id]; }
	void						DeleteTextures			()						{ m_VecTextures.clear(); }

	bool						GetPlayOnLoad			() const				{ return m_bPlayOnLoad; }
	void						SetPlayOnLoad			( bool play )			{ m_bPlayOnLoad = play; }

private:
	tTexturesVec		m_VecTextures;
	CTexture*			m_pDefaultTexture;
	CColor				m_Color;
	bool				m_bAnimated;
	bool				m_bLoop;
	float				m_fTimePerImage;
	float				m_fCounter;
	ETypeFlip			m_eFlip;
	bool				m_bIsQuadrant;
	bool				m_bIsBackGround;
	bool				m_bPlayForward;
	int16				m_sCurrentFrame;
	int16				m_sFinalFrame;
	bool				m_bPlayOnLoad;

	int					m_iEffect;
	float				m_fAlpha;
	float				m_fFadeCounter;
	float				m_fFadeStartAfter;
	float				m_fFadePerSecond;
};

#endif //INC_IMAGE_H