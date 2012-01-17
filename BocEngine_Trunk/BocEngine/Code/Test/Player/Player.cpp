#include "Player.h"
#include "Core.h"
#include "Fonts\FontManager.h"
#include "InputManager.h"
#include "InputDefs.h"
#include "Base.h"

CPlayer::CPlayer()
	: m_Position(0.0f, 0.0f)
	, m_InitPosition(0.0f, 0.0f)
	, m_Velocity(50.f)
	, m_Direction(UP)
	, m_TimeToAccelerate(0.0f)
	, m_Lifes(5)
	, m_bIsDead(false)
{
}


CPlayer::~CPlayer()
{
}

void CPlayer::Init(const Vect2i &screen)
{
	m_InitPosition.x = (float)(screen.x / 2);
	m_InitPosition.y = (float)(screen.y / 2);

	m_Position = m_InitPosition;
}

void CPlayer::Update(float elapsedTime)
{
	//Mirar el temps per augmentar la velocitat el personatge
	m_TimeToAccelerate += elapsedTime;
	if( m_TimeToAccelerate > 30.0f )
	{
		m_TimeToAccelerate = 0.0f;
		m_Velocity = m_Velocity * 1.5f;
	}

	//Canviar de direcció del personatge
	CInputManager *l_Input = CORE->GetInputManager();

	if( l_Input->IsDownUp(IDV_KEYBOARD, KEY_UP) ) //Fletxa amunt
	{
		m_Direction = UP;
	}
	else if( l_Input->IsDown( IDV_KEYBOARD, KEY_DOWN ) ) //Fletxa avall
	{
		m_Direction = DOWN;
	}
	else if( l_Input->IsDown( IDV_KEYBOARD, KEY_LEFT ) ) //Flexta esquerra
	{
		m_Direction = LEFT;
	}
	else if( l_Input->IsDown( IDV_KEYBOARD, KEY_RIGHT ) ) //Flexta dreta
	{
		m_Direction = RIGHT;
	}

	//Moure el personatge
	switch(m_Direction)
	{
	case UP:
		{
			m_Position.y -= m_Velocity * elapsedTime;
			break;
		}
	case DOWN:
		{
			m_Position.y += m_Velocity * elapsedTime;
			break;
		}
	case LEFT:
		{
			m_Position.x -= m_Velocity * elapsedTime;
			break;
		}
	case RIGHT:
		{
			m_Position.x += m_Velocity * elapsedTime;
			break;
		}
	}
}

void CPlayer::Render()
{
	CORE->GetFontManager()->DrawDefaultText((uint32)m_Position.x, (uint32)m_Position.y, colWHITE, "X");
}

void CPlayer::ResetPosition()
{
	m_Position = m_InitPosition;
}