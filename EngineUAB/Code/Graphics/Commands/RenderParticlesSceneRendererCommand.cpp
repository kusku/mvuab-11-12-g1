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
	LPDIRECT3DDEVICE9 Device = RM.GetDevice();
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CORE->GetBillboardManager()->Render(RM);
	CORE->GetParticleManager()->Render(RM);

	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
