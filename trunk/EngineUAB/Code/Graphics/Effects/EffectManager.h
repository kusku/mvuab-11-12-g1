#pragma once

#ifndef _EFFECTMANAGER_H
#define _EFFECTMANAGER_H

#include "Effect.h"
#include "EffectTechnique.h"
#include "Utils\MapManager.h"
#include "Utils\Types.h"
#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include <string>
#include <vector>
#include <map>

class CEffectManager : public CMapManager<CEffectTechnique>
{
public:
	//--- Init and End protocols------------------------------------------
						CEffectManager							( void );
	virtual				~CEffectManager							( void );

	//----Main Functions ------------------------------------------------
	bool				Load									( const std::string &_FileName );
	bool				Reload									( void );
	bool				ReloadShaders							( void );

	//----Functions -----------------------------------------------------
	void				ActivateCamera							( const Mat44f &_ViewMatrix, const Mat44f &_ProjectionMatrix,const Vect3f &_CameraEye, const Mat44f &PrevViewMatrix, const Mat44f &PrevProjectionMatrix);
	void				CleanUp									( void );

	//----Functions privadas ---------------------------------------------
private:
	bool				LoadXML									( void );
	void				AssignDefaultTechniques					( void );

	//----Properties ( get & Set )---------------------------------------
public:
	std::string			GetTechniqueEffectNameByVertexDefault	( uint16 VertexType );
	size_t				GetMaxLights							( void ) const;

	CEffect*			GetEffect								( const std::string &Name )			{ return m_Effects.GetResource(Name); }
	CEffectTechnique*	GetEffectTechnique						( const std::string &Name )			{ return GetResource(Name); }

	void				SetStaticMeshTechnique					( CEffectTechnique *StaticMeshTechnique )		{ m_StaticMeshTechnique = StaticMeshTechnique; }
	void				SetAnimatedModelTechnique				( CEffectTechnique *AnimatedModelTechnique )	{ m_AnimatedModelTechnique = AnimatedModelTechnique ; }

	CEffectTechnique*	GetStaticMeshTechnique					() const							{ return m_StaticMeshTechnique; }
	CEffectTechnique*	GetAnimatedModelTechnique				() const							{ return m_AnimatedModelTechnique; }

	const Mat44f&		GetWorldMatrix							() const							{ return m_WorldMatrix; }
	const Mat44f&		GetProjectionMatrix						() const							{ return m_ProjectionMatrix; }
	const Mat44f&		GetViewMatrix							() const							{ return m_ViewMatrix; }
	const Mat44f&		GetViewProjectionMatrix					() const							{ return m_ViewProjectionMatrix; }
	const Mat44f&		GetPrevProjectionMatrix					() const							{ return m_PrevProjectionMatrix; }
	const Mat44f&		GetPrevViewMatrix						() const							{ return m_PrevViewMatrix; }
	const Mat44f&		GetPrevViewProjectionMatrix				() const							{ return m_PrevViewProjectionMatrix; }
	const Mat44f&		GetShadowViewMatrix						() const							{ return m_ShadowViewMatrix; }
	const Mat44f&		GetShadowProjectionMatrix				() const							{ return m_ShadowProjectionMatrix; }
	const Mat44f		GetShadowViewProjMatrix					() const							{ return m_ShadowProjectionMatrix * m_ShadowViewMatrix; };
	const Mat44f		GetShadowWorldViewProjMatrix			() const							{ return m_ShadowProjectionMatrix * m_ShadowViewMatrix * m_WorldMatrix; };
	const Mat44f		GetShadowWorldViewMatrix				() const							{ return m_ShadowViewMatrix * m_WorldMatrix; };
	const Vect3f&		GetShadowCameraEye						() const							{ return m_ShadowCameraEye; }
	const Mat44f&		GetWorldInverseMatrix					() const							{ return m_WorldInverseMatrix; }
	const Mat44f&		GetViewInverseMatrix					() const							{ return m_ViewInverseMatrix; }
	const Mat44f&		GetProjInverseMatrix					() const							{ return m_ProjInverseMatrix; }
	const Vect3f&		GetCameraEye							() const							{ return m_CameraEye; }
	Vect2f				GetLightShadowLinNearFar				() const							{ return m_LightShadowLinNearFar; }
	Vect2f				GetTextureDim							() const							{ return m_TextureDim; }

	void				SetLightShadowLinNearFar				(const Vect2f lightShadowLinNearFar){ m_LightShadowLinNearFar = lightShadowLinNearFar; }
	void				SetWorldMatrix							( const Mat44f &Matrix )			{ m_WorldMatrix = Matrix; }
	void				SetProjectionMatrix						( const Mat44f &Matrix )			{ m_ProjectionMatrix = Matrix; }
	void				SetViewMatrix							( const Mat44f &Matrix )			{ m_ViewMatrix = Matrix; }
	void				SetViewProjectionMatrix					( const Mat44f &Matrix )			{ m_ViewProjectionMatrix = Matrix; }
	void				SetLightViewMatrix						( const Mat44f &Matrix )			{ m_ShadowViewMatrix = Matrix; }
	void				SetShadowProjectionMatrix				( const Mat44f &Matrix )			{ m_ShadowProjectionMatrix = Matrix; }
	void				SetShadowViewMatrix						( const Mat44f &Matrix )			{ m_ShadowViewMatrix = Matrix; }
	void				SetWorldInverseMatrix					( const Mat44f &Matrix )			{ m_WorldInverseMatrix = Matrix; }
	void				SetViewInverseMatrix					( const Mat44f &Matrix )			{ m_ViewInverseMatrix = Matrix; }
	void				SetProjInverseMatrix					( const Mat44f &Matrix )			{ m_ProjInverseMatrix = Matrix; }
	void				SetCameraEye							( const Vect3f &Eye )				{ m_CameraEye = Eye; }
	void				SetShadowCameraEye						( const Vect3f &ShadowEye )			{ m_ShadowCameraEye = ShadowEye; }
	void				SetTextureDim							( const Vect2f &textureDim )		{ m_TextureDim = textureDim; }

private:
	
	typedef std::map<uint16, std::string>	TDefaultTechniqueEffectMap;
	TDefaultTechniqueEffectMap				m_DefaultTechniqueEffectMap;

	std::string								m_Filename;

	
	Mat44f									m_PrevProjectionMatrix, m_PrevViewMatrix, m_PrevViewProjectionMatrix;
	Mat44f									m_WorldMatrix, m_ProjectionMatrix, m_ViewMatrix, m_ViewProjectionMatrix;
	Mat44f									m_WorldInverseMatrix, m_ProjInverseMatrix, m_ViewInverseMatrix;
	Mat44f									m_ShadowViewMatrix, m_ShadowProjectionMatrix;
	Vect3f									m_ShadowCameraEye;
	Vect3f									m_CameraEye;
	Vect2f									m_LightShadowLinNearFar;
	Vect2f									m_TextureDim;

	CMapManager<CEffect>					m_Effects;
	std::vector<std::string>				m_EffectsNames;

	CEffectTechnique						*m_StaticMeshTechnique;
	CEffectTechnique						*m_AnimatedModelTechnique;
};

#endif
