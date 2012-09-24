#pragma once

#ifndef _CORE_H
#define _CORE_H

#include <Windows.h>
#include "Utils\Singleton.h"
#include "CoreDefs.h"

//---Forward Declarations---
class CInputManager;
class CFontManager;
class CLanguageManager;
class CActionToInput;
class CRenderManager;
class CTextureManager;
class CStaticMeshManager;
class CRenderableObjectsManager;
class CAnimatedModelManager;
class CLightManager;
class CEffectManager;
class CRenderableObjectTechniqueManager;
class CSceneRendererCommandManager;
class CRenderableObjectsLayersManager;
class CBillboardManager;
class CCamera;
class CTimer;
class CScriptManager;
class CPhysicsManager;
class CEngineProcess;
class CStadistics;
class CDebugGUIManager;
class CGUIManager;
class CTriggersManager;
class CSoundManager;
class CEntityManager;
class CMessageDispatcher;
class CWayPointManager;
class CParticleEmitterManager;
class CParticleSystemManager;
class CParticleSettingsManager;
class CParticleEmitterSystemManager;
class CParticleStartUpInstances;
class CAnimalManager;
class CSteeringBehaviorsSeetingsManager;
class CRailManager;
//--------------------------

class CCore : public CSingleton<CCore>
{
public:
	//--- Init and End protocols--------------------------------------------
								CCore		();
	virtual						~CCore		();

	bool						Init		( HWND hWnd, const SConfig &config );
	void						Done		();
	bool						IsOk		() const							{ return m_bIsOk; }

	//----Main Methods -----------------------------------------------------
	void						Update		( float _ElapsedTime );
	void						Render		();

	//---Load Methods -------------------------------------------------------
	bool		LoadFonts						();
	bool		LoadLanguages					();
	bool		LoadInputs						();
	bool		LoadEffects						();
	bool		LoadROTechniques				();
	bool		LoadStaticMeshes				();
	bool		LoadAnimatedModels				();
	bool		LoadROLayers					();
	bool		LoadLights						();
	bool		LoadRenderCommands				();
	bool		LoadPhysics						();
	bool		LoadBillboards					();
	bool		LoadParticles					();
	bool		LoadGUI							();
	bool		LoadTriggers					();
	bool		LoadSounds						();
	bool		LoadDebugGUI					();
	bool		LoadWaypoints					();
	bool		LoadAnimalMovements				();
	bool		LoadSteeringBehaviorSettings	();
	bool		LoadRails						();

	//----Reload Methods ---------------------------------------------------
	void		Reload									();
	void		ReloadTTFs								();	
	void		ReloadLanguages							();	
	void		ReloadInputs							();
	void		ReloadSceneRendererCommandManager		();
	void		ReloadRenderableObjectsLayersManager	();
	void		ReloadShaders							();
	void		ReloadEffects							();
	void		ReloadMeshes							();
	void		ReloadAnimatedModels					();
	void		ReloadPools								();
	void		ReloadScripts							();
	void		ReloadLights							();
	void		ReloadPhysics							();
	void		ReloadBillboards						();
	void		ReloadParticles							();
	void		ReloadTriggers							();
	void		ReloadGUI								();
	void		ReloadSounds							();
	void		ReloadWayPoints							();
	void		ReloadSteeringBehaviorSettings			();
	void		ReloadRails								();

	//----Unload Methods ---------------------------------------------------
	void		UnloadStaticMeshes						();
	void		UnloadAnimatedModels					();
	void		UnloadRenderableObjectsLayers			();
	void		UnloadLights							();
	void		UnloadBillboards						();
	void		UnloadParticles							();
	void		UnloadTriggers							();
	void		UnloadAnimalMovements					();
	void		UnloadWayPoints							();

	//----Properties ( get & Set )---------------------------------------
	void				SetCamera					( CCamera *_Camera )			{ m_pCamera = _Camera; }
	void				SetTimer					( CTimer *_Timer )				{ m_pTimer = _Timer; }
	void				SetProcess					( CEngineProcess *_Process )	{ m_pProcess = _Process; }
	void				SetGameMode					( bool _GameMode );
	void				SetDrawingShadows			(bool ds)						{ m_DrawingShadows = ds; }
	
	void				SetDyingAmount				(float amount)					
	{
		if(amount < 0.0f)
		{
			m_DyingAmount = 0.0f;
		}
		else if(amount > 1.0f)
		{
			m_DyingAmount = 1.0f;
		}
		else
		{
			m_DyingAmount = amount;
		}
	}


	inline float		GetDyingAmount				() const						{ return m_DyingAmount; }
	inline bool			IsDebugMode					() const						{ return m_bDebugMode; }
	inline bool			IsReleaseMode				() const						{ return m_bReleaseMode; }
	inline bool			IsGameMode					() const						{ return m_bIsGameMode; }
	inline bool			IsGUIMode					() const						{ return !m_bIsGameMode; }
	inline bool			IsDrawingShadows			() const						{ return m_DrawingShadows; }

	inline CCamera*								GetCamera					() const					{ return m_pCamera; }
	inline CTimer*								GetTimer					() const					{ return m_pTimer; }
	inline CEngineProcess*						GetProcess					() const					{ return m_pProcess; }
	inline CStadistics*							GetStadistics				() const					{ return m_pStadistics; }
	inline CDebugGUIManager*					GetDebugGUIManager			() const					{ return m_pDebugGUIManager; }

	inline CRenderManager*						GetRenderManager			() const					{ return m_pRenderManager; }
	inline CFontManager*						GetFontManager				() const					{ return m_pFontManager; }
	inline CLanguageManager*					GetLanguageManager			() const					{ return m_pLanguageManager; }
	inline CInputManager*						GetInputManager				() const					{ return m_pInputManager; }
	inline CActionToInput*						GetActionToInput			() const					{ return m_pActionToInput; }
	inline CTextureManager*						GetTextureManager			() const					{ return m_pTextureManager; }
	inline CStaticMeshManager*					GetStaticMeshManager		() const					{ return m_pStaticMeshManager; }
	inline CRenderableObjectsManager*			GetRenderableObjectsManager	() const					{ return m_pRenderableObjectsManager; }
	inline CAnimatedModelManager*				GetAnimatedModelManager		() const					{ return m_pAnimatedModelManager; }
	inline CLightManager*						GetLightManager				() const					{ return m_pLightManager; }
	inline CEffectManager*						GetEffectManager			() const					{ return m_pEffectManager; }
	inline CRenderableObjectTechniqueManager*	GetROTManager				() const					{ return m_pROTManager; }	
	inline CSceneRendererCommandManager*		GetSceneRendererCommandManager	 () const				{ return m_pSceneRendererCommandManager; }
	inline CRenderableObjectsLayersManager*		GetRenderableObjectsLayersManager() const				{ return m_pRenderableObjectsLayersManager; }
	inline CScriptManager*						GetScriptManager			() const					{ return m_pScriptManager; }
	inline CPhysicsManager*						GetPhysicsManager			() const					{ return m_pPhysicsManager; }
	inline CBillboardManager*					GetBillboardManager			() const					{ return m_pBillboardManager; }
	inline CWayPointManager*					GetWayPointManager			() const					{ return m_WayPointManager; }
	inline CGUIManager*							GetGUIManager				() const					{ return m_pGUIManager; }
	inline CTriggersManager*					GetTriggersManager			() const					{ return m_pTriggersManager; }					// Propietats dels triggers 
	inline CSoundManager*						GetSoundManager				() const					{ return m_pSoundManager; }						// Propietats dels sò 
	inline CEntityManager*						GetEntityManager			() const					{ return m_pEntityManager; }					// Accés al gestor lógic d'entitats 
	inline CMessageDispatcher*					GetMessageDispatcher		() const					{ return m_pMessageDispatcher; }				// Accés al gestor lógic de missatges

	inline CParticleEmitterManager*				GetParticleEmitterManager	() const					{ return m_ParticleEmitterManager; }
	inline CParticleSystemManager*				GetParticleSystemManager	() const					{ return m_ParticleSystemManager; }
	inline CParticleSettingsManager*			GetParticleSettingsManager	() const					{ return m_ParticleSettingsManager; }
	inline CParticleEmitterSystemManager*		GetParticleEmitterSystemManager	() const				{ return m_ParticleEmitterSystemManager; }
	inline CParticleStartUpInstances*			GetParticleStartUpInstances	() const					{ return m_ParticleStartUpInstances; }
	
	inline CAnimalManager*						GetAnimalManager			() const					{ return m_Animalmanager; }

	inline CSteeringBehaviorsSeetingsManager*	GetSteeringBehaviourSettingsManager () const			{ return m_pSteeringBehaviorSeetingsManager; }
	
	inline CRailManager*						GetRailManager				() const					{ return m_pRailManager; }

private:
	void Release();

	//----Inputs Methods -----------------------------------------------------
	void		UpdateInputs								( float _ElapsedTime );
	void		UpdateDebugInputs							( float _ElapsedTime, CActionToInput &_Action2Input );


private:
	bool	m_bIsOk;
	bool	m_bDebugMode;
	bool	m_bReleaseMode;
	bool	m_bIsGameMode;

	bool	m_DrawingShadows;
	
	float	m_DyingAmount;

	SConfig	m_Config;

	CCamera								*m_pCamera;						// punter a la càmera
	CTimer								*m_pTimer;						// punter al timer
	CDebugGUIManager					*m_pDebugGUIManager;			// punter al gestor de debug
	CEngineProcess						*m_pProcess;					// punter al process de física per tests o altres
	CStadistics							*m_pStadistics;

	CRenderManager						*m_pRenderManager;
	CFontManager						*m_pFontManager;
	CLanguageManager					*m_pLanguageManager;
	CInputManager						*m_pInputManager;
	CActionToInput						*m_pActionToInput;
	CTextureManager						*m_pTextureManager;
	CStaticMeshManager					*m_pStaticMeshManager;
	CRenderableObjectsManager			*m_pRenderableObjectsManager;
	CAnimatedModelManager				*m_pAnimatedModelManager;
	CLightManager						*m_pLightManager;
	CEffectManager						*m_pEffectManager;
	CRenderableObjectTechniqueManager	*m_pROTManager;
	CSceneRendererCommandManager		*m_pSceneRendererCommandManager;
	CRenderableObjectsLayersManager		*m_pRenderableObjectsLayersManager;
	CScriptManager						*m_pScriptManager;
	CPhysicsManager						*m_pPhysicsManager;
	CBillboardManager					*m_pBillboardManager;
	CGUIManager							*m_pGUIManager;
	CTriggersManager					*m_pTriggersManager;
	CSoundManager						*m_pSoundManager;
	CEntityManager						*m_pEntityManager;
	CMessageDispatcher					*m_pMessageDispatcher;
	CWayPointManager					*m_WayPointManager;
	CParticleEmitterManager				*m_ParticleEmitterManager;
	CParticleSystemManager				*m_ParticleSystemManager;
	CParticleSettingsManager			*m_ParticleSettingsManager;
	CParticleEmitterSystemManager		*m_ParticleEmitterSystemManager;
	CParticleStartUpInstances			*m_ParticleStartUpInstances;
	CAnimalManager						*m_Animalmanager;
	CSteeringBehaviorsSeetingsManager	*m_pSteeringBehaviorSeetingsManager;
	CRailManager						*m_pRailManager;
};

#endif