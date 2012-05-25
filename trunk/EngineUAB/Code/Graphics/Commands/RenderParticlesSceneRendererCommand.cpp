#include "RenderParticlesSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Billboard\BillboardManager.h"
#include "Particles\ParticleSystemManager.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------
CRenderParticlesSceneRendererCommand::CRenderParticlesSceneRendererCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CRenderParticlesSceneRendererCommand ::Execute( CRenderManager &_RM )
{	
	CORE->GetParticleSystemManager()->Render();
}
