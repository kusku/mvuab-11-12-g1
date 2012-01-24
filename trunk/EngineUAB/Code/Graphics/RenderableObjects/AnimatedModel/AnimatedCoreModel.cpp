
#include <assert.h>

#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"
#include "Vertexs\VertexType.h"
#include "GraphicsDefs.h"
#include "Vertexs\IndexedVertexs.h"
#include "Vertexs\RenderableVertexs.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimatedCoreModel::CAnimatedCoreModel()
	: m_CalCoreModel(NULL)
	, m_Name("")
	, m_Path("")
	, m_RenderableVertexs(NULL)
	, m_CalHardwareModel(NULL)
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
	m_TextureFilenameVector.clear();
	CHECKED_DELETE(m_CalCoreModel);
}

CalHardwareModel* CAnimatedCoreModel::GetCalHardwareModel() const
{
	return m_CalHardwareModel;
}

CRenderableVertexs* CAnimatedCoreModel::GetRenderableVertexs() const
{
	return m_RenderableVertexs;
}

void CAnimatedCoreModel::Load(const std::string &Path, const std::string &XMLFilename)
{
	CXMLTreeNode newFile;
	std::string filename = Path + XMLFilename;
	if (!newFile.LoadFile(filename.c_str()))
	{
		std::string msg_error = "CAnimatedCoreModel::Load->Error al intentar leer el archivo xml de animación de un core: " + filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}
	m_Path = Path;

	CXMLTreeNode l_AnimationCore = newFile["animated_model"];
	if( l_AnimationCore.Exists() )
	{
		m_Name = l_AnimationCore.GetPszProperty("name", "");
		m_CalCoreModel = new CalCoreModel(m_Name); //Init he CalCoreModel
		assert(m_CalCoreModel!=NULL);

		uint16 l_Count = l_AnimationCore.GetNumChildren();
		for( uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_AnimationCore(i).GetName();
			if( l_Type == "mesh" )
			{
				std::string l_MeshFilename = l_AnimationCore(i).GetPszProperty("filename", "");
				l_MeshFilename = m_Path + l_MeshFilename;
				LoadMesh( l_MeshFilename ); //Load Mesh
			}
			else if( l_Type == "skeleton" )
			{
				std::string l_SkeletonFilename = l_AnimationCore(i).GetPszProperty("filename", "");
				l_SkeletonFilename = m_Path + l_SkeletonFilename;
				LoadSkeleton( l_SkeletonFilename ); //Load Skeleton
			}
			else if( l_Type == "animation" )
			{
				std::string l_AnimationName = l_AnimationCore(i).GetPszProperty("name", "");
				std::string l_AnimationPath = l_AnimationCore(i).GetPszProperty("filename", "");
				l_AnimationPath = m_Path + l_AnimationPath;
				LoadAnimation( m_Name, l_AnimationPath ); //Load Animation
			}
			else if( l_Type == "texture" )
			{
				std::string l_Texture = l_AnimationCore(i).GetPszProperty("filename", "");
				l_Texture = m_Path + l_Texture;
				m_TextureFilenameVector.push_back(l_Texture); //Load texture string
			}
		}
	}
}

bool CAnimatedCoreModel::LoadVertexBuffer(CalModel *Model)
{
	m_NumVtxs = 0;
	m_NumFaces = 0;

	//Calcula el nombre de vértices y caras que tiene el modelo animado

	for(int i=0;i<m_CalCoreModel->getCoreMeshCount();++i)
	{
		CalCoreMesh *l_CoreMesh=m_CalCoreModel->getCoreMesh(i);
		for(int j=0;j<l_CoreMesh->getCoreSubmeshCount();++j)
		{
			m_NumVtxs+=l_CoreMesh->getCoreSubmesh(j)->getVertexCount();
			m_NumFaces+=l_CoreMesh->getCoreSubmesh(j)->getFaceCount();
		}
	}
	/*CalRenderer *l_Renderer = Model->getRenderer();
	uint16 l_MeshCount = l_Renderer->getMeshCount();
	for(uint16 i=0; i < l_MeshCount; ++i)
	{
		CalMesh *l_Mesh = Model->getMesh(i);

		uint16 l_SubmeshCount = l_Mesh->getSubmeshCount();
		for(uint16 j=0; j < l_SubmeshCount; ++j)
		{
			CalSubmesh *l_SubMesh = l_Mesh->getSubmesh(j);

			m_NumVtxs += l_SubMesh->getVertexCount();
			m_NumFaces += l_SubMesh->getFaceCount();
		}
	}*/

	assert(m_NumVtxs > 0 && m_NumFaces > 0);

	//CAL3D_HW_VERTEX_BT* pVertex;

	m_CalHardwareModel = new CalHardwareModel(m_CalCoreModel);

	CAL3D_HW_VERTEX_BT* l_Vtxs = new CAL3D_HW_VERTEX_BT[m_NumVtxs*2]; //Cogemos el doble de vértices necesarios porque al crear el model de hardware puede necesitar más vértices que el modelo por software
	
	unsigned short* l_Idxs = new unsigned short[m_NumFaces*3];
	
	m_CalHardwareModel->setVertexBuffer((char*) l_Vtxs, sizeof(CAL3D_HW_VERTEX_BT));
	m_CalHardwareModel->setWeightBuffer(((char*)l_Vtxs) + 12, sizeof(CAL3D_HW_VERTEX_BT));
	m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vtxs) + 28, sizeof(CAL3D_HW_VERTEX_BT));
	m_CalHardwareModel->setNormalBuffer(((char*)l_Vtxs)+44, sizeof(CAL3D_HW_VERTEX_BT));
	m_CalHardwareModel->setTextureCoordNum(1);
	m_CalHardwareModel->setTextureCoordBuffer(0,((char*)l_Vtxs)+92,	sizeof(CAL3D_HW_VERTEX_BT));
	m_CalHardwareModel->setIndexBuffer(l_Idxs);
	m_CalHardwareModel->load( 0, 0, MAXBONES);

	m_NumVtxs = m_CalHardwareModel->getTotalVertexCount();
	m_NumFaces = m_CalHardwareModel->getTotalFaceCount();
	
	CalcTangentsAndBinormals(l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces*3, sizeof(CAL3D_HW_VERTEX_BT),0, 44, 60, 76, 92);
	
	m_RenderableVertexs = new CIndexedVertexs<CAL3D_HW_VERTEX_BT>(CORE->GetRenderManager(), l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces*3);
	
	CHECKED_DELETE_ARRAY(l_Vtxs);
	CHECKED_DELETE_ARRAY(l_Idxs);

	return true;
}

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
	assert(m_CalCoreModel!=NULL);
	uint16 err = m_CalCoreModel->loadCoreMesh( Filename );
	if( err == -1)
	{
		LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadMesh->No se ha podido leer la Core Mesh.");
		return false;
	}

	return true;
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
	assert(m_CalCoreModel!=NULL);
	return m_CalCoreModel->loadCoreSkeleton( Filename );
}

bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
	assert(m_CalCoreModel!=NULL);
	uint16 err = m_CalCoreModel->loadCoreAnimation(Filename, Name);
	if( err == -1 )
	{
		LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadMesh->No se ha podido leer la animación: %", Name.c_str());
		return false;
	}

	return true;
}