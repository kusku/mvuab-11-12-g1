#include "RenderDebugSceneSceneRendererCommand.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\RenderableObjectsLayersManager.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"

CRenderDebugSceneSceneRendererCommand::CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &Node)
	: m_pRenderableObjectsManager(NULL)
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

		RM.DrawAxis(1.0f);
		RM.DrawGrid(100.0f, 100.0f, 30, colBLACK);

		//TODO: Falta hacer la llamada de renderizar la información de debug de los objetos
	}
}
