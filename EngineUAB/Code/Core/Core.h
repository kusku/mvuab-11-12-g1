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
class CCamera;
class CLogRender;
class CDebugRender;
class CTimer;
class CModifierManager;
class CScriptManager;
class CPhysicsManager;
class CConsole;
class CProcess;
class CStadistics;
class CDebugOptions;

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

	void			SetCamera		( CCamera *camera )				{ m_pCamera = camera; }
	void			SetTimer		( CTimer *timer )				{ m_pTimer = timer; }
	void			SetLogRender	( CLogRender *logRender )		{ m_pLogRender = logRender; }
	void			SetDebugRender	( CDebugRender *DebugRender )	{ m_pDebugRender = DebugRender; }
	void			SetProcess		( CProcess *Process )			{ m_pProcess = Process; }

	inline CCamera*			GetCamera		() const				{ return m_pCamera; }
	inline CTimer*			GetTimer		() const				{ return m_pTimer; }
	inline CLogRender*		GetLogRender	() const				{ return m_pLogRender; }
	inline CDebugRender*	GetDebugRender	() const				{ return m_pDebugRender; }
	inline CConsole*		GetConsole		() const				{ return m_pConsole; }
	inline CProcess*		GetProcess		() const				{ return m_pProcess; }
	inline CStadistics*		GetStadistics	() const				{ return m_pStadistics; }
	inline CDebugOptions*	GetDebugOptions	() const				{ return m_pDebugOptions; }

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
	inline CModifierManager*					GetModifierManager() const					{ return m_pModifierManager; }
	inline CScriptManager*						GetScriptManager() const					{ return m_pScriptManager; }
	inline CPhysicsManager*						GetPhysicsManager() const					{ return m_pPhysicsManager; }

private:
	void Release();

	bool	m_bIsOk;

	CCamera								*m_pCamera;
	CDebugRender						*m_pDebugRender;
	CLogRender							*m_pLogRender;
	CTimer								*m_pTimer;
	CConsole							*m_pConsole;
	CProcess							*m_pProcess;
	CStadistics							*m_pStadistics;
	CDebugOptions						*m_pDebugOptions;

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
	CModifierManager					*m_pModifierManager;
	CScriptManager						*m_pScriptManager;
	CPhysicsManager						*m_pPhysicsManager;
};

#endif