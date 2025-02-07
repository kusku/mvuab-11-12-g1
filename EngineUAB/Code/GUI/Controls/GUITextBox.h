//----------------------------------------------------------------------------------
// CTextBox class
// Author: Enric Vergara
//
// Description:
// A TextBox...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_TEXT_BOX_H
#define INC_TEXT_BOX_H

#include "GUIDialogBox.h"
#include "Math/Color.h"
#include <string>
#include "Math/MathUtils.h"

//---Forward Declarations---
class CRenderManager;
//--------------------------


class CGUITextBox: public CGUIDialogBox
{
public:
	CGUITextBox( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
					const Vect2f position_percent,  float buttonWidthPercent, float buttonHeightPercent,
					std::string lit="", uint32 textHeightOffset=0, uint32 textWidthOffset=0, 
					bool isVisible = true, bool isActive = true);
	
	virtual ~CGUITextBox() {/*NOTHING*/;}

	//---------------CDialogBox Interface----------------------
	virtual void	Render				(CRenderManager *renderManager, CFontManager* fm);
			
	//---------------CTextBox Interface---------------------------
	void			SetMessage			(const std::string& message) {m_sMessage = message;}
	void			SetFont				(CColor textColor = colBLACK, uint32 fontID = 0);

private:
	std::string		m_sMessage;	
	uint32			m_uFontID;
	CColor			m_TextColor;
};

#endif //INC_TEXT_BOX_H