#pragma once

#ifndef _CORE_H
#define _CORE_H

#include <Windows.h>
#include "Utils\Singleton.h"
#include "CoreDefs.h"

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

class CCore : public CSingleton<CCore>
{
public:
	CCore();
	virtual ~CCore();

	bool Init		( HWND hWnd, const SConfig &config );
	void Done		();
	bool IsOk		() const { return m_bIsOk; }

	void Update		( float ElapsedTime );
	void Render		();

	void	Reload									();
	void	ReloadTTFs								();	
	void	ReloadLanguages							();	
	void	ReloadInputs							();
	void	ReloadSceneRendererCommandManager		();
	void	ReloadRenderableObjectsLayersManager	();
	void	ReloadEffects							();
	void	ReloadMeshes							();
	void	ReloadPools								();
	void	ReloadScripts							();
	void	ReloadLights							();

	void			SetCamera		( CCamera *camera )				{ m_pCamera = camera; }
	void			SetTimer		( CTimer *timer )				{ m_pTimer = timer; }
	void			SetProcess		( CEngineProcess *Process )		{ m_pProcess = Process; }

	inline CCamera*				GetCamera			() const				{ return m_pCamera; }
	inline CTimer*				GetTimer			() const				{ return m_pTimer; }
	inline CEngineProcess*		GetProcess			() const				{ return m_pProcess; }
	inline CStadistics*			GetStadistics		() const				{ return m_pStadistics; }
	inline CDebugGUIManager*	GetDebugGUIManager	() const				{ return m_pDebugGUIManager; }

	inline CRenderManager*						GetRenderManager() const					{ return m_pRenderManager; }
	inline CFontManager*						GetFontManager() const						{ return m_pFontManager; }
	inline CLanguageManager*					GetLanguageManager() const					{ return m_pLanguageManager; }
	inline CInputManager*						GetInputManager() const						{ return m_pInputManager; }
	inline CActionToInput*						GetActionToInput() const					{ return m_pActionToInput; }
	inline CTextureManager*						GetTextureManager() const					{ return m_pTextureManager; }
	inline CStaticMeshManager*					GetStaticMeshManager() const				{ return m_pStaticMeshManager; }
	inline CRenderableObjectsManager*			GetRenderableObjectsManager() const			{ return m_pRenderableObjectsManager; }
	inline CAnimatedModelManager*				GetAnimatedModelManager() const				{ return m_pAnimatedModelManager; }
	inline CLightManager*						GetLightManager() const						{ return m_pLightManager; }
	inline CEffectManager*						GetEffectManager() const					{ return m_pEffectManager; }
	inline CRenderableObjectTechniqueManager*	GetROTManager() const						{ return m_pROTManager; }	
	inline CSceneRendererCommandManager*		GetSceneRendererCommandManager() const		{ return m_pSceneRendererCommandManager; }
	inline CRenderableObjectsLayersManager*		GetRenderableObjectsLayersManager() const	{ return m_pRenderableObjectsLayersManager; }
	inline CScriptManager*						GetScriptManager() const					{ return m_pScriptManager; }
	inline CPhysicsManager*						GetPhysicsManager() const					{ return m_pPhysicsManager; }
	inline CBillboardManager*					GetBillboardManager() const					{ return m_pBillboardManager; }
	inline CParticlesSystemManager*				GetParticleManager() const					{ return m_pParticleManager; }
	inline CGUIManager*							GetGUIManager() const						{ return m_pGUIManager; }

private:
	void Release();

	bool	m_bIsOk;

	CCamera								*m_pCamera;
	CTimer								*m_pTimer;
	CDebugGUIManager					*m_pDebugGUIManager;
	CEngineProcess						*m_pProcess;
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
	CParticlesSystemManager				*m_pParticleManager;
	CGUIManager							*m_pGUIManager;
};

#endif