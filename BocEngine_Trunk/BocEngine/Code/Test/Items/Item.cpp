#include "Item.h"
#include "Base.h"
#include "Core.h"
#include "Fonts\FontManager.h"


CItem::CItem()
	: m_Type(NONE)
	, m_Position(0.0f, 0.0f)
{
}


CItem::~CItem()
{
}

void CItem::Render()
{
	if( m_Type == STONE )
	{
		CORE->GetFontManager()->DrawDefaultText(static_cast<uint32>(m_Position.x), static_cast<uint32>(m_Position.y), colMAGENTA, "O" );
	}
	else if( m_Type == BONUS )
	{
		CORE->GetFontManager()->DrawDefaultText(static_cast<uint32>(m_Position.x), static_cast<uint32>(m_Position.y), colMAGENTA, "b" );
	}
}

bool CItem::Impact(const Vect2f &player)
{
	float distance = m_Position.Distance(player);
	if(distance < 10.0f)
	{
		return true;
	}

	return false;
}
