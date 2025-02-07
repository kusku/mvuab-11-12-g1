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
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Textures\Texture.h"
#include "XML\XMLTreeNode.h"
#include "Stadistics\Stadistics.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Vertexs\VertexType.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

CAnimatedInstanceModel::CAnimatedInstanceModel()
	: m_CalModel				( NULL )
	, m_AnimatedCoreModel		( NULL )
	, m_pVB						( NULL )
    , m_pIB						( NULL )
	, m_CreatePhysics			( false )
	, m_CreateLogicBehaviour	( false )
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

void CAnimatedInstanceModel::ReadDataXML(CXMLTreeNode &Node)
{
	m_Name		= Node.GetPszProperty("name", "");
	m_Position	= Node.GetVect3fProperty("position", Vect3f(0.0f, 0.0f, 0.0f));
	m_fYaw		= Node.GetFloatProperty("yaw", 0.0f);
	m_fPitch	= Node.GetFloatProperty("pitch", 0.0f);
	m_fRoll		= Node.GetFloatProperty("roll", 0.0f);
	m_Scale		= Node.GetVect3fProperty("scale", Vect3f(1.0f, 1.0f, 1.0f));
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
	
	if(l_CalCoreModel->getCoreAnimationCount() > 0)
	{
		//ExecuteAction(0, 0.0f);
	/*}
	else
	{*/
		BlendCycle(0, 0.0f);
	}

	m_AnimatedCoreModel->LoadVertexBuffer(m_CalModel);
}

void CAnimatedInstanceModel::Update( float _ElapsedTime )
{
	m_CalModel->update( _ElapsedTime );
}

void CAnimatedInstanceModel::Render( CRenderManager *_RM )
{
	//Mat44f mat, trans, rot, rotPitch;
	//mat.SetIdentity();
	//trans.SetIdentity();
	//rot.SetIdentity();
	//rotPitch.SetIdentity();
	//trans.Translate(m_Position);
	//rot.SetRotByAngleY(m_fYaw);
	//rotPitch.SetRotByAngleZ(m_fPitch);
	
	////mat = trans * rot * rotPitch;

	//mat = mat * rot * rotPitch;
	//	
	//_RM->SetTransform(mat);

	RenderModelByHardware( _RM );
}

void CAnimatedInstanceModel::RenderModelByHardware(CRenderManager* RM)
{
	CEffectManager* l_EffectManager = CORE->GetEffectManager();
	std::string techName = CORE->GetROTManager()->GetRenderableObjectTechniqueNameByVertexType( CAL3D_HW_VERTEX_BT::GetVertexType() );
	CRenderableObjectTechniqueManager * l_ROTMgr = CORE->GetROTManager();
 	CEffectTechnique* l_EffectTechnique = CORE->GetROTManager()->GetResourcesMap()[techName].m_Value->GetEffectTechnique();

	//CEffectTechnique* l_EffectTechnique = GetRenderableObjectTechnique()->GetEffectTechnique();;

	if( l_EffectTechnique == NULL )
	{
		l_EffectTechnique = CORE->GetEffectManager()->GetAnimatedModelTechnique();
		if ( l_EffectTechnique == NULL) return;
	}

	l_EffectManager->SetWorldMatrix( GetTransform() );

	CEffect* m_Effect= l_EffectTechnique->GetEffect();
	
	if ( m_Effect == NULL )
	{
		return;
	}
	
	LPD3DXEFFECT l_Effect=m_Effect->GetD3DEffect();
	
	if(l_Effect)
	{
		l_EffectTechnique->BeginRender();
		CalHardwareModel *l_CalHardwareModel=m_AnimatedCoreModel->GetCalHardwareModel();
		D3DXMATRIX transformation[MAXBONES];

		int l_MeshCount = l_CalHardwareModel->getHardwareMeshCount();
		for(int hardwareMeshId=0;hardwareMeshId<l_MeshCount; ++hardwareMeshId)
		{
			l_CalHardwareModel->selectHardwareMesh(hardwareMeshId);

			for(int boneId = 0; boneId < l_CalHardwareModel->getBoneCount(); ++boneId)
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
			
			if(m_AnimatedCoreModel->GetNumTextures() != 1)
			{
				m_AnimatedCoreModel->GetTextureById(1)->Activate(1);
			}
			
			//m_NormalTextureList[0]->Activate(1);

			//CalBoundingBox bb = m_CalModel->getBoundingBox();
			//
			//bool render = CORE->GetRenderManager()->GetFrustum()->BoxVisible(bb);

			//if(render == false)
			//{
			//	return;
			//}

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

#if defined(_DEBUG)
		//Capture Info for Stadistics
		CStadistics *l_pStadistics = CORE->GetStadistics();

		l_pStadistics->AddDrawCall();
		l_pStadistics->AddVerticesInFrustum(l_VertexCount);
		l_pStadistics->AddTriangles( l_pCalRenderer->getFaceCount() );
#endif

			}
		}
	}
	// end the rendering
	l_pCalRenderer->endRendering();
}

void CAnimatedInstanceModel::ExecuteAction(uint32 Id, float Time)
{
	m_CalModel->getMixer()->executeAction( Id, 0, Time );
}

void CAnimatedInstanceModel::ExecuteAction(uint32 _Id, float _Time, bool _Autolock )
{
	m_CalModel->getMixer()->executeAction( _Id, 0, _Time, 1.f, _Autolock );
}

void CAnimatedInstanceModel::RemoveAction( uint32 _Id )
{
	m_CalModel->getMixer()->removeAction(_Id);
}

void CAnimatedInstanceModel::BlendCycle( uint32 Id, float Time )
{
	m_CalModel->getMixer()->blendCycle( Id, 1.0, Time);
}

void CAnimatedInstanceModel::ClearCycle( uint32 Id, float Time )
{
	m_CalModel->getMixer()->clearCycle( Id, Time);
}

float CAnimatedInstanceModel::GetCurrentAnimationDuration(const std::string &_Name)
{
	uint16 id = m_AnimatedCoreModel->GetAnimationIdByName(_Name);
	return m_AnimatedCoreModel->GetCoreModel()->getCoreAnimation(id)->getDuration();
}

unsigned int CAnimatedInstanceModel::GetCurrentAnimationTrack(const std::string &_Name)
{
	uint16 id = m_AnimatedCoreModel->GetAnimationIdByName(_Name);
	return m_AnimatedCoreModel->GetCoreModel()->getCoreAnimation(id)->getTrackCount();
}


bool CAnimatedInstanceModel::GetBonePosition( const std::string & _Bone, Vect3f & _Position )
{
	int l_iBoneIndex = m_AnimatedCoreModel->GetCoreModel()->getCoreSkeleton()->getCoreBoneId(_Bone);
	if ( l_iBoneIndex != -1 )
	{
		CalSkeleton* l_pSkeleton = m_CalModel->getSkeleton();
		CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneIndex);
		CalVector l_vTranslation = l_pBone->getTranslationAbsolute();

		_Position = Vect3f(l_vTranslation.x, l_vTranslation.y, l_vTranslation.z);

		return true;
	}

	return false;
}

bool CAnimatedInstanceModel::GetBoneRotation( const std::string & _Bone, Vect4f & _Rotation )
{
	int l_iBoneIndex = m_AnimatedCoreModel->GetCoreModel()->getCoreSkeleton()->getCoreBoneId(_Bone);
	if ( l_iBoneIndex != -1 )
	{
		CalSkeleton* l_pSkeleton = m_CalModel->getSkeleton();
		CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneIndex);
		CalQuaternion l_vRotation = l_pBone->getRotationAbsolute();

		_Rotation = Vect4f(l_vRotation.x, l_vRotation.y, l_vRotation.z, l_vRotation.w);

		return true;
	}
	
	return false;
}