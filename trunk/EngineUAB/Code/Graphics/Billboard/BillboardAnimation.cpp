#include "BillboardAnimation.h"
#include "RenderManager.h"
#include "Cameras\Camera.h"
#include <assert.h>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CBillboardAnimation::CBillboardAnimation(float height, float width, const Vect3f &position, float rotation, bool loop, const CColor& color)
	: m_uCurrentFrame(0)
	, m_fTimeToUpdate(0.f)
	, m_fTimeBeforeUpdate(0.f)
	, m_bLoop(loop)
{
	m_fHeight = height;
	m_fWidth = width;
	m_vPosition = position;
	m_fRotation = rotation;
	m_Color = color;
	m_pTexture = NULL;
}

CBillboardAnimation::CBillboardAnimation()
	: m_uCurrentFrame(0)
	, m_fTimeToUpdate(0.f)
	, m_fTimeBeforeUpdate(0.f)
	, m_bLoop(false)
{
}

CBillboardAnimation::~CBillboardAnimation()
{
	m_vTextures.clear();
}

void CBillboardAnimation::Update(float elapsedTime, CCamera &camera)
{
	m_fTimeBeforeUpdate += elapsedTime;
	if( m_fTimeBeforeUpdate > m_fTimeToUpdate )
	{
		++m_uCurrentFrame;
		if( m_uCurrentFrame >= m_vTextures.size() )
		{
			m_uCurrentFrame = 0;
		}

		m_fTimeBeforeUpdate -= m_fTimeToUpdate;
	}

	CBillboard::Update(camera);
}

void CBillboardAnimation::Render(CRenderManager &RM)
{
	m_pTexture = m_vTextures[ m_uCurrentFrame ];
	CBillboard::Render(RM);
}