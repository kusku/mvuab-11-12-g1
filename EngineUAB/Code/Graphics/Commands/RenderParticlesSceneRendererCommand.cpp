#include "RenderParticlesSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Billboard\BillboardManager.h"
#include "Particles\ParticlesSystemManager.h"
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
	RM.EnableAlphaBlend();

	CORE->GetBillboardManager()->Render(RM);
	CORE->GetParticleManager()->Render(RM);

	RM.DisableAlphaBlend();
}
