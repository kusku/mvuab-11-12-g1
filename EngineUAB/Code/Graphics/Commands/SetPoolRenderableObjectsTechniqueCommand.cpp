#include "SetPoolRenderableObjectsTechniqueCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "RenderableObjects\PoolRenderableObjectTechnique.h"
#include "Core.h"
#include "Base.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "RenderableObjects\PoolRenderableObjectTechnique.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


CSetPoolRenderableObjectsTechniqueCommand::CSetPoolRenderableObjectsTechniqueCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
	, m_PoolRenderableObjectTechnique(NULL)
{
	//<set_pool_renderable_objects_technique pool="generate_deferred_shading_pool_renderable_object_technique"/>
	std::string poolName = Node.GetPszProperty("pool", "");

	CTemplatedVectorMapManager<CPoolRenderableObjectTechnique> &l_pools = CORE->GetROTManager()->GetPoolRenderableObjectTechniques();

	m_PoolRenderableObjectTechnique = l_pools.GetResource(poolName);
}

void CSetPoolRenderableObjectsTechniqueCommand::Execute(CRenderManager &RM)
{
	m_PoolRenderableObjectTechnique->Apply();
}
