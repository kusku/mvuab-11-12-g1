#pragma once

#ifndef __CLASS_STATE_H__
#define __CLASS_STATE_H__

#include <string>
#include "MessageDefs.h"
#include "Math\Matrix44.h"
#include "Particles\ParticleEmitterManager.h"
#include "Particles\ParticleEmitterInstance.h"
#include "Particles\ParticleEmitterSystemManager.h"
#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

//#include "mathutils\Mat44f"

#include "Core.h"

//--- Foward Declarations ---//
class CCharacter;
//---------------------------//

struct STelegram;

#include "Utils\Named.h"
#include <string>

template <class T>
class CState : public CNamed
{
public:
	// ------------- Constructors i Destructors ----------------
					CState		( T* )								{ return; }
					CState		( T* , const std::string &_Name )	{ SetName(_Name); }
	virtual			~CState		( void ) {};

	// ------------- Mètodes Principals ------------------------
	
	// Se ejecuta en cada update del estado
	virtual void	Execute		( T*, float _ElapsedTime ) {
											return;			
									   };

	// Se ejecuta cuando el estado es entrado
	virtual void	OnEnter		( T* ) {
											return;			
									   };

	// Se ejecuta cuando el estado sale
	virtual void	OnExit		( T* ) {
											return;			
									   };

	//// Pausa la ejecución del estado
	//virtual void	OnSuspend	( T* )						{};

	//// Permite la continuación de la ejecución del estado
	//virtual void	OnResume	( T* )						{};

	// Este se ejecuta si el agente recibe un mensaje de un dispensador de mensajes
	virtual bool	OnMessage	( T*, const STelegram& ) { 
															return true; 
														};
	
	//----Methods --------------------------------------------
	void SetParticlePosition( CCharacter* _pCharacter, const std::string &_ParticlesCoreName, const std::string &_ParticlesInstaceName, const std::string &_Bone = "", const Vect3f &_Position = Vect3f(0,0,0))
	{
		if ( _Bone.compare( "" ) != 0 )
		{
			CAnimatedInstanceModel *l_pAnimatedModel = _pCharacter->GetAnimatedModel();
	
			Mat44f l_TransformMatrix		= m44fIDENTITY;
			Mat44f l_RotationMatrix			= m44fIDENTITY;
			Vect4f l_Rotation				= v3fZERO;
			Vect3f l_Translation			= v3fZERO;
			Mat44f l_AnimatedModelTransform = l_pAnimatedModel->GetTransform();
	
			l_pAnimatedModel->GetBonePosition(_Bone, l_Translation);
			l_pAnimatedModel->GetBoneRotation(_Bone, l_Rotation);
	
			l_TransformMatrix.Translate(l_Translation);
			l_RotationMatrix.SetFromQuaternion(l_Rotation);
	
			l_TransformMatrix = l_AnimatedModelTransform * l_TransformMatrix * l_RotationMatrix;
	
			GetParticleEmitterInstance(_ParticlesCoreName, _ParticlesInstaceName)->SetPosition( l_TransformMatrix.GetPos() );
		}
		else 
		{
			GetParticleEmitterInstance(_ParticlesCoreName, _ParticlesInstaceName)->SetPosition( _Position );
		}
	}

	//----Properties -----------------------------------------
	CParticleEmitterInstance* GetParticleEmitterInstance( const std::string & _EmitterCoreName, const std::string & _EmitterInstanceName ) const 
	{ 
		return CORE->GetParticleEmitterManager()->GetResource(_EmitterCoreName)->GetParticleEmitterInstance(_EmitterInstanceName); 
	}

	CParticleEmitterSystemInstance* GetParticleGroupInstance( const std::string & _EmitterCoreGroupName, const std::string & _EmitterInstanceGroupName ) const 
	{ 
		return CORE->GetParticleEmitterSystemManager()->GetResource(_EmitterCoreGroupName)->GetParticleEmitterSystemInstance(_EmitterInstanceGroupName); 
	}
	// ------------- Members ---------------------------------
private:

};

#endif __CLASS_STATE_H__