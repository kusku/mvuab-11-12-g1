#include "AnimationCallbackManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"
#include "AnimationCallback.h"
#include "Base.h"
#include <assert.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimationCallbackManager::CAnimationCallbackManager()
{

}

CAnimationCallbackManager::~CAnimationCallbackManager()
{
	TCallbackMap::iterator l_It = m_CallbackMap.begin();
	TCallbackMap::iterator l_End = m_CallbackMap.end();

	//Elimina el callback de cal3d
	for(; l_It != l_End; ++l_It)
	{
		CAnimationCallback *l_pAnimationCallback = l_It->second;

		std::string l_CallbackName = l_pAnimationCallback->GetAnimationName();
		CAnimatedInstanceModel *l_pAnimatedModel = l_pAnimationCallback->GetAnimatedModel();

		int l_ID = l_pAnimatedModel->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimationId( l_CallbackName );
		l_pAnimatedModel->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimation(l_ID)->removeCallback( l_pAnimationCallback );
	}

	m_CallbackMap.clear();
}

bool CAnimationCallbackManager::CreateCallback( const std::string &_Animation, CAnimatedInstanceModel *_pAnimatedModel )
{
	assert( _pAnimatedModel != NULL );
	assert( _Animation != "" );

	//Crea el callback
	CAnimationCallback *l_pAnimationCallback = new CAnimationCallback(_Animation, _pAnimatedModel);
	TCallbackMap::iterator l_It = m_CallbackMap.find(_Animation);
	if( l_It == m_CallbackMap.end() )
	{
		m_CallbackMap[_Animation] = l_pAnimationCallback;
	}
	else
	{
		CHECKED_DELETE(l_pAnimationCallback);
		return false;
	}

	//Añade el callback a la animación
	int l_ID = _pAnimatedModel->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimationId( _Animation );
	CalCoreAnimation *l_pCoreAnimation = _pAnimatedModel->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimation(l_ID);
	l_pCoreAnimation->registerCallback(l_pAnimationCallback, 0.f);
	return true;
}

void CAnimationCallbackManager::RemoveCallback( const std::string &_Animation )
{
	TCallbackMap::iterator l_It = m_CallbackMap.find(_Animation);
	if( l_It != m_CallbackMap.end() )
	{
		CAnimationCallback *l_pAnimationCallback = l_It->second;
		CAnimatedInstanceModel *l_pAnimatedModel = l_pAnimationCallback->GetAnimatedModel();

		//Elimina el callback de cal3d
		int l_ID = l_pAnimatedModel->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimationId( _Animation );
		l_pAnimatedModel->GetAnimatedCoreModel()->GetCoreModel()->getCoreAnimation(l_ID)->removeCallback(l_pAnimationCallback);

		//Elimina el callback del map
		m_CallbackMap.erase( l_It );
	}
}

CAnimationCallback* CAnimationCallbackManager::GetCallback( const std::string &_Animation )
{
	TCallbackMap::iterator l_It = m_CallbackMap.find(_Animation);
	if( l_It != m_CallbackMap.end() )
	{
		return l_It->second;
	}

	return NULL;
}