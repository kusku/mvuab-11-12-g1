//----------------------------------------------------------------------------------
// CSpeaker class
// Author: Marc Cuscullola
//
// Description:
// Clase que representa un altavoz en el mundo
//----------------------------------------------------------------------------------
#pragma once

#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include "Utils\Named.h"
#include "Math\Vector3.h"
#include <AK/SoundEngine/Common/AkTypes.h>

//--- Foward Declarations ---
class CObject3D;
class CRenderManager;
//---------------------------

class CSpeaker : public CNamed
{
public:
	//----Constructor & Destructor--------------
	CSpeaker( AkGameObjectID _ID, const std::string &_Name );
	CSpeaker( AkGameObjectID _ID, CObject3D *_Object3D, const std::string &_Name );
	~CSpeaker();

	//----Common Methods-----------------------
	void	Init	();
	void	Update	();
	void	Render	( CRenderManager &_RM );

	//----Properties----------------------------
	inline void				SetPosition		( const Vect3f &pos )		{ m_SoundPosition.Position.X = pos.x; m_SoundPosition.Position.Y = pos.y; m_SoundPosition.Position.Z = pos.z; }
	inline void				SetOrientation	( const Vect3f &orien )		{ m_SoundPosition.Orientation.X = orien.x; m_SoundPosition.Orientation.Y = orien.y; m_SoundPosition.Orientation.Z = orien.z; }
	inline void				SetObject3D		( CObject3D *object )		{ m_pObject3D = object; }
	inline void				SetID			( AkGameObjectID id )		{ m_ID = id; }

	inline const AkSoundPosition&	GetSoundPosition() const					{ return m_SoundPosition; }
	inline CObject3D*				GetObject3D		() const					{ return m_pObject3D; }
	inline AkGameObjectID			GetID			() const					{ return m_ID; }

private:
	AkSoundPosition		m_SoundPosition;
	AkGameObjectID		m_ID;
	CObject3D			*m_pObject3D;
};

#endif