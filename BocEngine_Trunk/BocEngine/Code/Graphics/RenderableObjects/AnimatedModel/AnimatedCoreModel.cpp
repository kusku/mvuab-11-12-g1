#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimatedCoreModel::CAnimatedCoreModel()
	: m_CalCoreModel(NULL)
	, m_Name("")
	, m_Path("")
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
	m_TextureFilenameVector.clear();
	CHECKED_DELETE(m_CalCoreModel);
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