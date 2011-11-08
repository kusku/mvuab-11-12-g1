#include "EnemyManager.h"
#include "PatrolEnemy.h"
#include "TurretEnemy.h"
#include "XML\XMLTreeNode.h"
#include <string>

CEnemyManager::CEnemyManager()
{
}


CEnemyManager::~CEnemyManager()
{
	std::vector<CEnemy *>::iterator it = m_EnemiesVector.begin();

	for(; it != m_EnemiesVector.end(); ++it)
	{
		if ((*it) != NULL)
		{
			delete (*it);
		}
	}
	m_EnemiesVector.clear();
}

void CEnemyManager::LoadXML(const std::string &filename)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(filename.c_str()))
	{
		return;
	}

	CXMLTreeNode l_EnemiesNode = newFile["enemies"];
	if (l_EnemiesNode.Exists())
	{
		int count = l_EnemiesNode.GetNumChildren();
		for(int i=0; i < count; ++i)
		{
			std::string l_Name = l_EnemiesNode(i).GetName();
			if (l_Name == "enemy")
			{
				std::string l_Type = l_EnemiesNode(i).GetPszProperty("type","");
				const char *l_Pos = l_EnemiesNode(i).GetPszProperty("pos","");
				float l_Yaw = l_EnemiesNode(i).GetFloatProperty("yaw",0.0f);
				float l_Pitch = l_EnemiesNode(i).GetFloatProperty("pitch",0.0f);			

				CEnemy *l_Enemy = NULL;

				if (l_Type == "patrol")
				{
					l_Enemy = static_cast<CEnemy *>(new CPatrolEnemy());
				}
				else if (l_Type == "turret")
				{
					l_Enemy = static_cast<CEnemy *>(new CTurretEnemy());
				}

				if(l_Enemy != NULL)
				{
					l_Enemy->Init(l_Pos, l_Yaw, l_Pitch);
					m_EnemiesVector.push_back(l_Enemy);
				}
			}
		}
	}
}

void CEnemyManager::Update(float ElapsedTime)
{
	printf("Updating EnemyManager.\n");

	std::vector<CEnemy *>::iterator it = m_EnemiesVector.begin();

	for(; it != m_EnemiesVector.end(); ++it)
	{
		(*it)->Update(ElapsedTime);
	}
}

void CEnemyManager::Render()
{
	printf("Rendering EnemyManager.\n");

	std::vector<CEnemy *>::iterator it = m_EnemiesVector.begin();

	for(; it != m_EnemiesVector.end(); ++it)
	{
		(*it)->Render();
	}
}