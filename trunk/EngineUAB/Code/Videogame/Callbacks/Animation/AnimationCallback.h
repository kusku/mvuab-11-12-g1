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
	//---Constructors &  Destructors----------------------------
							CAnimationCallback	( const std::string &_Animation, const std::string &_CalAnimationName, CAnimatedInstanceModel *_pAnimatedModel );
							~CAnimationCallback	();

	//---Main Methods----------------------------
	void					Init				( void )		{ m_bIsAnimationStarted = false; }
	virtual void			AnimationUpdate		(float anim_time,CalModel *model, void * userData);
	virtual void			AnimationComplete	(CalModel *model, void * userData);
	
	//---Methods---------------------------------
	void					StartAnimation		()				{ m_bIsAnimationFinished = false; m_bIsAnimationStarted = true; }
	bool					IsAnimationFinished	() const;
	bool					IsAnimationStarted	() const		{ return m_bIsAnimationStarted; }		// Me permite saber si he realizado el Start Animation
	
	//---Set & Get Methods-----------------------
	const std::string&		GetAnimationName	() const		{ return m_AnimationName; }
	CAnimatedInstanceModel*	GetAnimatedModel	() const		{ return m_pAnimatedModel; }

private:
	bool					m_bIsAnimationFinished;
	bool					m_bIsAnimationStarted;
	std::string				m_AnimationName;
	std::string				m_CalAnimationName;
	CAnimatedInstanceModel	*m_pAnimatedModel;
};

#endif //_ANIMATION_CALLBACK_H_