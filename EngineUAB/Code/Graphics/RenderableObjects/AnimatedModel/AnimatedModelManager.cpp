#include "AnimatedModelManager.h"
#include "AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// -----------------------------------------

CAnimatedModelManager::CAnimatedModelManager()
{
	CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
}


CAnimatedModelManager::~CAnimatedModelManager()
{
	CleanUp();
}

// -----------------------------------------
//				MAIN METHODS 
// -----------------------------------------
void CAnimatedModelManager::CleanUp()
{
	Destroy();
}

void CAnimatedModelManager::Load(const std::string &Filename)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedModelManager::Load->Cargando los Animated Models.");

	CXMLTreeNode newFile;
	if (!newFile.LoadFile(Filename.c_str()))
	{
		std::string msg_error = "CAnimatedModelManager::Load->Error al intentar leer el archivo xml de modelos animados: " + Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}	

	CXMLTreeNode l_AnimationModels = newFile["animated_models"];
	if( l_AnimationModels.Exists() )
	{
		uint16 l_Count = l_AnimationModels.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_AnimationModels(i).GetName();
			if( l_Type == "animated_model" )
			{
				std::string l_Name = l_AnimationModels(i).GetPszProperty("name", "");
				std::string l_Path = l_AnimationModels(i).GetPszProperty("path", "");
				std::string l_XMLFilename = l_AnimationModels(i).GetPszProperty("filename", "");
				GetCore(l_Name, l_Path, l_XMLFilename);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------
// GetCore : Obtiene el modelo o core existente en el Manager, en caso de no existir crea uno nuevo y carga el modelo segun 
//			la ruta especificada. Devuelve el la classe encap�uladora del modelo animado de Cal3D
// ------------------------------------------------------------------------------------------------------------------------------
CAnimatedCoreModel* CAnimatedModelManager::GetCore( const std::string &_Name, const std::string &_Path, const std::string &_XMLFilename )
{
	CAnimatedCoreModel* l_CoreModel = GetResource( _Name );
	if( l_CoreModel == NULL )
	{
		l_CoreModel = new CAnimatedCoreModel();
		l_CoreModel->Load( _Path, _XMLFilename );

		AddResource( _Name, l_CoreModel );
	}

	assert(l_CoreModel != NULL);
	return l_CoreModel;
}

// ------------------------------------------------------------------------------------------------------------------------------
// GetInstance : Obtiene el modelo o core existente en el Manager, en caso de no existir sale devolviendo una instancia nula. En 
//				otro caso crea una nueva instancia y carga el modelo o core a esta devovieldo la instancia animada
// ------------------------------------------------------------------------------------------------------------------------------
CAnimatedInstanceModel* CAnimatedModelManager::GetInstance( const std::string &_Name )
{
	CAnimatedInstanceModel* l_InstanceModel = NULL;
	CAnimatedCoreModel* l_CoreModel = GetResource( _Name );
	
	if( l_CoreModel != NULL )
	{
		l_InstanceModel = new CAnimatedInstanceModel();
		l_InstanceModel->Initialize( l_CoreModel );
	}

	assert(l_InstanceModel != NULL);
	return l_InstanceModel;
}