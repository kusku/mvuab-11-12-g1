#include "DebugGUIManager.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"
#include "DebugOptions\DebugOptions.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Utils\Timer.h"
#include "Console\Console.h"
#include "Modifiers\ModifierManager.h"
#include "Scripting\ScriptManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CDebugGUIManager::CDebugGUIManager()
	: m_pDebugOptions(NULL)
	, m_pConsole(NULL)
	, m_pDebugRender(NULL)
	, m_pLogRender(NULL)
	, m_pModifierManager(NULL)
	, m_bRenderDebugOptions(false)
	, m_bRenderConsole(false)
	, m_bRenderDebugRender(false)
	, m_bRenderLogger(false)
	, m_bRenderModifiers(false)
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
	CHECKED_DELETE(m_pModifierManager);
}

bool CDebugGUIManager::Init(const std::string &_modifiers_path, const std::string &_debug_options_path )
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

	m_pModifierManager = new CModifierManager();
	m_pModifierManager->Load(_modifiers_path);

	m_pDebugOptions = new CDebugOptions();
	m_pDebugOptions->Load(_debug_options_path);

	return true;
}

void CDebugGUIManager::Update(float _ElapsedTime)
{
	if( !m_pDebugOptions->GetActive() )
		m_pConsole->Update(_ElapsedTime);

	m_pLogRender->Update(_ElapsedTime);
}

void CDebugGUIManager::Render(CRenderManager &_RM, CFontManager &_FM, CTimer *_Timer)
{
	if( m_bRenderConsole )
		m_pConsole->Render(_RM, _FM);

	if( m_bRenderDebugRender )
		m_pDebugRender->Render(&_RM, &_FM, _Timer);

	if( m_bRenderLogger )
		m_pLogRender->Render(&_RM, &_FM);

	if( m_bRenderModifiers )
		m_pModifierManager->Render(_RM, _FM);

	if( m_bRenderDebugOptions )
		m_pDebugOptions->Render(_RM, _FM);
}