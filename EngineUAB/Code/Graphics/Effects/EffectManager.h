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
#include <map>

class CEffectManager : public CMapManager<CEffectTechnique>
{
public:
	CEffectManager();
	virtual ~CEffectManager();

	void		Load		( const std::string &Filename );
	void		Reload		();
	void		CleanUp		();

	void		ActivateCamera		( const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix, const Vect3f &CameraEye );

	std::string			GetTechniqueEffectNameByVertexDefault		( uint16 VertexType );
	size_t				GetMaxLights		() const;

	CEffect*			GetEffect				( const std::string &Name )			{ return m_Effects.GetResource(Name); }
	CEffectTechnique*	GetEffectTechnique		( const std::string &Name )			{ return GetResource(Name); }

	void		SetStaticMeshTechnique		( CEffectTechnique *StaticMeshTechnique )		{ m_StaticMeshTechnique = StaticMeshTechnique; }
	void		SetAnimatedModelTechnique	( CEffectTechnique *AnimatedModelTechnique )	{ m_AnimatedModelTechnique = AnimatedModelTechnique ; }

	CEffectTechnique*	GetStaticMeshTechnique		() const		{ return m_StaticMeshTechnique; }
	CEffectTechnique*	GetAnimatedModelTechnique	() const		{ return m_AnimatedModelTechnique; }

	const Mat44f&			GetWorldMatrix				() const	{ return m_WorldMatrix; }
	const Mat44f&			GetProjectionMatrix			() const	{ return m_ProjectionMatrix; }
	const Mat44f&			GetViewMatrix				() const	{ return m_ViewMatrix; }
	const Mat44f&			GetViewProjectionMatrix		() const	{ return m_ViewProjectionMatrix; }
	const Mat44f&			GetLightViewMatrix			() const	{ return m_LightViewMatrix; }
	const Mat44f&			GetShadowProjectionMatrix	() const	{ return m_ShadowProjectionMatrix; }
	const Vect3f&			GetCameraEye				() const	{ return m_CameraEye; }

	void			SetWorldMatrix					( const Mat44f &Matrix )	{ m_WorldMatrix = Matrix; }
	void			SetProjectionMatrix				( const Mat44f &Matrix )	{ m_ProjectionMatrix = Matrix; }
	void			SetViewMatrix					( const Mat44f &Matrix )	{ m_ViewMatrix = Matrix; }
	void			SetViewProjectionMatrix			( const Mat44f &Matrix )	{ m_ViewProjectionMatrix = Matrix; }
	void			SetLightViewMatrix				( const Mat44f &Matrix )	{ m_LightViewMatrix = Matrix; }
	void			SetShadowProjectionMatrix		( const Mat44f &Matrix )	{ m_ShadowProjectionMatrix = Matrix; }
	void			SetCameraEye					( const Vect3f &Eye )		{ m_CameraEye = Eye; }

private:
	typedef std::map<uint16, std::string>	TDefaultTechniqueEffectMap;
	TDefaultTechniqueEffectMap				m_DefaultTechniqueEffectMap;

	std::string								m_Filename;

	Mat44f									m_WorldMatrix, m_ProjectionMatrix, m_ViewMatrix, m_ViewProjectionMatrix;
	Mat44f									m_LightViewMatrix, m_ShadowProjectionMatrix;
	Vect3f									m_CameraEye;

	CMapManager<CEffect>					m_Effects;

	CEffectTechnique						*m_StaticMeshTechnique;
	CEffectTechnique						*m_AnimatedModelTechnique;
};

#endif
