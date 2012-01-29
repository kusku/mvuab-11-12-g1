#pragma once

#ifndef _CORE_H
#define _CORE_H

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

#include <Windows.h>
#include "Utils\Singleton.h"
#include "CoreDefs.h"

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

	void	Reload					();
	void	ReloadTTFs				();	
	void	ReloadLanguages			();	
	void	ReloadInputs			();
	void	ReloadRenderableObjects	();

	CRenderManager*							GetRenderManager() const			{ return m_pRenderManager; }
	CFontManager*							GetFontManager() const				{ return m_pFontManager; }
	CLanguageManager*						GetLanguageManager() const			{ return m_pLanguageManager; }
	CInputManager*							GetInputManager() const				{ return m_pInputManager; }
	CActionToInput*							GetActionToInput() const			{ return m_pActionToInput; }
	CTextureManager*						GetTextureManager() const			{ return m_pTextureManager; }
	CStaticMeshManager*						GetStaticMeshManager() const		{ return m_pStaticMeshManager; }
	CRenderableObjectsManager*				GetRenderableObjectsManager() const	{ return m_pRenderableObjectsManager; }
	CAnimatedModelManager*					GetAnimatedModelManager() const		{ return m_pAnimatedModelManager; }
	CLightManager*							GetLightManager() const				{ return m_pLightManager; }
	CEffectManager*							GetEffectManager() const			{ return m_pEffectManager; }
	CRenderableObjectTechniqueManager*		GetROTManager() const				{ return m_pROTManager; }		

private:
	void Release();

	bool	m_bIsOk;

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
};

#endif