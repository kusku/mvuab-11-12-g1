//----------------------------------------------------------------------------------
// CObject3D class
// Author: Enric Vergara
//
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_OBJECT_3D_H_
#define INC_OBJECT_3D_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "Math\Matrix44.h"
#include <string>

class CObject3D
{
public:
	CObject3D(const Vect3f& pos, const Vect3f& scale, float yaw, float pitch, float roll = 0);
	CObject3D();
	virtual ~CObject3D(void) {/*Nothing*/;}

	//---Get Functions
	inline float				GetYaw				() const { return m_fYaw; }
	inline float				GetRoll				() const { return m_fRoll; }
	inline float				GetPitch			() const { return m_fPitch; }
	inline const Vect3f&		GetPosition			() const { return m_Position; }
	inline const Vect3f&		GetScale			() const { return m_Scale; }
	inline bool					GetVisible			() const { return m_Visible; }
	inline const std::string&	GetTechniqueName	() const { return m_TechniqueName; }

	const Mat44f				GetTransform		() const;

	//---Set Functions
	inline void		SetPosition			( const Vect3f& pos )			{ m_Position = pos; }
	inline void		SetScale			( const Vect3f& sca )			{ m_Scale = sca; }
	inline void		SetYaw				( float yaw )					{ m_fYaw = yaw; }
	inline void		SetPitch			( float pitch )					{ m_fPitch = pitch; }
	inline void		SetRoll				( float roll )					{ m_fRoll = roll; }
	inline void		SetVisible			( bool visible )				{ m_Visible = visible; }
	inline void		SetTechniqueName	( const std::string& name )		{ m_TechniqueName = name; }

protected:
	Vect3f		m_Position;
	Vect3f		m_Scale;
	float		m_fYaw;
	float		m_fPitch;
	float		m_fRoll;

	bool		m_Visible;

	std::string m_TechniqueName;
};

#endif //INC_CORE_H_