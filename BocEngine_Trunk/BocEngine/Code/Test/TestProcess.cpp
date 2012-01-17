#include "TestProcess.h"
#include "Player\Player.h"
#include "Base.h"
#include "Core.h"
#include "Fonts\FontManager.h"
#include "Physics\CollisionManager.h"
#include "RenderManager.h"
#include "Items\ItemManager.h"
#include "Items\Item.h"
#include "InputManager.h"

CTestProcess::CTestProcess(void)
	: m_Player(NULL)
	, m_CollisionManager(NULL)
	, m_ItemManager(NULL)
	, m_NumItems(50)
	, m_NumBonus(0)
	, m_Time(0.0f)
	, m_TimeToStart(4.0f)
	, m_State(PLAYING)
	, m_Screen(0,0)
{
}

CTestProcess::~CTestProcess(void)
{
	CHECKED_DELETE(m_Player);
	CHECKED_DELETE(m_CollisionManager);
	CHECKED_DELETE(m_ItemManager);
}

void CTestProcess::Init()
{
	Vect2i screen = CORE->GetRenderManager()->GetScreenSize();
	m_Screen = screen;

	m_Player = new CPlayer();
	m_Player->Init(screen);

	m_CollisionManager = new CCollisionManager();
	m_CollisionManager->Init(screen);

	m_ItemManager = new CItemManager();
	m_ItemManager->Init(m_NumItems, screen);
}

void CTestProcess::Update(float elapsedTime)
{
	m_Time += elapsedTime;
	m_Player->Update(elapsedTime);

	if( m_State == DEAD )
	{
		m_TimeToStart -= elapsedTime;
		if( m_TimeToStart < 0.0f )
		{
			m_Player->ResetPosition();
			m_TimeToStart = 4.0f;

			m_ItemManager->Release();
			m_ItemManager->Init(m_NumItems, m_Screen);

			m_State = PLAYING;
		}
	}
	else if( m_State == PLAYING )
	{
		Vect2f l_PlayerPosition = m_Player->GetPosition();
		if( m_CollisionManager->TestCollisionWithScreen( l_PlayerPosition ) || m_ItemManager->Impact2Stone( l_PlayerPosition ) )
		{
			m_Player->SubstractLife();
			if( m_Player->GetLife() == 0)
			{
				m_State = GAMEOVER;
			}
			else
			{
				m_State = DEAD;
			}
		
		}
		else if( m_ItemManager->Impact2Bonus( l_PlayerPosition ) )
		{
			++m_NumBonus;
		}
	}
	else if( m_State == GAMEOVER )
	{
		if( CORE->GetInputManager()->IsDownUp(IDV_KEYBOARD, KEY_SPACE) )
		{
			m_NumBonus = 0;
			m_Time = 0.0f;
			m_TimeToStart = 4.0f;
			m_Player->ResetPosition();
			m_Player->SetLife(5);
			m_ItemManager->Release();
			m_ItemManager->Init(m_NumItems, m_Screen);
			m_State = DEAD;
		}
	}
}

void CTestProcess::Render(CRenderManager *RM)
{
	if( m_State == PLAYING )
	{
		m_Player->Render();
		m_ItemManager->Render();

		CORE->GetFontManager()->DrawDefaultText(200, 0, colCYAN, "Temps: %f", m_Time );
		CORE->GetFontManager()->DrawDefaultText(350, 0, colCYAN, "Vides: %d", m_Player->GetLife() );
		CORE->GetFontManager()->DrawDefaultText(500, 0, colCYAN, "Bonus: %d", m_NumBonus );
		CORE->GetFontManager()->DrawDefaultText(600, 0, colCYAN, "Velocitat: %f", m_Player->GetSpeed() );
	}
	else if( m_State == DEAD )
	{
		CORE->GetFontManager()->DrawDefaultText(150, 150, colWHITE, "Comença en: %d", static_cast<int>(m_TimeToStart) );
	}
	else if( m_State == GAMEOVER )
	{
		CORE->GetFontManager()->DrawDefaultText(150, 150, colRED, "GAME OVER\n Prém Barra Espaciadora per tornar a jugar." );
	}
}

void CTestProcess::ResetValues()
{
	m_Time = 0.0f;
}