#include <assert.h>
#include <string>

#include "LensFlareSceneRendererCommand.h"
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
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "Lights\DirectionalLight.h"

#include "PostProcess\LensFlare\LensFlarePostProcess.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------
CLensFlareSceneRemdererCommand::CLensFlareSceneRemdererCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{	
	m_LensFlare = new CLensFlarePostProcess();
	m_LensFlare->Init();

	m_LightIndex = _Node.GetIntProperty("light_index", 0, true);
}

CLensFlareSceneRemdererCommand::~CLensFlareSceneRemdererCommand()
{
	CHECKED_DELETE(m_LensFlare);
}

// --------------------------------------------
//			   MÈTODES PRINCIPALS
// --------------------------------------------
void CLensFlareSceneRemdererCommand::Execute( CRenderManager &_RM )
{
	assert(m_LensFlare);

	CDirectionalLight* light = (CDirectionalLight*)CORE->GetLightManager()->GetResourcesVector().at(m_LightIndex);
	
	m_LensFlare->Render(light->GetDirection());
}
