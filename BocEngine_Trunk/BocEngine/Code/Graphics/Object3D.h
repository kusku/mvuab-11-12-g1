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

class CObject3D
{
public:
	CObject3D(const Vect3f& pos, const Vect3f& scale, float yaw, float pitch, float roll = 0);
	CObject3D();
	virtual ~CObject3D(void) {/*Nothing*/;}

	//---Get Functions
	float						GetYaw			() const { return m_fYaw; }
	float						GetRoll			() const { return m_fRoll; }
	float						GetPitch		() const { return m_fPitch; }
	const Vect3f&				GetPosition		() const { return m_Position; }
	const Vect3f&				GetScale		() const { return m_Scale; }
	bool						GetVisible		() const { return m_Visible; }
	const Mat44f&				GetTransform	() const { return m_WorldMatrix; }

	//---Set Functions
	void		SetPosition		( const Vect3f& pos )			{ m_Position = pos; }
	void		SetScale		( const Vect3f& sca )			{ m_Scale = sca; }
	void		SetYaw			( float yaw )					{ m_fYaw = yaw; }
	void		SetPitch		( float pitch )					{ m_fPitch = pitch; }
	void		SetRoll			( float roll )					{ m_fRoll = roll; }
	void		SetVisible		( bool visible )				{ m_Visible = visible; }
protected:
	Vect3f		m_Position;
	Vect3f		m_Scale;
	float		m_fYaw;
	float		m_fPitch;
	float		m_fRoll;

	bool		m_Visible;

	Mat44f		m_WorldMatrix;
	
};

#endif //INC_CORE_H_