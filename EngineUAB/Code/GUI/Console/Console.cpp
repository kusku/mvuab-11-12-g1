#include "Console.h"
#include "Core.h"
#include "Utils/BaseUtils.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"
#include "ScriptManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CConsole::Release()
{
	CHECKED_DELETE(m_Result);
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CConsole::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CConsole::Init()
{
	m_bIsOk = true;

	m_Result = new uint16;
	if (!m_bIsOk)
	{
		Release();
	}
	return m_bIsOk;
}

void CConsole::Render (CRenderManager &renderManager, CFontManager &fm )
{
	uint32 w,h;
	//renderManager->GetWidthAndHeight(w,h);
	Vect2i size = renderManager.GetScreenSize();
	h = size.y;
	w = size.x;

	CColor quad2dColor(0.f,0.f,0.5f,0.7f);
	CColor edgeColor = colBLACK;
	edgeColor.SetAlpha(0.7f);

	if( m_bIsActive )
	{		
		//Renderiza la consola

		renderManager.DrawRectangle2D(Vect2i(5,h-45),w-10, 40,quad2dColor,2,2,edgeColor);

		//Draw Info Text
		std::string total_text = m_sBuffer.substr(0, m_uCursorPos);
		total_text = total_text + m_sFocusObject;
		total_text = total_text + m_sBuffer.substr(m_uCursorPos);
		fm.DrawDefaultText(10, h-25, colWHITE, total_text.c_str());

		//Draw Info text:
		std::string info = "[CONSOLE]              | AutoComplete: Tab | Reverse AutoComplete: LShift+Tab | History: Up | Reverse History: Down | Close: Type \"exit()\"";
		uint32 dy = h-45;
		fm.DrawDefaultText(10, dy, colWHITE, info.c_str());
		std::string header = "______________________________________________________________________________________________________________________________________";
		fm.DrawDefaultText(10, dy, colWHITE, header.c_str());
	}
	else
	{
		//Imprime la información para abrir la consola

		Vect2i l_SizeRectangle;
		Vect2i l_Screen = renderManager.GetScreenSize();

		std::string l_sInfo, l_Action;
		CORE->GetActionToInput()->GetActionInfo("Console", l_Action);
		baseUtils::FormatSrting (l_sInfo, "Press %s to view the Console", l_Action.c_str() );
		l_SizeRectangle.x = fm.SizeX(l_sInfo.c_str());
		l_SizeRectangle.y = fm.SizeY(l_sInfo.c_str());

		uint32 dx = 15;
		renderManager.DrawRectangle2D(Vect2i(dx, h-15), l_SizeRectangle.x, l_SizeRectangle.y, quad2dColor, 2, 2, edgeColor);

		fm.DrawDefaultText(dx, h-15, colWHITE, "Press %s to view the Console", l_Action.c_str());
	}
}

void CConsole::Update(float elapsedTime)
{
if( !m_bIsActive ) return;

	if (m_sBuffer.size() == 0)
	{
		m_sBuffer = ">";
		m_uCursorPos = 1;
	}
  
  /*
  if( CORE->GetActionManager()->IsActionActive("Console") )
	{
    Toggle();
    return;
	}*/

	CInputManager* im = CORE->GetInputManager();
	assert(im);

	if( im->IsUpDown(IDV_KEYBOARD, KEY_LEFT) )
	{
		m_uCursorPos--;
		if (m_uCursorPos < 2) //">_"
		{
			m_uCursorPos = 1;
		}
	}

	if( im->IsUpDown(IDV_KEYBOARD, KEY_RIGHT) )
	{
		m_uCursorPos++;
		if (m_uCursorPos > m_sBuffer.size())
		{
			m_uCursorPos = (uint8)m_sBuffer.size();
		}
	}

	if( im->IsUpDown(IDV_KEYBOARD, KEY_RETURN) )
	{
		FinalizeAutoComplete();
		if (m_sBuffer.compare(">")!=0)
		{
			//LUA con buffer:
			//primero de todo hemos de quitar el ">" del buffer--> ">funcion()"
			std::string action = m_sBuffer.substr(1,m_sBuffer.length());
			CScriptManager* l_pScriptManager = CORE->GetScriptManager();
			assert(l_pScriptManager);
			l_pScriptManager->RunCode(action);
			m_sBuffer = ">";

			//Actualizamos el historial:
			m_History.push_back(action);
			if (m_History.size() > MAX_HISTORY)
			{
				m_History.erase(m_History.begin());
			}
			m_ItHistory = m_History.end() -1;
		}
		m_uCursorPos = (uint8)m_sBuffer.size();
		return;
	}

	

	if( im->IsDown(IDV_KEYBOARD, KEY_BACK) )
	{
		m_BackTime1 += elapsedTime;
		if (m_BackTime1 > 0.7f)
		{
			m_BackTime2 += elapsedTime;
			if (m_BackTime2 > 0.05f)
			{
				FinalizeAutoComplete();
				Remove1character();
				m_BackTime2 = 0.f;
			}
		}		
	}
	else
	{
		m_BackTime1 = 0.f;
		m_BackTime2 = 0.f;
	}

	if (im->IsUpDown(IDV_KEYBOARD, KEY_RSHIFT))
	{
		if (m_uCursorPos < m_sBuffer.size())
		{
			m_sBuffer.erase( m_sBuffer.begin()+m_uCursorPos );
		}
	}

	if( im->IsUpDown(IDV_KEYBOARD, KEY_BACK) )
	{
		FinalizeAutoComplete();
		Remove1character();
	}
	else if( im->IsDown(IDV_KEYBOARD, KEY_LSHIFT) && im->IsUpDown(IDV_KEYBOARD, KEY_TAB) )
	{
		ProcessAutoComplete(true);

	}
	else if (!im->IsDown(IDV_KEYBOARD, KEY_LSHIFT) && im->IsUpDown(IDV_KEYBOARD, KEY_TAB) )
	{
		ProcessAutoComplete(false);
	}
	else
	{ 
		ProcessNewEntries(im);
	}

	

	m_fTimeCount += elapsedTime;
	if( m_fTimeCount > m_fFocusTime )
	{
		if( !m_sFocusObject.compare(" ") )
			m_sFocusObject = "_";
		else
			m_sFocusObject = " ";

		m_fTimeCount = 0.f;
	}

	if( !im->IsDown(IDV_KEYBOARD, KEY_LCTRL) && im->IsUpDown(IDV_KEYBOARD, KEY_UP) && m_History.size() > 0)
	{
		FinalizeAutoComplete();
		//En busca de los comandos ejecutados más recientemente
		std::string buffer = *m_ItHistory;
		m_sBuffer = ">" + buffer;
		m_uCursorPos = (uint8)m_sBuffer.size();

		if (m_ItHistory == m_History.begin())
		{
			m_ItHistory = m_History.end()-1;
		}
		else
		{
			m_ItHistory--;
		}
	}

	if( !im->IsDown(IDV_KEYBOARD, KEY_LCTRL) && im->IsUpDown(IDV_KEYBOARD, KEY_DOWN) && m_History.size() > 0)
	{
		FinalizeAutoComplete();
		//En busca de los comandos ejecutados hace mas tiempo
		m_ItHistory++;
		if (m_ItHistory == m_History.end())
		{
			m_ItHistory = m_History.begin();
		}
		std::string buffer = *m_ItHistory;
		m_sBuffer = ">" + buffer;
		m_uCursorPos = (uint8)m_sBuffer.size();
	}

}


void CConsole::ProcessNewEntries (CInputManager* im)
{
	uint32 index= 237;
	for(uint32 cont = 0; cont<=index; cont++)
	{
		if( im->IsUpDown(IDV_KEYBOARD, cont))
		{
			if( im->Scan2ascii(cont, m_Result))
			{
					FinalizeAutoComplete();

					std::string aux = m_sBuffer.substr(0, m_uCursorPos);
					aux += char(m_Result[0]);
					m_sBuffer = aux + m_sBuffer.substr(m_uCursorPos);
					m_uCursorPos++;
			}
		}
	}	
}

//----Auto Complete----------------------
void CConsole::ProcessAutoComplete (bool reverse)
{
	if (!m_sAutoCInfo.m_bAutoComplete)
	{
		m_sAutoCInfo.m_Functions.clear();
		std::string action = m_sBuffer.substr(1,m_sBuffer.length()) + "autoComplete()";
		CScriptManager* l_pScriptManager = CORE->GetScriptManager();
		assert(l_pScriptManager);
		l_pScriptManager->RunCode(action);
		if (m_sAutoCInfo.m_Functions.size() > 0)
		{
			m_sAutoCInfo.m_sOldBuffer			= m_sBuffer;
			m_sBuffer											= m_sBuffer + m_sAutoCInfo.m_Functions[0];
			m_uCursorPos									= (uint8)m_sBuffer.size();
			m_sAutoCInfo.m_bAutoComplete	= true;
			m_sAutoCInfo.m_uIndex					= 0;
		}
	}
	else
	{
		if (reverse && m_sAutoCInfo.m_Functions.size()>0)
		{
			if (m_sAutoCInfo.m_uIndex == 0)
			{
				m_sAutoCInfo.m_uIndex = uint32(m_sAutoCInfo.m_Functions.size()-1);
			}
			else
			{
				m_sAutoCInfo.m_uIndex--;
			}
		}
		else
		{
			m_sAutoCInfo.m_uIndex++;
			if (m_sAutoCInfo.m_uIndex > m_sAutoCInfo.m_Functions.size()-1)
			{
				m_sAutoCInfo.m_uIndex = 0;
			}
		}
		
		
		std::string action = m_sBuffer.substr(1,m_sBuffer.length());
		m_sBuffer = m_sAutoCInfo.m_sOldBuffer + m_sAutoCInfo.m_Functions[m_sAutoCInfo.m_uIndex];
		m_uCursorPos									= (uint8)m_sBuffer.size();
	}
}

void CConsole::FinalizeAutoComplete()
{
	m_sAutoCInfo.m_bAutoComplete = false;
	m_sAutoCInfo.m_Functions.clear();
	m_sAutoCInfo.m_uIndex = 0;
}


void CConsole::Remove1character()
{
	if (m_uCursorPos > 1)
	{
		m_sBuffer.erase( m_sBuffer.begin()+m_uCursorPos-1 );
		m_uCursorPos--;
	}
}
