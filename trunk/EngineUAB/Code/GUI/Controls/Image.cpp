#include "Image.h"
#include "Textures/Texture.h"
#include "RenderManager.h"
#include "GraphicsDefs.h"
#include "Core.h"
#include "Base.h"

//---Constructor
CImage::CImage(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
							 const Vect2f position_percent, std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, 
							 bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, IMAGE, lit, textHeightOffset, textWidthOffset, isVisible,isActive)
, m_sActiveTexture( "default" )
, m_bAnimated(false)
, m_bLoop(false)
, m_fTimePerImage(0.f)
, m_fCounter(0.f)
, m_Color(colBLACK)
, m_eFlip(NONE_FLIP)
, m_bIsQuadrant(false)
, m_bIsBackGround(false)
, m_iEffect(GUI_I_NO_EFFECT)
, m_fAlpha(1.f)
{}


//---------------CGuiElement Interface----------------------
void CImage::Render (CRenderManager *renderManager, CFontManager* fm)
{
	if( CGuiElement::m_bIsVisible )
	{
		CTexture* texture = NULL;
		if (m_bAnimated)
		{
			texture = *m_itVecTextures;
		}
		else
		{
			tTexturesMap::iterator it = m_Textures.find(m_sActiveTexture);
			if (it != m_Textures.end())
			{
				texture = it->second;
			}
		}
		
		if (texture)
		{
			uint32 w = CGuiElement::m_uWidth;
			uint32 h = CGuiElement::m_uHeight;
			if (m_bIsQuadrant)
				h = CGuiElement::m_uWidth;

			if (texture)
      {
				if (m_iEffect == GUI_I_FADE_OUT)
        {
          if (m_fFadeCounter >= m_fFadeStartAfter)
          {
            m_fAlpha -= (m_fFadeCounter - m_fFadeStartAfter) * m_fFadePerSecond * 0.01f;
            if (m_fAlpha <= 0.f)
            {
              m_fAlpha = 0.f;
              CGuiElement::m_bIsVisible = false;
              m_iEffect = GUI_I_NO_EFFECT;
            }
          }
        }
        else
        {
          m_fAlpha = 1.0f;
        }

        if (m_fAlpha < 1.0f)
        {
          CColor color = CColor (1.0f, 1.0f, 1.0f, m_fAlpha);
          renderManager->DrawTexturedQuad2D(CGuiElement::m_Position, w, h, UPPER_LEFT, NONE_FLIP, texture, color);
        }
        else
  				renderManager->DrawTexturedQuad2D(CGuiElement::m_Position,w, h,  UPPER_LEFT, m_eFlip, texture );
      }
			else
				renderManager->DrawQuad2D(CGuiElement::m_Position, w, h,  UPPER_LEFT, m_Color);
		}

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText(renderManager, fm);	
	}
	
}

void CImage::Update (CInputManager* intputManager, float elapsedTime)
{
	if (m_bAnimated)
	{
		m_fCounter += elapsedTime;
		if (m_fCounter > m_fTimePerImage)
		{
			m_itVecTextures++;
			if (m_itVecTextures == m_VecTextures.end())
			{
				m_itVecTextures = m_VecTextures.begin();
				if (!m_bLoop)
				{
					m_bAnimated = false;	
				}
			}
			m_fCounter = 0.f;
		}
	}

  if (m_iEffect != GUI_I_NO_EFFECT)
  {
    m_fFadeCounter += elapsedTime;
  }
}

//---------------CImage Interface----------------------
void CImage::PlayAnimation (float timePerImage, bool loop)
{
	assert(m_Textures.size()>0);

	m_bAnimated			= true;
	m_bLoop					= loop;
	m_fTimePerImage	= timePerImage;
	m_itVecTextures	= m_VecTextures.begin();
	m_fCounter			= 0.f;
}

void CImage::SetTexture (CTexture* texture, std::string name )
{
	m_Textures.insert( std::pair<std::string,CTexture*>(name, texture) );
	m_VecTextures.push_back(texture);
}

void CImage::FadeOut(float startTime, float fadePerSecond)
{
  m_iEffect = GUI_I_FADE_OUT;
  m_fFadeCounter = 0.0f;
  m_fFadeStartAfter = startTime;
  m_fFadePerSecond = fadePerSecond;
}

