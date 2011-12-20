#include "AnimatedCoreModel.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimatedCoreModel::CAnimatedCoreModel()
	: m_CalCoreModel(NULL)
	, m_Name("")
	, m_MeshFilename("")
	, m_SkeletonFilename("")
	, m_Path("")
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
	m_TextureFilenameVector.clear();
}

void CAnimatedCoreModel::Load(const std::string &Path)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(Path.c_str()))
	{
		std::string msg_error = "CAnimatedCoreModel::Load->Error al intentar leer el archivo xml de animación de un core: " + Path;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}
	m_Path = Path;

	CXMLTreeNode l_AnimationCore = newFile["skeletal"];
	if( l_AnimationCore.Exists() )
	{
		uint16 l_Count = l_AnimationCore.GetNumChildren();
		for( uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_AnimationCore(i).GetName();
			if( l_Type == "Model" )
			{
				m_Name = l_AnimationCore(i).GetPszProperty("name", "");
				m_MeshFilename = l_AnimationCore(i).GetPszProperty("mesh", "");
				m_SkeletonFilename = l_AnimationCore(i).GetPszProperty("skeleton", "");

				uint16 l_CountTex = l_AnimationCore(i).GetNumChildren();
				for( uint16 j=0; j<l_CountTex; ++j)
				{
					l_Type = l_AnimationCore(i)(j).GetName();
					if( l_Type == "Texture" )
					{
						std::string l_Texture = l_AnimationCore(i)(j).GetPszProperty("material", "");
						m_TextureFilenameVector.push_back(l_Texture);
					}
					else if( l_Type == "Animation" )
					{
						std::string l_AnimationName = l_AnimationCore(i)(j).GetPszProperty("name", "");
						std::string l_AnimationPath = l_AnimationCore(i)(j).GetPszProperty("path", "");
						LoadAnimation( m_Name, l_AnimationPath );
					}
				}

				LoadMesh();
				LoadSkeleton();
			}
		}
	}
}

bool CAnimatedCoreModel::LoadMesh()
{
	uint16 err = m_CalCoreModel->loadCoreMesh( m_MeshFilename, m_Name );
	if( err == -1)
	{
		LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadMesh->No se ha podido leer la Core Mesh.");
		return false;
	}

	return true;
}

bool CAnimatedCoreModel::LoadSkeleton()
{
	return m_CalCoreModel->loadCoreSkeleton( m_SkeletonFilename );
}

bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
	uint16 err = m_CalCoreModel->loadCoreAnimation(Filename, Name);
	if( err == -1 )
	{
		LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadMesh->No se ha podido leer la animación: %", Name.c_str());
		return false;
	}

	return true;
}