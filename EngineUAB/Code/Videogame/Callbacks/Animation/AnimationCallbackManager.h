#pragma once

#ifndef _CALLBACK_MANAGER_H_
#define _CALLBACK_MANAGER_H_

#include "AnimationCallback.h"
#include "cal3d\animcallback.h"
#include <string>
#include <map>

//---Forward Declarations----------
class CAnimatedInstanceModel;
//---------------------------------

class CAnimationCallbackManager
{
public:
	CAnimationCallbackManager();
	~CAnimationCallbackManager();

	bool				CreateCallback		( const std::string &_Animation, CAnimatedInstanceModel *_pAnimatedModel );
	void				RemoveCallback		( const std::string &_Animation );
	CAnimationCallback*	GetCallback			( const std::string &_Animation );

private:
	typedef std::map< std::string, CAnimationCallback* >	TCallbackMap;
	TCallbackMap		m_CallbackMap;
};

#endif //_CALLBACK_MANAGER_H_