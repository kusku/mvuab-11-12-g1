#include "SkySphere.h"
#include "Math\Color.h"
#include "Effects\EffectTechnique.h"
#include "XML\XMLTreeNode.h"
#include "Textures\TextureCube.h"
#include "Core.h"
#include "Effects\EffectManager.h"
#include "Cameras\Camera.h"
#include "Math\Vector3.h"
#include "Utils\Timer.h"
#include "Textures\Texture.h"
#include "RenderManager.h"
#include <d3d9.h>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSkySphere::CSkySphere()
	: m_SphereMesh(1, colRED)
	, m_SkySphereTechnique(NULL)
	, m_SkyTexture(NULL)
	, m_SkyTextureParameter(NULL)
	, m_World(m44fIDENTITY)
	, m_Clouds(false)
	, m_CloudTimer(0.0f)
	, m_CloudTimerParameter(NULL)
	, m_SkyCloudTextureParameter(NULL)
	, m_SkyCloudTexture(NULL)
	, m_CloudAnimSpeed(0.01f)
{
	m_World.Scale(1000, 900, 1000);
}

CSkySphere::~CSkySphere()
{
	CHECKED_DELETE(m_SkyTexture);
	CHECKED_DELETE(m_SkyCloudTexture);
}

bool CSkySphere::Load(CXMLTreeNode &XMLNode)
{
	//Clean up before loading
	CHECKED_DELETE(m_SkyTexture);
	m_SkySphereTechnique = NULL;
	m_SkyTextureParameter = NULL;

	m_Clouds = XMLNode.GetBoolProperty("Clouds", false, false);

	std::string skyTextureFile = XMLNode.GetPszProperty("SkyTexture", "", !m_Clouds);
	std::string skyCloudTextureFile = XMLNode.GetPszProperty("SkyCloudTexture", "", m_Clouds);
	std::string skyEffectTechnique = XMLNode.GetPszProperty("SkyEffectTechnique", "");
	Vect3f scale = XMLNode.GetVect3fProperty("Scale", Vect3f(1000, 900, 1000));

	m_SkyTexture = new CTextureCube();
	m_SkyCloudTexture = new CTexture();

	m_World = m44fIDENTITY;
	m_World.Scale(scale.x, scale.y, scale.z);

	m_SkySphereTechnique = CORE->GetEffectManager()->GetEffectTechnique(skyEffectTechnique);

	if(m_SkySphereTechnique == NULL)
	{
		return false;
	}

	if(m_Clouds)
	{
		m_SkySphereTechnique->GetEffect()->GetParameterBySemantic("CLOUD_TIMER", m_CloudTimerParameter);
		m_SkySphereTechnique->GetEffect()->GetParameterBySemantic("SKY_CLOUD_TEXTURE", m_SkyCloudTextureParameter);

		if(!m_SkyCloudTexture->Load(skyCloudTextureFile))
		{
			CHECKED_DELETE(m_SkyCloudTexture);
			return false;
		}

		assert(m_CloudTimerParameter);
		assert(m_SkyCloudTextureParameter);
	}
	else
	{
		if(!m_SkyTexture->Load(skyTextureFile))
		{
			CHECKED_DELETE(m_SkyTexture);
			return false;
		}

		m_SkySphereTechnique->GetEffect()->GetParameterBySemantic("SKY_TEXTURECUBE", m_SkyTextureParameter);

		if(m_SkyTextureParameter == NULL)
		{
			CHECKED_DELETE(m_SkyTexture);
			return false;
		}
	}

	return true;
}

bool CSkySphere::Render()
{
	if(m_SkySphereTechnique == NULL)
	{
		return false;
	}

	if(m_Clouds)
	{
		m_CloudTimer += (m_CloudAnimSpeed * CORE->GetTimer()->GetElapsedTime());
		
		if (m_CloudTimer > 1)
		{
			m_CloudTimer = 0;
		}

		//Set Cloud Texture
		m_SkySphereTechnique->GetEffect()->GetD3DEffect()->SetTexture(m_SkyCloudTextureParameter, m_SkyCloudTexture->GetDXTexture());

		//Set Timer
		m_SkySphereTechnique->GetEffect()->GetD3DEffect()->SetFloat(m_CloudTimerParameter, m_CloudTimer);
	}
	else
	{
		//Set Sky Texture
		m_SkySphereTechnique->GetEffect()->GetD3DEffect()->SetTexture(m_SkyTextureParameter, m_SkyTexture->GetDXCubeTexture());
	}

	//Set World Matrix
	Mat44f pos(m44fIDENTITY);
	Vect3f newPos = CORE->GetCamera()->GetPosition();
	newPos.y = -85;
	pos.Translate(newPos);

	CORE->GetEffectManager()->SetWorldMatrix(pos * m_World);

	CORE->GetRenderManager()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	m_SphereMesh.Render(CORE->GetRenderManager(), m_SkySphereTechnique);

	CORE->GetRenderManager()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);

	return true;
}