#pragma once

#ifndef _ANIMATION_CALLBACK_H_
#define _ANIMATION_CALLBACK_H_

#include "cal3d\cal3d.h"
#include "cal3d\animcallback.h"
#include <string>

//---Forward Declarations----------
class CAnimatedInstanceModel;
//---------------------------------

class CAnimationCallback : public CalAnimationCallback
{
public:
	CAnimationCallback( const std::string &_Animation, CAnimatedInstanceModel *_pAnimatedModel );
	~CAnimationCallback();

	virtual void AnimationUpdate(float anim_time,CalModel *model, void * userData);
	virtual void AnimationComplete(CalModel *model, void * userData);

	//---Set & Get Methods-----------------------
	void					StartAnimation		()				{ m_bIsAnimationFinished = false; }
	bool					IsAnimationFinished	() const;
	const std::string&		GetAnimationName	() const		{ return m_AnimationName; }
	CAnimatedInstanceModel*	GetAnimatedModel	() const		{ return m_pAnimatedModel; }

private:
	bool					m_bIsAnimationFinished;
	std::string				m_AnimationName;
	CAnimatedInstanceModel	*m_pAnimatedModel;
};

#endif //_ANIMATION_CALLBACK_H_