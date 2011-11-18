#include "ItemManager.h"
#include "Item.h"
#include "Base.h"
#include <time.h>

CItemManager::CItemManager(void)
{
}


CItemManager::~CItemManager(void)
{
	Release();
}

void CItemManager::Release()
{
	std::vector<CItem*>::iterator it;
	for(it = m_ItemVector.begin(); it != m_ItemVector.end(); ++it)
	{
		CHECKED_DELETE( (*it) );
	}
	m_ItemVector.clear();
}

void CItemManager::Init(int numItems, const Vect2i &screen)
{
	CItem *l_Item;
	Vect2f l_Position;
	ITEM_TYPE l_Type;

	for(int i=0; i<numItems; ++i)
	{
		srand(i * (uint32)time(NULL));
		l_Item = new CItem();

		l_Position.x = static_cast<float>(rand() % screen.x);
		l_Position.y = static_cast<float>(rand() % screen.y);
		l_Item->SetPosition(l_Position);

		 int typeValue =  static_cast<ITEM_TYPE>(rand() % 5);
		 if( typeValue < 4)
		 {
			 l_Type = STONE;
		 }
		 else
		 {
			 l_Type = BONUS;
		 }
		 
		l_Item->SetType(l_Type);

		m_ItemVector.push_back(l_Item);
		l_Item = NULL;
	}
}

void CItemManager:: Render()
{
	std::vector<CItem*>::iterator it;
	for(it = m_ItemVector.begin(); it != m_ItemVector.end(); ++it)
	{
		(*it)->Render();
	}
}

bool CItemManager::Impact2Bonus(const Vect2f &player)
{
	std::vector<CItem*>::iterator it;
	for(it = m_ItemVector.begin(); it != m_ItemVector.end(); ++it)
	{
		if( (*it)->GetType() == BONUS )
		{
			if( (*it)->Impact(player) )
			{
				m_ItemVector.erase(it);
				return true;
			}
		}
	}

	return false;
}

bool CItemManager::Impact2Stone(const Vect2f &player)
{
	std::vector<CItem*>::iterator it;
	for(it = m_ItemVector.begin(); it != m_ItemVector.end(); ++it)
	{
		if( (*it)->GetType() == STONE )
		{
			if( (*it)->Impact(player) )
			{
				return true;
			}
		}
	}

	return false;
}