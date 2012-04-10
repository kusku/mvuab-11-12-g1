#include "RenderParticlesSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Billboard\BillboardManager.h"
#include "Particles\ParticleManager.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderParticlesSceneRendererCommand::CRenderParticlesSceneRendererCommand(CXMLTreeNode &Node)
{
}

void CRenderParticlesSceneRendererCommand ::Execute(CRenderManager &RM)
{	
	CORE->GetBillboardManager()->Render(RM);
	CORE->GetParticleManager()->Render(RM);
}
