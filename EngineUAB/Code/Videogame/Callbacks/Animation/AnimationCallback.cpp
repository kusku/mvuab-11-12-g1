#include "AnimationCallback.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"
#include "RenderableObjects\AnimatedModel\AnimatedCoreModel.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimationCallback::CAnimationCallback(const std::string &_Animation, CAnimatedInstanceModel *_pAnimatedModel)
	: m_pAnimatedModel(_pAnimatedModel)
	, m_AnimationName(_Animation)
	, m_bIsAnimationFinished(false)
{

}

CAnimationCallback::~CAnimationCallback()
{

}

void CAnimationCallback::AnimationUpdate(float anim_time, CalModel *model, void * userData)
{

}

void CAnimationCallback::AnimationComplete(CalModel *model, void * userData)
{
	m_bIsAnimationFinished = true;
}

bool CAnimationCallback::IsAnimationFinished() const
{
	return m_bIsAnimationFinished;
}