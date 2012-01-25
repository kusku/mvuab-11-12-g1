#include "AnimatedInstanceModel.h"
#include "AnimatedCoreModel.h"
#include "AnimatedModelManager.h"
#include "RenderManager.h"
#include "Vertexs\RenderableVertexs.h"
#include "Vertexs\IndexedVertexs.h"
#include "Vertexs\VertexType.h"
#include "Math\Matrix44.h"
#include "Base.h"
#include "Core.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Textures\Texture.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimatedInstanceModel::CAnimatedInstanceModel()
	: m_CalModel(NULL)
	, m_AnimatedCoreModel(NULL)
	, m_pVB(NULL)
    , m_pIB(NULL)

{
}

CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
	Destroy();
}

void CAnimatedInstanceModel::Destroy()
{
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

	m_AnimatedCoreModel->LoadVertexBuffer(m_CalModel);
}

void CAnimatedInstanceModel::Update(float elapsedTime)
{
	m_CalModel->update(elapsedTime);
}

void CAnimatedInstanceModel::Render(CRenderManager *RM)
{
	Mat44f mat, rotYaw, rotPitch, rotRoll;

	mat.SetIdentity();
	/*rotYaw.SetIdentity();
	rotPitch.SetIdentity();
	rotRoll.SetIdentity();

	mat.Translate( GetPosition() );
		
	rotPitch.SetRotByAngleX( mathUtils::Deg2Rad<float>(GetPitch()) );
	rotYaw.SetRotByAngleY( mathUtils::Deg2Rad<float>(GetYaw()) );
	rotRoll.SetRotByAngleZ( mathUtils::Deg2Rad<float>(GetRoll()) );
		
	mat = mat * rotYaw * rotPitch * rotRoll;*/
		
	RM->SetTransform(mat);

	RenderModelBySoftware(RM);
}

void CAnimatedInstanceModel::Render(CRenderManager *RM, CEffectTechnique* technique)
{
	RenderModelByHardware(RM, technique);
}

void CAnimatedInstanceModel::RenderModelByHardware(CRenderManager* RM, CEffectTechnique* technique)
{
	CEffectManager* l_EffectManager = CORE->GetEffectManager();
	CEffectTechnique* l_EffectTechnique = technique;

	if(l_EffectTechnique==NULL)
	{
		return;
	}

	l_EffectManager->SetWorldMatrix(GetTransform());

	CEffect* m_Effect= l_EffectTechnique->GetEffect();
	
	if(m_Effect==NULL)
	{
		return;
	}
	
	LPD3DXEFFECT l_Effect=m_Effect->GetD3DEffect();
	
	if(l_Effect)
	{
		l_EffectTechnique->BeginRender();
		CalHardwareModel *l_CalHardwareModel=m_AnimatedCoreModel->GetCalHardwareModel();
		D3DXMATRIX transformation[MAXBONES];

		for(int hardwareMeshId=0;hardwareMeshId<l_CalHardwareModel->getHardwareMeshCount(); hardwareMeshId++)
		{
			l_CalHardwareModel->selectHardwareMesh(hardwareMeshId);

			for(int boneId = 0; boneId < l_CalHardwareModel->getBoneCount(); boneId++)
			{
				D3DXMatrixRotationQuaternion(&transformation[boneId],(CONST D3DXQUATERNION*)&l_CalHardwareModel->getRotationBoneSpace(boneId, m_CalModel->getSkeleton()));
				CalVector translationBoneSpace = l_CalHardwareModel->getTranslationBoneSpace(boneId,m_CalModel->getSkeleton());
				transformation[boneId]._14 = translationBoneSpace.x;
				transformation[boneId]._24 = translationBoneSpace.y;
				transformation[boneId]._34 = translationBoneSpace.z;
			}
		
			float l_Matrix[MAXBONES*3*4];

			for(int b=0;b<l_CalHardwareModel->getBoneCount();++b)
			{
				memcpy(&l_Matrix[b*3*4], &transformation[b], sizeof(float)*3*4);
			}

			l_Effect->SetFloatArray(m_Effect->GetBonesMatrix(), (float *)l_Matrix,(l_CalHardwareModel->getBoneCount())*3*4);
			m_AnimatedCoreModel->GetTextureById(0)->Activate(0);
			//m_NormalTextureList[0]->Activate(1);
	
			m_AnimatedCoreModel->GetRenderableVertexs()->Render(RM, l_EffectTechnique, l_CalHardwareModel->getBaseVertexIndex(), 
				0, l_CalHardwareModel->getVertexCount(), l_CalHardwareModel->getStartIndex(),l_CalHardwareModel->getFaceCount());
		}
	}
}

void CAnimatedInstanceModel::RenderModelBySoftware(CRenderManager *RM)
{
	// get the renderer of the model
	uint32	l_VBCursor;
	uint32	l_IBCursor;
	CalRenderer *l_pCalRenderer;
	l_pCalRenderer = m_CalModel->getRenderer();

	LPDIRECT3DDEVICE9 Device = RM->GetDevice();

	// begin the rendering loop
	if(!l_pCalRenderer->beginRendering()) return;

	l_VBCursor=0;
	l_IBCursor=0;

	DWORD l_dwVBLockFlags=D3DLOCK_NOOVERWRITE;
	DWORD l_dwIBLockFlags=D3DLOCK_NOOVERWRITE;
	
	Device->SetStreamSource( 0, m_pVB, 0,  sizeof(TNORMALTEXTURE1_VERTEX) );
	Device->SetFVF(TNORMALTEXTURE1_VERTEX::GetFVF());
	Device->SetIndices(m_pIB);
	      
	// get the number of meshes
	uint32 l_MeshCount;
	l_MeshCount=l_pCalRenderer->getMeshCount();

	// render all meshes of the model
	uint32 l_MeshId;
	for(l_MeshId=0;l_MeshId<l_MeshCount; ++l_MeshId)
	{
		// get the number of submeshes
		uint32 l_SubmeshCount;
		l_SubmeshCount=l_pCalRenderer->getSubmeshCount(l_MeshId);

		m_AnimatedCoreModel->GetTextureById(l_MeshId)->Activate(0);

		// render all submeshes of the mesh
		uint32 l_SubmeshId;
		for(l_SubmeshId = 0; l_SubmeshId < l_SubmeshCount; ++l_SubmeshId)
		{
			// select mesh and submesh for further data access
			if(l_pCalRenderer->selectMeshSubmesh(l_MeshId, l_SubmeshId))
			{
				// Get vertexbuffer from the model
				TNORMALTEXTURE1_VERTEX *l_pVertices;

				m_pVB->Lock(l_VBCursor*sizeof(TNORMALTEXTURE1_VERTEX), l_pCalRenderer->getVertexCount()*sizeof(TNORMALTEXTURE1_VERTEX), (void**)&l_pVertices, l_dwVBLockFlags);

				uint32 l_VertexCount = l_pCalRenderer->getVerticesNormalsAndTexCoords(&l_pVertices->x);
				m_pVB->Unlock();
			  
				CalIndex *l_MeshFaces;
				uint32 l_FaceCount;

				m_pIB->Lock(l_IBCursor* 3*sizeof(CalIndex), l_pCalRenderer->getFaceCount()*3* sizeof(CalIndex), (void**)&l_MeshFaces,l_dwIBLockFlags);

				l_FaceCount = l_pCalRenderer->getFaces(l_MeshFaces);
				m_pIB->Unlock();


				Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	          
				Device->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					l_VBCursor,
					0,
					l_VertexCount,
					l_IBCursor*3,
					l_FaceCount
					);

				l_VBCursor+=l_VertexCount;
				l_IBCursor+=l_FaceCount;

				l_dwIBLockFlags=D3DLOCK_NOOVERWRITE;
				l_dwVBLockFlags=D3DLOCK_NOOVERWRITE;

			}
		}
	}
	// end the rendering
	l_pCalRenderer->endRendering();
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