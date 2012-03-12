//----------------------------------------------------------------------------------
// CConsole class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona internamente una especie de EditBox exclusiva para ejecutat scripts.
// Hace uso del ScriptManager para enviarle el script que se ha introducido.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_CONSOLE_H
#define INC_CONSOLE_H

#include "Base.h"
#include <string>
#include<vector>
//---Forward Declarations----
class CRenderManager;
class CFontManager;
class CInputManager;
//---------------------------

#define MAX_HISTORY 20


//----Declaración de nuevos tipos-----
struct  SAutoCompleteInfo
{
	SAutoCompleteInfo::SAutoCompleteInfo(): m_sOldBuffer(""), m_uIndex(0), m_bAutoComplete(false) {}

	std::string					m_sOldBuffer;
	uint32						m_uIndex;
	bool						m_bAutoComplete;
	std::vector<std::string>	m_Functions;

};
//------------------------------------

class CConsole 
{
public:
	//---Init and End protocols----
	CConsole(): m_sBuffer( ">" ), 
							m_sFocusObject("_"),		m_bIsActive( false ),
							m_fTimeCount(0.f),			m_fFocusTime(0.5f),
							m_Result(NULL),				m_BackTime1(0.f),
							m_BackTime2(0.f),			m_uCursorPos(1) {}

	virtual ~CConsole() { Done(); }  

	bool	Init		  					();
	void	Done							();
	bool	IsOk      						() const	{ return m_bIsOk; }
	//-----------------------------

	void	Render						(CRenderManager &renderManager, CFontManager &fontManager);
	void	Update						(float elapsedTime);

	void	SetActive					(bool active)										{ m_bIsActive = active; }
	bool	IsActive					()						const 						{ return m_bIsActive; }
	void	SetAutoComplete				(std::vector<std::string>	functions)				{ m_sAutoCInfo.m_Functions = functions; }
	void	Remove1character			();
	void	Toggle						()													{ m_bIsActive = !m_bIsActive; };
	
	
private:
	void	Release					();
	void	ProcessNewEntries		(CInputManager*input);
	void	ProcessAutoComplete		(bool reverse = false);
	void	FinalizeAutoComplete	();

private:
	//Types
	typedef std::vector<std::string> THistory;

	bool									m_bIsOk;
	std::string								m_sBuffer;	
	std::string								m_sFocusObject;
	bool									m_bIsActive;
	float									m_fTimeCount;
	float									m_fFocusTime;
	uint16*									m_Result;
	THistory								m_History;
	THistory::iterator						m_ItHistory;
	SAutoCompleteInfo						m_sAutoCInfo;

	float									m_BackTime1;
	float									m_BackTime2;
	uint8									m_uCursorPos;
};

#endif // INC_CONSOLE_H