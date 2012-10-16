#include <assert.h>
#include <string>

#include "BloomPostProcessCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Logger\Logger.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Base.h"
#include "Core.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Vertexs\VertexType.h"
#include "Math\Matrix44.h"

#include "PostProcess\Bloom\BloomPostProcess.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------
CBloomPostProcessCommand::CBloomPostProcessCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{
	SBloomPostProcessSettings settings;

	settings.m_BloomThreshold	= _Node.GetFloatProperty("BloomThreshold", 0.25f);
	settings.m_BlurAmount		= _Node.GetFloatProperty("BlurAmount", 4.0f);
	settings.m_BloomIntensity	= _Node.GetFloatProperty("BloomIntensity", 1.25f);
	settings.m_BaseIntensity	= _Node.GetFloatProperty("BaseIntensity", 1.0f);
	settings.m_BloomSaturation	= _Node.GetFloatProperty("BloomSaturation", 1.0f);
	settings.m_BloomSaturation	= _Node.GetFloatProperty("BaseSaturation", 1.0f);
	
	m_Bloom = new CBloomPostProcess(settings);
	m_Bloom->Init();
}

CBloomPostProcessCommand::~CBloomPostProcessCommand()
{
	CHECKED_DELETE(m_Bloom);
}

// --------------------------------------------
//			   MÈTODES PRINCIPALS
// --------------------------------------------
void CBloomPostProcessCommand::Execute( CRenderManager &_RM )
{
	assert(m_Bloom);

	if(!CORE->GetConfig().bloom_enable)
	{
		return;
	}

	m_Bloom->Render();
}
