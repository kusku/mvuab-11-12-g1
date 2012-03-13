#include "DebugGUIManager.h"
#include "RenderManager.h"
#include "Fonts\FontManager.h"
#include "DebugOptions\DebugOptions.h"
#include "DebugInfo\DebugRender.h"
#include "LogRender\LogRender.h"
#include "Utils\Timer.h"
#include "Console\Console.h"
#include "Modifiers\ModifierManager.h"
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
	m_pConsole->Render(_RM, _FM);
	m_pDebugRender->Render(&_RM, &_FM, _Timer);
	m_pLogRender->Render(&_RM, &_FM);
	m_pModifierManager->Render(_RM, _FM);
	m_pDebugOptions->Render(_RM, _FM);
}