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
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Core.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------
CAnimatedCoreModel::CAnimatedCoreModel()
	: m_CalCoreModel		( NULL )
	, m_Name				( "" )
	, m_Path				( "" )
	, m_RenderableVertexs	( NULL )
	, m_CalHardwareModel	( NULL )
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
	m_TextureFilenameVector.clear();
	CHECKED_DELETE(m_CalCoreModel);
	CHECKED_DELETE(m_CalHardwareModel);
	CHECKED_DELETE(m_RenderableVertexs);
}

// -----------------------------------------
//				MAIN METHODS 
// -----------------------------------------

void CAnimatedCoreModel::ClearTextureVector()
{
	std::vector<CTexture*>::iterator l_It = m_TextureVector.begin();
	std::vector<CTexture*>::iterator l_End = m_TextureVector.end();
	for(uint32 i = 0; i < m_TextureVector.size(); ++i)
	{
		CTexture* texture = m_TextureVector[i];

		if(!CORE->GetTextureManager()->RemoveResource(texture->GetName()))
		{
			CHECKED_DELETE(texture);
		}
	}
	m_TextureVector.clear();
}

void CAnimatedCoreModel::Load(const std::string &Path, const std::string &XMLFilename)
{
	CXMLTreeNode newFile;
	std::string filename = Path + XMLFilename;
	if (!newFile.LoadFile(filename.c_str()))
	{
		std::string msg_error = "CAnimatedCoreModel::Load->Error al intentar leer el archivo xml de animación de un core: " + filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return;
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
			std::string l_File = l_AnimationCore(i).GetPszProperty("filename", "");
			l_File = m_Path + l_File;
			if( l_Type == "mesh" )
			{
				if( !LoadMesh( l_File ) ) //Load Mesh
				{
					LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load->No se ha podido cargar la mesh: %s", l_File.c_str());
				}
			}
			else if( l_Type == "skeleton" )
			{
				if( !LoadSkeleton( l_File ) ) //Load Skeleton
				{
					LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load->No se ha podido cargar el esqueleto: %s", l_File.c_str());
				}
			}
			else if( l_Type == "animation" )
			{
				std::string l_AnimationName = l_AnimationCore(i).GetPszProperty("name", "");
				if( !LoadAnimation( l_AnimationName, l_File ) ) //Load Animation
				{
					LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load->No se ha podido cargar la animación: %s", l_File.c_str());
				}
			}
			else if( l_Type == "texture" )
			{
				m_TextureFilenameVector.push_back( l_File ); //Load texture string
			}
		}

		LoadTextures();
	}
}

bool CAnimatedCoreModel::LoadVertexBuffer(CalModel *Model)
{
	m_NumVtxs = 0;
	m_NumFaces = 0;

	//Calcula el nombre de vértices y caras que tiene el modelo animado
	uint16 l_MeshCount = m_CalCoreModel->getCoreMeshCount();
	for(uint16 i=0;i < l_MeshCount; ++i)
	{
		CalCoreMesh *l_CoreMesh=m_CalCoreModel->getCoreMesh(i);
		uint16 l_SubMeshCount = l_CoreMesh->getCoreSubmeshCount();
		for(uint16 j=0;j < l_SubMeshCount; ++j)
		{
			m_NumVtxs+=l_CoreMesh->getCoreSubmesh(j)->getVertexCount();
			m_NumFaces+=l_CoreMesh->getCoreSubmesh(j)->getFaceCount();
		}
	}
	assert(m_NumVtxs > 0 && m_NumFaces > 0);

	//CAL3D_HW_VERTEX_BT* pVertex;
	CHECKED_DELETE(m_CalHardwareModel);
	CHECKED_DELETE(m_RenderableVertexs);

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
	assert( m_CalCoreModel != NULL );
	
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
	assert( m_CalCoreModel != NULL );

	uint16 err = m_CalCoreModel->loadCoreAnimation(Filename, Name);
	if( err == -1 )
	{
		LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadMesh->No se ha podido leer la animación: %", Name.c_str());
		return false;
	}

	return true;
}

void CAnimatedCoreModel::LoadTextures()
{
	size_t l_TexCount = m_TextureFilenameVector.size();
	for(size_t i = 0; i < l_TexCount; ++i)
	{
		std::string l_Path = m_TextureFilenameVector[i];

		CTexture* l_Texture = CORE->GetTextureManager()->GetTexture(l_Path);
		if( l_Texture == NULL )
		{
			l_Texture = CORE->GetTextureManager()->GetNoTexture();

			std::string err = "CAnimatedCoreModel::LoadTextures->No se ha podido crear la textura: " + l_Path;
			LOGGER->AddNewLog(ELL_WARNING, err.c_str() );
		}
		
		m_TextureVector.push_back(l_Texture);
		l_Texture = NULL;
	}
}

// -----------------------------------------
//				  PROPERTIES
// -----------------------------------------

CalHardwareModel* CAnimatedCoreModel::GetCalHardwareModel() const
{
	return m_CalHardwareModel;
}

CRenderableVertexs* CAnimatedCoreModel::GetRenderableVertexs() const
{
	return m_RenderableVertexs;
}
