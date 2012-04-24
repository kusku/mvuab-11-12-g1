//----------------------------------------------------------------------------------
// CPointerMouse class
// Author: Enric Vergara
//
// Description:
// A pointermouse...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_POINTER_MOUSE_H
#define INC_POINTER_MOUSE_H

#include "GuiElement.h"
#include "Math/Color.h"
#include <map>
#include "RenderManager.h"
#include "Math/MathUtils.h"
#include "Textures/Texture.h"
#include "GraphicsDefs.h"

//---Forward Declarations---
class CTexture;
//--------------------------

class CGUIPointerMouse: public CGuiElement
{
private:
	typedef std::map<std::string, CTexture*>	tTexturesMap;

public:
	CGUIPointerMouse(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
									const Vect2f position_percent, bool isVisible = true, bool isActive = true);

	virtual ~CGUIPointerMouse() {/*NOTHING*/;}

	//---------------CGuiElement Interface----------------------
	virtual void		Render						(CRenderManager *renderManager, CFontManager* fm);
	virtual void		Update						(CInputManager* intputManager, float elapsedTime);
	virtual void		OnClickedChild				(const std::string& name) {/*NOTHING*/;}


	//---------------CPointerMouse Interface--------------------
	void				SetTexture					(CTexture* texture, std::string name );
	void				SetActiveTexture			(const std::string& inName)		{m_sActiveTexture = inName;}
	std::string&		GetActiveTexture			()								{return m_sActiveTexture;}
	CTexture*			GetTexture					(const std::string& tex)		{return m_Textures[tex]; }
	void				SetAlignment				(ETypeAlignment alignment)		{m_Alignment=alignment;}
	bool				IsQuadrant					() const						{return m_bIsQuadrant;}
	void				SetQuadrant					(bool flag)						{m_bIsQuadrant = flag;}


private:
	tTexturesMap				m_Textures;
	std::string					m_sActiveTexture;
	bool						m_bAnimated;
	bool						m_bLoop;
	float						m_fTime;
	float						m_fCounter;
	ETypeAlignment				m_Alignment;
	bool						m_bIsQuadrant;
};

#endif //INC_POINTER_MOUSE_H