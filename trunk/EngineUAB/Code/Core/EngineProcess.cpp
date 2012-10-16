#include "EngineProcess.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------
CEngineProcess::CEngineProcess ( void )
	: m_pCamera ( NULL )
{
}

CEngineProcess::~CEngineProcess ( void )
{
	Done ();
}

// -----------------------------------------
//					METODOS
// -----------------------------------------

void CEngineProcess::Done ( void )
{
	if ( IsOk ( ) )
	{
		Release();
		m_bIsOk = false;
	}
}

void CEngineProcess::Release ( void )
{
}

bool CEngineProcess::Init ( void )
{
	return m_bIsOk;
}

void CEngineProcess::Render( CRenderManager &_RM )
{
}

void CEngineProcess::DebugRender( CRenderManager &_RM )
{
}

void CEngineProcess::Update( float _ElapsedTime )
{
}

void CEngineProcess::Reload( void )
{
}
