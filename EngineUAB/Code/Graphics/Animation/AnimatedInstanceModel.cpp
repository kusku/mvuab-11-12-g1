#include "AnimatedInstanceModel.h"
#include "AnimatedCoreModel.h"
#include "AnimatedModelManager.h"
#include "RenderManager.h"
#include "Vertexs\RenderableVertexs.h"
#include "Vertexs\IndexedVertexs.h"
#include "Vertexs/VertexType.h"
#include "Textures/Texture.h"
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

	InitD3D(CORE->GetRenderManager());
}

void CAnimatedInstanceModel::InitD3D(CRenderManager *RM)
{
	LoadVertexBuffer(RM);
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
	uint32 l_VBCursor = 0;
	uint32 l_IBCursor = 0;

	LPDIRECT3DDEVICE9 Device = RM->GetDevice();
	CalRenderer *l_CalRenderer;
	l_CalRenderer = m_CalModel->getRenderer();

	if(!l_CalRenderer->beginRendering()) return;

	Device->SetStreamSource( 0, m_pVB, 0,  sizeof(TNORMALTEXTURE1_VERTEX) );
	Device->SetFVF(TNORMALTEXTURE1_VERTEX::GetFVF());
	Device->SetIndices(m_pIB);

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
				/*TNORMALTEXTURE1_VERTEX *l_Vtxs;

				m_pVB->Lock(l_VBCursor*sizeof(TNORMALTEXTURE1_VERTEX), l_CalRenderer->getVertexCount()*sizeof(TNORMALTEXTURE1_VERTEX), (void**)&l_Vtxs, D3DLOCK_NOOVERWRITE);
				uint16 l_VtxCount = l_CalRenderer->getVerticesNormalsAndTexCoords(&l_Vtxs->x);
				m_pVB->Unlock();

				CalIndex *l_Idxs;

				m_pIB->Lock(l_IBCursor*3*sizeof(CalIndex), l_CalRenderer->getFaceCount()*3* sizeof(CalIndex), (void**)&l_Idxs, D3DLOCK_NOOVERWRITE);
				uint16 l_IdxCount = (uint16)l_CalRenderer->getFaces(l_Idxs);
				m_pIB->Unlock();

				Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

				Device->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					l_VBCursor,
					0,
					l_VtxCount,
					l_IBCursor*3,
					l_IdxCount
				);

				l_VBCursor+=l_VtxCount;
				l_IBCursor+=l_IdxCount;*/
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

bool CAnimatedInstanceModel::LoadVertexBuffer(CRenderManager *RM)
{
	LPDIRECT3DDEVICE9 Device = RM->GetDevice();
	
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
	////Crea el Vertex Buffer e Index Buffer
	//if(FAILED(Device->CreateVertexBuffer(m_NumVtxs*sizeof(TNORMALTEXTURE1_VERTEX), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, TNORMALTEXTURE1_VERTEX::GetFVF(), D3DPOOL_DEFAULT , &m_pVB, NULL )))
	//{
	//	return false;
	//}

	//// Create index buffer
	//if(sizeof(CalIndex)==2)
	//{
	//	if(FAILED(Device->CreateIndexBuffer(m_NumFaces*3*sizeof(CalIndex), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,D3DFMT_INDEX16, D3DPOOL_DEFAULT ,&m_pIB, NULL)))
	//	{
	//		return false;
	//	}
	//}
	//else
	//{
	//	if( FAILED(Device->CreateIndexBuffer(m_NumFaces*3*sizeof(CalIndex), D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,D3DFMT_INDEX32, D3DPOOL_DEFAULT ,&m_pIB, NULL)))
	//	{
	//		return false;
	//	}
	//}

	return true;
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