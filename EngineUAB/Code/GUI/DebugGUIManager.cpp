#include "DebugGUIManager.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"
#include "DebugOptions\DebugOptions.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Utils\Timer.h"
#include "Console\Console.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"
#include <string>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDebugGUIManager::CDebugGUIManager()
	: m_pDebugOptions(NULL)
	, m_pConsole(NULL)
	, m_pDebugRender(NULL)
	, m_pLogRender(NULL)
{
}


CDebugGUIManager::~CDebugGUIManager()
{
	Destroy();
}

void CDebugGUIManager::Destroy()
{
	CHECKED_DELETE(m_pDebugOptions);
	CHECKED_DELETE(m_pConsole);
	CHECKED_DELETE(m_pDebugRender);
	CHECKED_DELETE(m_pLogRender);
}

bool CDebugGUIManager::Init(/*CXMLTreeNode &_Node*/)
{
	bool l_bIsOk = false;

	//Init Elements without XML Info
	m_pConsole = new CConsole();
	l_bIsOk = m_pConsole->Init();
	if( !l_bIsOk ) return false; //if Init fails, exits the method
	
	m_pDebugRender = new CDebugRender();
	m_pDebugRender->Init();

	m_pLogRender = new CLogRender();
	m_pLogRender->SetLinePerPage(20);

	/*uint8 l_uCount = _Node.GetNumChildren();
	for(uint8 i=0; i < l_uCount; ++i)
	{
		std::string l_Type = _Node(i).GetName();
		if( l_Type == "Debug_Options" )
		{
			m_pDebugOptions = new CDebugOptions();
			m_pDebugOptions->Load( _Node(i).GetPszProperty("debug_options_xml", "") );
		}
	}*/
	
	return true;
}

void CDebugGUIManager::Update(float _ElapsedTime)
{
	m_pConsole->Update(_ElapsedTime);
	m_pLogRender->Update(_ElapsedTime);
}

void CDebugGUIManager::Render(CRenderManager &_RM, CFontManager &_FM, CTimer *_Timer)
{
	m_pConsole->Render(_RM, _FM);
	m_pDebugRender->Render(&_RM, &_FM, _Timer);
	m_pLogRender->Render(&_RM, &_FM);
	//m_pDebugOptions->Render(_RM, _FM);
}