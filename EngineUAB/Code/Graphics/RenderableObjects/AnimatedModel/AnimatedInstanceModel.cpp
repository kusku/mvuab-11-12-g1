#include "AnimatedInstanceModel.h"
#include "AnimatedCoreModel.h"
#include "AnimatedModelManager.h"
#include "RenderManager.h"
#include "Vertexs\RenderableVertexs.h"
#include "Vertexs\IndexedVertexs.h"
#include "Vertexs\VertexType.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimatedInstanceModel::CAnimatedInstanceModel()
	: m_CalModel(NULL)
	, m_AnimatedCoreModel(NULL)
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
}

void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel)
{
	m_AnimatedCoreModel = AnimatedCoreModel;
	CalCoreModel *l_CalCoreModel = m_AnimatedCoreModel->GetCoreModel();
	m_CalModel = new CalModel( l_CalCoreModel );

	uint16 l_CoreMeshCount = l_CalCoreModel->getCoreMeshCount();
	for( uint16 meshId = 0; meshId < l_CoreMeshCount; ++meshId )
	{
		m_CalModel->attachMesh( meshId );
	}
	
	if(l_CalCoreModel->getCoreAnimationCount() > 1)
	{
		ExecuteAction(0, 0.0f);
	}
	else
	{
		BlendCycle(0, 0.0f);
	}

	CalculateNumVtxsIdxs();
	LoadTextures();
}

void CAnimatedInstanceModel::Update(float elapsedTime)
{
	m_CalModel->update(elapsedTime);
}


void CAnimatedInstanceModel::Render(CRenderManager *RM)
{
	Mat44f mat, rotYaw, rotPitch, rotRoll;

	mat.SetIdentity();
	rotYaw.SetIdentity();
	rotPitch.SetIdentity();
	rotRoll.SetIdentity();

	mat.Translate( GetPosition() );
		
	rotPitch.SetRotByAngleX( mathUtils::Deg2Rad<float>(GetPitch()) );
	rotYaw.SetRotByAngleY( mathUtils::Deg2Rad<float>(GetYaw()) );
	rotRoll.SetRotByAngleZ( mathUtils::Deg2Rad<float>(GetRoll()) );
		
	mat = mat * rotYaw * rotPitch * rotRoll;
		
	RM->SetTransform(mat);

	RenderModelBySoftware(RM);
}

void CAnimatedInstanceModel::RenderModelBySoftware(CRenderManager *RM)
{
	LPDIRECT3DDEVICE9 Device = RM->GetDevice();
	CalRenderer *l_CalRenderer;
	l_CalRenderer = m_CalModel->getRenderer();

	if(!l_CalRenderer->beginRendering()) return;

	uint16 meshCount = l_CalRenderer->getMeshCount();

	for(uint16 meshId = 0; meshId < meshCount; ++meshId)
	{
		// render all submeshes of the mesh
		uint16 submeshCount = l_CalRenderer->getSubmeshCount(meshId);
		for(uint16 submeshId = 0; submeshId < submeshCount; ++submeshId)
		{
			m_TextureVector[meshId]->Activate(0);

			 // select mesh and submesh for further data access
			if(l_CalRenderer->selectMeshSubmesh(meshId, submeshId))
			{
				uint16 l_VtxCount = l_CalRenderer->getVertexCount();
				uint16 l_IdxCount = l_CalRenderer->getFaceCount()*3;

				TNORMALTEXTURE1_VERTEX* l_Vtxs = new TNORMALTEXTURE1_VERTEX[l_VtxCount];
				CalIndex *l_Idxs = new CalIndex[l_IdxCount];

				l_CalRenderer->getVerticesNormalsAndTexCoords(&l_Vtxs->x);
				l_CalRenderer->getFaces(l_Idxs);

				CRenderableVertexs* l_RV = new CIndexedVertexs<TNORMALTEXTURE1_VERTEX>(RM, l_Vtxs, l_Idxs, l_VtxCount, l_IdxCount);
				l_RV->Render(RM);

				CHECKED_DELETE(l_RV);
				CHECKED_DELETE_ARRAY(l_Vtxs);
				CHECKED_DELETE_ARRAY(l_Idxs);
			}
		}
	}

	l_CalRenderer->endRendering();
}

void CAnimatedInstanceModel::ExecuteAction(uint32 Id, float Time)
{
	m_CalModel->getMixer()->executeAction(Id, 0, Time);
}

void CAnimatedInstanceModel::BlendCycle(uint32 Id, float Time)
{
	m_CalModel->getMixer()->blendCycle(Id, 1.0, Time);
}

void CAnimatedInstanceModel::ClearCycle(float Time)
{
	m_CalModel->getMixer()->clearCycle(0, Time);
}

void CAnimatedInstanceModel::CalculateNumVtxsIdxs()
{	
	m_NumVtxs = 0;
	m_NumFaces = 0;

	//Calcula el nombre de vértices y caras que tiene el modelo animado
	CalRenderer *l_Renderer = m_CalModel->getRenderer();
	uint16 l_MeshCount = l_Renderer->getMeshCount();
	for(uint16 i=0; i < l_MeshCount; ++i)
	{
		CalMesh *l_Mesh = m_CalModel->getMesh(i);

		uint16 l_SubmeshCount = l_Mesh->getSubmeshCount();
		for(uint16 j=0; j < l_SubmeshCount; ++j)
		{
			CalSubmesh *l_SubMesh = l_Mesh->getSubmesh(j);

			m_NumVtxs += l_SubMesh->getVertexCount();
			m_NumFaces += l_SubMesh->getFaceCount();
		}
	}

	assert(m_NumVtxs > 0 && m_NumFaces > 0);
}

void CAnimatedInstanceModel::LoadTextures()
{
	size_t l_TexCount = m_AnimatedCoreModel->GetNumTextures();
	for(size_t i = 0; i < l_TexCount; ++i)
	{
		std::string l_Path = m_AnimatedCoreModel->GetTextureName(i);

		CTexture* l_Texture = CORE->GetTextureManager()->GetTexture(l_Path);
		m_TextureVector.push_back(l_Texture);

		l_Texture = NULL;
	}
}