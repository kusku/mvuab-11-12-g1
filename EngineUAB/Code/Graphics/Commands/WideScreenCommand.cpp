#include <assert.h>
#include <string>

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
#include "WideScreenCommand.h"
#include "Math\Vector2.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------
CWideScreenCommand::CWideScreenCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{

	LPDIRECT3DDEVICE9 dx9Device = CORE->GetRenderManager()->GetDevice();
	D3DXCreateSprite(dx9Device, &m_SpriteBatch);

	assert(m_SpriteBatch);

	m_DiffuseRT = NULL;

	std::string diffuseRTName = _Node.GetPszISOProperty("DiffuseRT", "");
	
	m_DiffuseRT = CORE->GetTextureManager()->GetResource(diffuseRTName);

	assert(m_DiffuseRT);

	if(CORE->GetRenderManager()->GetScreenSizeOriginal() == CORE->GetRenderManager()->GettWindowSize())
	{
		m_DiffusePosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		Vect2i screenSize = CORE->GetRenderManager()->GetScreenSize();
		Vect2i windowSize = CORE->GetRenderManager()->GetWindowRectSize();

		int diffHeight = (windowSize.y - screenSize.y) / 2;

		m_DiffusePosition.x = 0.0f;
		m_DiffusePosition.y = (float)diffHeight;
		m_DiffusePosition.z = 0.0f;
	}

	m_DiffuseCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

CWideScreenCommand::~CWideScreenCommand()
{
}

// --------------------------------------------
//			   MÈTODES PRINCIPALS
// --------------------------------------------
void CWideScreenCommand::Execute( CRenderManager &_RM )
{
	CORE->GetRenderManager()->ClearTarget(colBLACK);
	
	//CORE->GetRenderManager()->GetDevice()->SetRenderState(D3DRS_ZENABLE, false);

	CColor color = colWHITE;

	m_SpriteBatch->Begin(0);

	HRESULT hr = m_SpriteBatch->Draw(m_DiffuseRT->GetDXTexture(), NULL, NULL, &m_DiffusePosition, (D3DCOLOR)color.GetUint32Argb());

	assert(hr == D3D_OK);

	m_SpriteBatch->End();

	//CORE->GetRenderManager()->GetDevice()->SetRenderState(D3DRS_ZENABLE, true);
}
