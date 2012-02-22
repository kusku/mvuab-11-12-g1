#include "base.h"
#include "Jump.h"

#include "Utils/MemLeaks.h"

void CJump::StartJump(float v0)
{
	if(m_bJump)
	{
		return;
	}

	m_fJumpTime	= 0.0f;
	m_fV0				= v0;
	m_bJump			= true;
}

void CJump::StopJump()
{
	if(!m_bJump)
	{
		return;
	}

	m_bJump = false;
}

float CJump::GetHeight(float elapsedTime)
{
	/*
	a(t) = cte = g
	v(t) = g*t + v0
	y(t) = g*t^2 + v0*t + y0
	*/

	if(!m_bJump)
	{
		return 0.0f;
	}

	m_fJumpTime += elapsedTime;
	float h = m_fG*m_fJumpTime*m_fJumpTime + m_fV0*m_fJumpTime;
	return (h - m_fDefaultGravity)*elapsedTime;
}