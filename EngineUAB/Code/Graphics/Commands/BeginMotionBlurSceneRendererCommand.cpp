#include <assert.h>

#include "BeginMotionBlurSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Core.h"
#include "Math\Vector2.h"
#include <d3dx9.h>

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------	
CBeginMotionBlurSceneRendererCommand::CBeginMotionBlurSceneRendererCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
	, m_PrevMotionBlurVelocity(NULL)
	, m_CurrentMotionBlurVelocity(NULL)
{
	bool ret = false;

	Vect2i screenSize = CORE->GetRenderManager()->GetScreenSize();

	if(CORE->GetTextureManager()->GetResource("__PREV_MOTION_BLUR_TEXTURE") != NULL)
	{
		ret = CORE->GetTextureManager()->RemoveResource("__PREV_MOTION_BLUR_TEXTURE");

		assert(ret);
	}

	m_PrevMotionBlurVelocity = new CTexture();

	ret = m_PrevMotionBlurVelocity->Create("__PREV_MOTION_BLUR_TEXTURE", screenSize.x, screenSize.y, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::G32R32F);

	assert(ret);

	ret = CORE->GetTextureManager()->AddResource("__PREV_MOTION_BLUR_TEXTURE", m_PrevMotionBlurVelocity);

	assert(ret);

	//////////////////////////
	//////////////////////////
	//////////////////////////

	if(CORE->GetTextureManager()->GetResource("__CURRENT_MOTION_BLUR_TEXTURE") != NULL)
	{
		ret = CORE->GetTextureManager()->RemoveResource("__CURRENT_MOTION_BLUR_TEXTURE");

		assert(ret);
	}

	m_CurrentMotionBlurVelocity = new CTexture();

	ret = m_CurrentMotionBlurVelocity->Create("__CURRENT_MOTION_BLUR_TEXTURE", screenSize.x, screenSize.y, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::G32R32F);

	assert(ret);

	ret = CORE->GetTextureManager()->AddResource("__CURRENT_MOTION_BLUR_TEXTURE", m_CurrentMotionBlurVelocity);

	assert(ret);
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CBeginMotionBlurSceneRendererCommand::Execute( CRenderManager &_RM )
{	
	assert(m_PrevMotionBlurVelocity || m_CurrentMotionBlurVelocity);

	LPDIRECT3DTEXTURE9 tempText = NULL;

	tempText = m_PrevMotionBlurVelocity->GetDXTexture();

	m_PrevMotionBlurVelocity->SetDXTexture(m_CurrentMotionBlurVelocity->GetDXTexture());

	m_CurrentMotionBlurVelocity->SetDXTexture(tempText);
}
