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
	void	ReloadRenderableObjects					();
	void	ReloadSceneRendererCommandManager		();
	void	ReloadRenderableObjectsLayersManager	();

	void			SetCamera		( CCamera *camera )			{ m_pCamera = camera; }
	CCamera*		GetCamera		() const					{ return m_pCamera; }

	CRenderManager*							GetRenderManager() const					{ return m_pRenderManager; }
	CFontManager*							GetFontManager() const						{ return m_pFontManager; }
	CLanguageManager*						GetLanguageManager() const					{ return m_pLanguageManager; }
	CInputManager*							GetInputManager() const						{ return m_pInputManager; }
	CActionToInput*							GetActionToInput() const					{ return m_pActionToInput; }
	CTextureManager*						GetTextureManager() const					{ return m_pTextureManager; }
	CStaticMeshManager*						GetStaticMeshManager() const				{ return m_pStaticMeshManager; }
	CRenderableObjectsManager*				GetRenderableObjectsManager() const			{ return m_pRenderableObjectsManager; }
	CAnimatedModelManager*					GetAnimatedModelManager() const				{ return m_pAnimatedModelManager; }
	CLightManager*							GetLightManager() const						{ return m_pLightManager; }
	CEffectManager*							GetEffectManager() const					{ return m_pEffectManager; }
	CRenderableObjectTechniqueManager*		GetROTManager() const						{ return m_pROTManager; }	
	CSceneRendererCommandManager*			GetSceneRendererCommandManager() const		{ return m_SceneRendererCommandManager; }
	CRenderableObjectsLayersManager*		GetRenderableObjectsLayersManager() const	{ return m_RenderableObjectsLayersManager; }
	
private:
	void Release();

	bool	m_bIsOk;

	CCamera								*m_pCamera;

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
	CSceneRendererCommandManager		*m_SceneRendererCommandManager;
	CRenderableObjectsLayersManager		*m_RenderableObjectsLayersManager;
};

#endif