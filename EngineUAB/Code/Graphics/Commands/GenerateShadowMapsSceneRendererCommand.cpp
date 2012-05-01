#include <vector>

#include "GenerateShadowMapsSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Lights\LightManager.h"
#include "Core.h"
#include "Base.h"
#include "Lights\Light.h"
#include "Utils\Types.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &Node)
	: CStagedTexturedRendererCommand ( Node )
{
}

CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CRenderManager &RM)
{
	std::vector<CLight*> lights = CORE->GetLightManager()->GetResourcesVector();
	uint32 numLights = lights.size();

	for(uint32 i = 0; i < numLights; i++)
	{
		lights[i]->GenerateShadowMap(&RM);
	}
}
