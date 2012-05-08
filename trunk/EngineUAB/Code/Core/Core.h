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
class CParticlesSystemManager;
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
//--------------------------

class CCore : public CSingleton<CCore>
{
public:
	//--- Init and End protocols--------------------------------------------
								CCore		( void );
	virtual						~CCore		( void );

	bool						Init		( HWND hWnd, const SConfig &config );
	void						Done		( void );
	bool						IsOk		( void ) const							{ return m_bIsOk; }

private:
	void Release();

	//----Main Methods -----------------------------------------------------
public:
	void						Update		( float _ElapsedTime );
	void						Render		( void );

	//----Inputs Methods -----------------------------------------------------
	void						UpdateInputs								( float _ElapsedTime );
	void						UpdateDebugInputs							( float _ElapsedTime, CActionToInput &_Action2Input );

	//----Reload Methods ---------------------------------------------------
	void						Reload										( void );
	void						ReloadTTFs									( void );	
	void						ReloadLanguages								( void );	
	void						ReloadInputs								( void );
	void						ReloadSceneRendererCommandManager			( void );
	void						ReloadRenderableObjectsLayersManager		( void );
	void						ReloadShaders								( void );
	void						ReloadEffects								( void );
	void						ReloadMeshes								( void );
	void						ReloadAnimatedModels						( void );
	void						ReloadPools									( void );
	void						ReloadScripts								( void );
	void						ReloadLights								( void );
	void						ReloadPhysics								( void );
	void						ReloadBillboards							( void );
	void						ReloadParticlesManager						( void );
	void						ReloadTriggersManager						( void );
	void						ReloadGUIManager							( void );
	void						ReloadSoundManager							( void );

	//----Properties ( get & Set )---------------------------------------
	void										SetCamera					( CCamera *_Camera )			{ m_pCamera = _Camera; }
	void										SetTimer					( CTimer *_Timer )				{ m_pTimer = _Timer; }
	void										SetProcess					( CEngineProcess *_Process )	{ m_pProcess = _Process; }

	inline bool									IsDebugMode					() const						{ return m_bDebugMode; }
	inline bool									IsReleaseMode				() const						{ return m_bReleaseMode; }

	inline CCamera*								GetCamera					( void ) const					{ return m_pCamera; }
	inline CTimer*								GetTimer					( void ) const					{ return m_pTimer; }
	inline CEngineProcess*						GetProcess					( void ) const					{ return m_pProcess; }
	inline CStadistics*							GetStadistics				( void ) const					{ return m_pStadistics; }
	inline CDebugGUIManager*					GetDebugGUIManager			( void ) const					{ return m_pDebugGUIManager; }

	inline CRenderManager*						GetRenderManager			( void ) const					{ return m_pRenderManager; }
	inline CFontManager*						GetFontManager				( void ) const					{ return m_pFontManager; }
	inline CLanguageManager*					GetLanguageManager			( void ) const					{ return m_pLanguageManager; }
	inline CInputManager*						GetInputManager				( void ) const					{ return m_pInputManager; }
	inline CActionToInput*						GetActionToInput			( void ) const					{ return m_pActionToInput; }
	inline CTextureManager*						GetTextureManager			( void ) const					{ return m_pTextureManager; }
	inline CStaticMeshManager*					GetStaticMeshManager		( void ) const					{ return m_pStaticMeshManager; }
	inline CRenderableObjectsManager*			GetRenderableObjectsManager	( void ) const					{ return m_pRenderableObjectsManager; }
	inline CAnimatedModelManager*				GetAnimatedModelManager		( void ) const					{ return m_pAnimatedModelManager; }
	inline CLightManager*						GetLightManager				( void ) const					{ return m_pLightManager; }
	inline CEffectManager*						GetEffectManager			( void ) const					{ return m_pEffectManager; }
	inline CRenderableObjectTechniqueManager*	GetROTManager				( void ) const					{ return m_pROTManager; }	
	inline CSceneRendererCommandManager*		GetSceneRendererCommandManager	 ( void ) const				{ return m_pSceneRendererCommandManager; }
	inline CRenderableObjectsLayersManager*		GetRenderableObjectsLayersManager( void ) const				{ return m_pRenderableObjectsLayersManager; }
	inline CScriptManager*						GetScriptManager			( void ) const					{ return m_pScriptManager; }
	inline CPhysicsManager*						GetPhysicsManager			( void ) const					{ return m_pPhysicsManager; }
	inline CBillboardManager*					GetBillboardManager			( void ) const					{ return m_pBillboardManager; }
	inline CParticlesSystemManager*				GetParticlesManager			( void ) const					{ return m_pParticlesManager; }
	inline CGUIManager*							GetGUIManager				( void ) const					{ return m_pGUIManager; }
	inline CTriggersManager*					GetTriggersManager			( void ) const					{ return m_pTriggersManager; }					// Propietats dels triggers 
	inline CSoundManager*						GetSoundManager				( void ) const					{ return m_pSoundManager; }						// Propietats dels sò 

	//----Members ---------------------------------------
private:
	bool	m_bIsOk;
	bool	m_bDebugMode;
	bool	m_bReleaseMode;

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
	CParticlesSystemManager				*m_pParticlesManager;
	CGUIManager							*m_pGUIManager;
	CTriggersManager					*m_pTriggersManager;
	CSoundManager						*m_pSoundManager;
};

#endif