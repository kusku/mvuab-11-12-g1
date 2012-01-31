#include "SetPoolRenderableObjectsTechniqueCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "RenderableObjects\PoolRenderableObjectTechnique.h"
#include "Core.h"
#include "Base.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Utils\TemplatedVectorMapManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CSetPoolRenderableObjectsTechniqueCommand::CSetPoolRenderableObjectsTechniqueCommand()
	: m_PoolRenderableObjectTechnique(NULL)
{

}

CSetPoolRenderableObjectsTechniqueCommand::~CSetPoolRenderableObjectsTechniqueCommand()
{

}

CSetPoolRenderableObjectsTechniqueCommand::CSetPoolRenderableObjectsTechniqueCommand(CXMLTreeNode &Node)
{
	//<set_pool_renderable_objects_technique pool="generate_deferred_shading_pool_renderable_object_technique"/>
	std::string poolName = Node.GetPszProperty("pool", "");

	m_PoolRenderableObjectTechnique = CORE->GetROTManager()->GetPoolRenderableObjectTechniques().GetResource(poolName);
}

void CSetPoolRenderableObjectsTechniqueCommand::Execute(CRenderManager &RM)
{
	m_PoolRenderableObjectTechnique->Apply();
}
