#include "Engine.h"
#include "Base.h"
#include <stdio.h>

CEngine::CEngine()
	: m_Core(NULL)
	, m_Process(NULL)
{
}

CEngine::~CEngine()
{
	CHECKED_DELETE(m_Core);
	//CHECKED_DELETE(m_Process);
}

void CEngine::Init()
{
	m_Core = new CCore();
	m_Core->Init();
	m_Process->Init();
}

void CEngine::Update(float ElapsedTime)
{
	m_Core->Update(ElapsedTime);
	m_Process->Update(ElapsedTime);
}

void CEngine::Render()
{
	m_Core->Render();
	m_Process->Render();
}

void CEngine::SetProcess(CProcess *process)
{
	m_Process = process;
}