#include "stdafx.h"
#include "ParticleEditorProcess.h"
#include "Particles\ParticleEmitterManager.h"
#include "Particles\ParticleSettingsManager.h"
#include "Particles\ParticleSystemManager.h"
#include "Particles\ParticleDef.h"
#include "Properties\PropertiesSaver.h"
#include "RenderManager.h"
#include "Base.h"
#include "Core.h"
#include "Cameras\ThPSCamera.h"
#include "Cameras\Camera.h"
#include "HWNDManager.h"
#include "Utils\Active.h"
#include "defines.h"

CParticleEditorProcess::CParticleEditorProcess()
	: m_pThPSCamera(NULL)
	, m_bIsInitializeParticles(true)
	, m_CurrentParticleSettings("")
	, m_PreviousTextureFilename("")
{	
}


CParticleEditorProcess::~CParticleEditorProcess()
{
	Release();
}

// -----------------------------------------
//				MÈTODES 
// -----------------------------------------
void CParticleEditorProcess::Done ()
{
	if ( IsOk ( ) )
	{
		Release();
		m_bIsOk = false;
	}
}

void CParticleEditorProcess::Release ()
{
}

bool CParticleEditorProcess::Init( void )
{
	Vect2i pos;
	Vect2i screen	= CORE->GetRenderManager()->GetScreenSize();
	pos.x			= screen.x / 2;
	pos.y			= screen.y / 2;

	//Establece la cámara
	m_Player.SetPosition(Vect3f(0.f, 0.f, 0.f));
	m_Player.SetPitch(0.0f);
	m_Player.SetYaw(0.0f);
	m_Player.SetRoll(0.0f);

	float aspect	= CORE->GetRenderManager()->GetAspectRatio();
	m_pThPSCamera	= new CThPSCamera(1.0f, 10000.f, 45.f * D3DX_PI / 180.f, aspect, &m_Player, 10.0f, 0.f, 0.f, "Player");
	m_pCamera		= static_cast<CCamera*>(m_pThPSCamera);
	CORE->SetCamera(m_pCamera);

	m_bIsInitializeParticles = true;

	m_CurrentParticleSettings = "System_0";

	return true;
}

void CParticleEditorProcess::Update(float elapsedTime)
{
	if(m_bIsInitializeParticles)
	{
		LoadSystemsOnEditor();

		//Set off all the particle emitters
		CParticleEditorProcess::DisableAllEmitters();
		m_bIsInitializeParticles = false;

		TParticleSystemSettings *l_pSettings = CORE->GetParticleSettingsManager()->GetResource( m_CurrentParticleSettings );
		m_PreviousTextureFilename = l_pSettings->m_TextureName;
	}

	//Actualiza los valores de propiedades
	TParticleSystemSettings *l_pSettings = CORE->GetParticleSettingsManager()->GetResource( m_CurrentParticleSettings );
	assert(l_pSettings);

	CPropertiesSaver::GetInstance()->SaveProperties(l_pSettings);
	if( m_PreviousTextureFilename != l_pSettings->m_TextureName )
	{
		CORE->GetParticleSystemManager()->GetResource(l_pSettings->m_Name)->RefreshTexture();
	}

	SetSettingsToEmitters();

	//Actualiza la cámara
	m_Player.Update(elapsedTime, m_pThPSCamera);

	//Envia un mensaje para actualizar el Logger
	HWND hWnd = CHWNDManager::GetInstance()->GetHWNDOutput();
	PostMessage( hWnd, WM_GETLOGGER, 0, 0);
}


void CParticleEditorProcess::Render( CRenderManager &_RM )
{
}

void CParticleEditorProcess::DisableAllEmitters()
{
	CORE->GetParticleEmitterManager()->GetResource("point_emitter_editor")->SetActive(false);
	CORE->GetParticleEmitterManager()->GetResource("line_emitter_editor")->SetActive(false);
	CORE->GetParticleEmitterManager()->GetResource("ring_emitter_editor")->SetActive(false);
	CORE->GetParticleEmitterManager()->GetResource("box_emitter_editor")->SetActive(false);
	CORE->GetParticleEmitterManager()->GetResource("sphere_emitter_editor")->SetActive(false);
}

void CParticleEditorProcess::SetSettingsToEmitters()
{

	CParticleSystem *l_pSystem = CORE->GetParticleSystemManager()->GetResource(m_CurrentParticleSettings);
	assert(l_pSystem);

	CORE->GetParticleEmitterManager()->GetResource("point_emitter_editor")->SetParticleSystem(l_pSystem);
	CORE->GetParticleEmitterManager()->GetResource("line_emitter_editor")->SetParticleSystem(l_pSystem);
	CORE->GetParticleEmitterManager()->GetResource("ring_emitter_editor")->SetParticleSystem(l_pSystem);
	CORE->GetParticleEmitterManager()->GetResource("box_emitter_editor")->SetParticleSystem(l_pSystem);
	CORE->GetParticleEmitterManager()->GetResource("sphere_emitter_editor")->SetParticleSystem(l_pSystem);
}

void CParticleEditorProcess::SetParticleSettings( const std::string &settings )
{
	//Guarda los datos del sistema
	TParticleSystemSettings *l_pSettings = CORE->GetParticleSettingsManager()->GetResource( m_CurrentParticleSettings );
	assert(l_pSettings);
	CPropertiesSaver::GetInstance()->SaveProperties(l_pSettings);

	//Cambia el sistema
	m_CurrentParticleSettings = settings;

	//Carga los datos del sistema
	l_pSettings = CORE->GetParticleSettingsManager()->GetResource( m_CurrentParticleSettings );
	assert(l_pSettings);
	CPropertiesSaver::GetInstance()->LoadProperties(l_pSettings);

	//Establece el sistema a los emisores
	SetSettingsToEmitters();
}

void CParticleEditorProcess::LoadSystemsOnEditor()
{
	CParticleSettingsManager *l_pStettingsManager = CORE->GetParticleSettingsManager();
	uint16 size = l_pStettingsManager->GetResourcesVector().size();

	for(uint16 i=0; i<size; ++i)
	{
		std::string l_Name = l_pStettingsManager->GetResourceById(i)->m_Name;
		CString *name = new CString(l_Name.c_str());
		PostMessage(CHWNDManager::GetInstance()->GetHWNDFiles(), WM_ADD_NEW_SYSTEM, (WPARAM)name, 0); 
	}

	std::string l_Name = l_pStettingsManager->GetResourceById(0)->m_Name;
	CString *name = new CString(l_Name.c_str());
	PostMessage(CHWNDManager::GetInstance()->GetHWNDFiles(), WM_SET_CURRENT_SETTING, (WPARAM)name, 0); 

	CPropertiesSaver::GetInstance()->LoadProperties( l_pStettingsManager->GetResourceById(0) );

	m_CurrentParticleSettings = l_Name;

	//Establece el sistema a los emisores
	SetSettingsToEmitters();
}