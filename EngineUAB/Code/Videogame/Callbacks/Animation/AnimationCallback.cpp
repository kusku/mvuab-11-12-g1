#include "AnimationCallback.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimationCallback::CAnimationCallback(const std::string &_Animation, const std::string &_CalAnimationName, CAnimatedInstanceModel *_pAnimatedModel)
	: m_pAnimatedModel(_pAnimatedModel)
	, m_AnimationName(_Animation)
	, m_CalAnimationName(_CalAnimationName)
	, m_bIsAnimationFinished(false)
	, m_bIsAnimationStarted	(false)
{

}

CAnimationCallback::~CAnimationCallback()
{

}

void CAnimationCallback::AnimationUpdate(float anim_time, CalModel *model, void * userData)
{
	CalAnimation *l_pAnimation = model->getMixer()->getAnimationAction(m_CalAnimationName);
	if(l_pAnimation != NULL)
	{
		if( l_pAnimation->getTime() == l_pAnimation->getCoreAnimation()->getDuration() )
		{
			m_bIsAnimationFinished = true;
		}
	}
}

void CAnimationCallback::AnimationComplete(CalModel *model, void * userData)
{
	//m_bIsAnimationStarted = false;	// Dudo, esto puede provocar alguna confusión
	m_bIsAnimationFinished = true;
}

bool CAnimationCallback::IsAnimationFinished() const
{
	return m_bIsAnimationFinished;
}