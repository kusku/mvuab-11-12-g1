#include "AnimatedInstanceModel.h"
#include "AnimatedCoreModel.h"
#include "../RenderManager.h"
#include "../Textures/Texture.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimatedInstanceModel::CAnimatedInstanceModel()
	: m_CalModel(NULL)
	, m_AnimatedCoreModel(NULL)
	, m_CurrentCycle(0)
	, m_CurrentAnimationId(0)
	, m_CurrentAnimationTime(0.f)
	, m_pVB(NULL)
	, m_pIB(NULL)
	, m_NumVtxs(0)
	, m_NumFaces(0)
{
}


CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
	Destroy();
}

void CAnimatedInstanceModel::Destroy()
{
	std::vector<CTexture*>::iterator l_It = m_TextureVector.begin();
	std::vector<CTexture*>::iterator l_End = m_TextureVector.end();
	for(; l_It != l_End; ++l_It)
	{
		CHECKED_DELETE( (*l_It) );
	}
	m_TextureVector.clear();

	CHECKED_DELETE(m_AnimatedCoreModel);
	CHECKED_DELETE(m_CalModel);

	CHECKED_RELEASE(m_pVB);
	CHECKED_RELEASE(m_pIB);
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel)
{
	CalCoreModel *l_CalCoreModel = AnimatedCoreModel->GetCoreModel();

	uint16 l_CoreMeshCount = l_CalCoreModel->getCoreMeshCount();
	for( uint16 meshId = 0; meshId < l_CoreMeshCount; ++meshId )
	{
		m_CalModel->attachMesh( meshId );
	}
}

void CAnimatedInstanceModel::InitD3D(CRenderManager *RM)
{
}

void CAnimatedInstanceModel::Update(float elapsedTime)
{
}

void CAnimatedInstanceModel::Render(CRenderManager *RM)
{
}

void CAnimatedInstanceModel::RenderModelBySoftware(CRenderManager *RM)
{
}

void CAnimatedInstanceModel::ExecuteAction(uint32 Id, float Time)
{
}

void CAnimatedInstanceModel::BlendCycle(uint32 Id, float Time)
{
}

void CAnimatedInstanceModel::ClearCycle(float Time)
{
}

bool CAnimatedInstanceModel::LoadVertexBuffer(CRenderManager *RM)
{
	return true;
}

void CAnimatedInstanceModel::LoadTextures()
{
}