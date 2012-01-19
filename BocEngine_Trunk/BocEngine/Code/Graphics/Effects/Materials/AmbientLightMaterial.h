#pragma once

#ifndef AMBIENT_LIGHT_MATERIAL
#define AMBIENT_LIGHT_MATERIAL

#include "Effects\Effect.h"
#include "Utils\Types.h"
#include "Math\Color.h"
#include "Math\Matrix44.h"

class CEffectParameter;

class CAmbientLightMaterial : public CEffect
{
public:
	CAmbientLightMaterial( CXMLTreeNode *XMLNode );
	~CAmbientLightMaterial();

	/**Sets**/
	bool	SetAmbientLightColor			(CColor color);
	bool	SetAmbientLightIntensity		(float ambientLightIntensity);
	bool	SetWorldViewProjectionMatrix	(Mat44f worldViewProjectionMatrix);

	/**Gets**/
	CColor	GetAmbientLightColor			() const	{return m_AmbientLightColor;}
	float	GetAmbientLightIntensity		() const	{return m_AmbientLightIntensity;}
	Mat44f	GetWorldViewProjectionMatrix	() const	{return m_WorldViewProjectionMatrix;}

protected:
	bool	LoadEffect	();

private:
	CColor m_AmbientLightColor;
	float m_AmbientLightIntensity;
	Mat44f m_WorldViewProjectionMatrix;

	CEffectParameter* m_AmbientLightColorParameter;
	CEffectParameter* m_AmbientLightIntensityParameter;
	CEffectParameter* m_WorldViewProjectionMatrixParameter;

public:
	static const uint32 m_MaterialID=AMBIENT_LIGHT;
};

#endif AMBIENT_LIGHT_MATERIAL

