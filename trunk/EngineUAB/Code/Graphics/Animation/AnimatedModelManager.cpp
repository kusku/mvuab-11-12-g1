#include "AnimatedModelManager.h"
#include "AnimatedInstanceModel.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CAnimatedModelManager::CAnimatedModelManager(void)
{
}


CAnimatedModelManager::~CAnimatedModelManager(void)
{
}

CAnimatedCoreModel* CAnimatedModelManager::GetCore(const std::string &Name, const std::string &Path)
{
	CAnimatedCoreModel* l_CoreModel = GetResource(Name);
	if( l_CoreModel == NULL )
	{
		l_CoreModel = new CAnimatedCoreModel();
		l_CoreModel->Load(Path);

		AddResource(Name, l_CoreModel);
	}

	return l_CoreModel;
}

CAnimatedInstanceModel* CAnimatedModelManager::GetInstance(const std::string &Name)
{
	CAnimatedInstanceModel* l_InstanceModel = NULL;
	CAnimatedCoreModel* l_CoreModel = GetResource(Name);
	
	if( l_CoreModel != NULL )
	{
		l_InstanceModel = new CAnimatedInstanceModel();
		l_InstanceModel->Initialize( l_CoreModel );
	}

	return l_InstanceModel;
}