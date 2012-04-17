#include "GUIAnimatedImage.h"
#include "Textures\Texture.h"
#include "RenderManager.h"
#include "Core.h"
#include "Base.h"

//---Constructor
CGUIAnimatedImage::CGUIAnimatedImage(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
							 const Vect2f position_percent, std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, 
							 bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, IMAGE, lit, textHeightOffset, textWidthOffset, isVisible,isActive)
, m_bAnimated(false)
, m_bLoop(false)
, m_fTimePerImage(0.f)
, m_fCounter(0.f)
, m_Color(colBLACK)
, m_eFlip(NONE_FLIP)
, m_bIsQuadrant(false)
, m_bIsBackGround(false)
, m_bPlayForward(true)
, m_sCurrentFrame(0)
, m_sFinalFrame(-1)
, m_iEffect(GUI_AI_NO_EFFECT)
, m_fAlpha(1.f)
{}


//---------------CGuiElement Interface----------------------
void CGUIAnimatedImage::Render (CRenderManager *renderManager, CFontManager* fm)
{
	if( CGuiElement::m_bIsVisible )
	{
		CTexture* texture = NULL;
		
    texture = m_VecTextures[m_sCurrentFrame];
		
		if (texture)
		{
			uint32 w = CGuiElement::m_uWidth;
			uint32 h = CGuiElement::m_uHeight;
			if (m_bIsQuadrant)
				h = CGuiElement::m_uWidth;

      if (texture) {
        if (m_iEffect == GUI_AI_FADE_OUT)
        {
          if (m_fFadeCounter >= m_fFadeStartAfter)
          {
            m_fAlpha -= (m_fFadeCounter - m_fFadeStartAfter) * m_fFadePerSecond * 0.01f;
            if (m_fAlpha <= 0.f)
            {
              m_fAlpha = 0.f;
              CGuiElement::m_bIsVisible = false;
              m_iEffect = GUI_AI_NO_EFFECT;
            }
          }
        }
        else
        {
          m_fAlpha = 1.0f;
        }

        CColor color = CColor (1.0f, 1.0f, 1.0f, m_fAlpha);

        renderManager->DrawTexturedQuad2D(CGuiElement::m_Position, w, h, UPPER_LEFT, texture, color);
				//renderManager->DrawQuad2D(CGuiElement::m_Position,w, h,  CRenderManager::UPPER_LEFT, texture, m_eFlip);
      }
			else
				renderManager->DrawQuad2D(CGuiElement::m_Position, w, h,  UPPER_LEFT, m_Color);
		}

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText(renderManager, fm);	
	}
	
}

void CGUIAnimatedImage::Update (CInputManager* intputManager, float elapsedTime)
{
	if (m_bAnimated)
	{
		m_fCounter += elapsedTime;
    
    if (m_fCounter > m_fTimePerImage)
		{
      if (m_bPlayForward)
      {
        if (m_sCurrentFrame == m_sFinalFrame)
        {
          m_bAnimated = false;
        }
        else
        {
          m_sCurrentFrame++;

          if (m_sCurrentFrame == m_VecTextures.size())
		      {
			      m_sCurrentFrame = 0;
  				  
            if (!m_bLoop)
			      {
				      m_bAnimated = false;	
			      }
		      }
        }
      }
      else
      {
        if (m_sCurrentFrame == m_sFinalFrame)
        {
          m_bAnimated = false;
        }
        else
        {
          m_sCurrentFrame--;
          if (m_sCurrentFrame < 0)
          {
            m_sCurrentFrame = m_VecTextures.size() - 1;
          
            if (!m_bLoop)
			      {
				      m_bAnimated = false;	
			      }
          }
        }
      }
			
			m_fCounter = 0.f;
		}
	}

  if (m_iEffect != GUI_AI_NO_EFFECT)
  {
    m_fFadeCounter += elapsedTime;
  }
}

//---------------CGUIAnimatedImage Interface----------------------
void CGUIAnimatedImage::PlayAnimation (float timePerImage, bool loop, bool playForward, int16 finalFrame)
{
	assert(m_VecTextures.size()>0);

	if (!m_bAnimated)
  {
    m_bAnimated = true;
	  m_fCounter  = 0.f;
  }

  m_bLoop					= loop;
	m_fTimePerImage	= timePerImage;
	m_bPlayForward  = playForward;
  m_sFinalFrame   = finalFrame;
}

void CGUIAnimatedImage::AddFrame (CTexture* texture)
{
	m_VecTextures.push_back(texture);
}

/// <summary>
/// La animación pasa al frame especificado y se detiene la animaci´n en caso de estarse reproduciendo
/// </summary>
///<param name="frame">frame destino</param>
void CGUIAnimatedImage::GotoAndStop(int16 frame)
{
  m_sCurrentFrame = frame;
  m_bAnimated = false; 
}

void CGUIAnimatedImage::FadeOut(float startTime, float fadePerSecond)
{
  m_iEffect = GUI_AI_FADE_OUT;
  m_fFadeCounter = 0.0f;
  m_fFadeStartAfter = startTime;
  m_fFadePerSecond = fadePerSecond;
}


