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
#include "Utils\Named.h"
#include <boost\math\special_functions\fpclassify.hpp>

class CObject3D : public CNamed
{
public:
	//--- Init and End protocols------------------------------------------
								CObject3D			( const Vect3f& pos, const Vect3f& scale, float yaw, float pitch, float roll = 0 );
								CObject3D			();
	virtual						~CObject3D			() {/*Nothing*/;}

	//---- Methods ---------------------------------------------------------
	void						InitMat44			( void );

	inline void					AddYaw				( float _Radians )						{ m_fYaw   += _Radians; }
	inline void					AddPitch			( float _Radians )						{ m_fPitch += _Radians; }
	inline void					AddRoll				( float _Radians )						{ m_fRoll  += _Radians; }

	//----Properties ( get & Set )---------------------------------------
	//---Get Functions
	inline float				GetYaw				() const					{ return m_fYaw; }
	inline float				GetRoll				() const					{ return m_fRoll; }
	inline float				GetPitch			() const					{ return m_fPitch; }
	inline const Vect3f&		GetPosition			() const					{ return m_Position; }
	inline const Vect3f&		GetScale			() const					{ return m_Scale; }
	inline bool					GetVisible			() const					{ return m_Visible; }
	inline const std::string&	GetTechniqueName	() const					{ return m_TechniqueName; }
	const Mat44f				GetTransform		() const;
	const Mat44f				GetRotationMatrix		() const;

	//---Set Functions
	inline void					SetPosition			( const Vect3f& pos )			{ 
																						/*bool l_isNan  = boost::math::isnan( pos.x );
																						if ( l_isNan )
																						{
																							return;
																						}*/
																						/*if ( pos.x <= 4.035e-039 )
																							return;

																						if ( pos.x == -1.#IND00 )
																							return;*/
																						
																						m_Position = pos; 
																					}

	inline void					SetScale			( const Vect3f& sca )			{ m_Scale = sca; }
	inline void					SetYaw				( float yaw )					{ m_fYaw = yaw; }
	inline void					SetPitch			( float pitch )					{ m_fPitch = pitch; }
	inline void					SetRoll				( float roll )					{ m_fRoll = roll; }
	inline void					SetVisible			( bool visible )				{ m_Visible = visible; }
	inline void					SetTechniqueName	( const std::string& name )		{ m_TechniqueName = name; }

	inline Vect3f				GetFront			() const						{ Vect3f l_Front; l_Front.GetXZFromAngle( mathUtils::Deg2Rad(m_fYaw) ) ; return l_Front; }

	//----Members -------------------------------------------------------
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