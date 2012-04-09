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

#include "PostProcess\BloomPostProcess.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CBloomPostProcessCommand::CBloomPostProcessCommand(CXMLTreeNode &Node)
{
	SBloomPostProcessSettings settings;

	settings.m_BloomThreshold = Node.GetFloatProperty("BloomThreshold", 0.25f);
	settings.m_BlurAmount = Node.GetFloatProperty("BlurAmount", 4.0f);
	settings.m_BloomIntensity = Node.GetFloatProperty("BloomIntensity", 1.25f);
	settings.m_BaseIntensity = Node.GetFloatProperty("BaseIntensity", 1.0f);
	settings.m_BloomSaturation = Node.GetFloatProperty("BloomSaturation", 1.0f);
	settings.m_BloomSaturation = Node.GetFloatProperty("BaseSaturation", 1.0f);
	
	m_Bloom = new CBloomPostProcess(settings);
	m_Bloom->Init();
}

CBloomPostProcessCommand::~CBloomPostProcessCommand()
{
	CHECKED_DELETE(m_Bloom);
}

void CBloomPostProcessCommand::Execute(CRenderManager &RM)
{
	assert(m_Bloom);

	m_Bloom->Render();
}
