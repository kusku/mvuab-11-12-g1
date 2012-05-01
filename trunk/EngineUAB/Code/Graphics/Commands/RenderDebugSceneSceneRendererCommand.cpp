#include "RenderDebugSceneSceneRendererCommand.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CRenderDebugSceneSceneRendererCommand::CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &Node)
	: CSceneRendererCommand ( Node )
	, m_pRenderableObjectsManager(NULL)
{
	bool l_Active = Node.GetBoolProperty("active", false);
	SetActive( l_Active );

	std::string l_Layer = Node.GetPszProperty("layer", "");
	m_pRenderableObjectsManager = CORE->GetRenderableObjectsLayersManager()->GetResource(l_Layer);
}

void CRenderDebugSceneSceneRendererCommand::Execute(CRenderManager &RM)
{
	if( GetActive() )
	{
		Mat44f mat;
		mat.SetIdentity();
		RM.SetTransform(mat);

		//TODO: Falta hacer la llamada de renderizar la información de debug de los objetos
	}
}
