#include "IntroMovie.h"
#include "Base.h"
#include "Core.h"
#include "RenderManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"


#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

IntroMovie::IntroMovie(void)
	: m_Started(false)
	, m_Done(false)
	, m_TextureFrame(NULL)
	, m_SurfaceOffScreen(NULL)
	, m_Play(false)
	, m_TextureSurface(NULL)
{
	m_VideoManager = new TheoraVideoManager();
	m_Iface_factory = new OpenAL_AudioInterfaceFactory();

	m_VideoManager->setAudioInterfaceFactory(m_Iface_factory);
}


IntroMovie::~IntroMovie(void)
{
	CHECKED_DELETE(m_VideoManager);
	CHECKED_DELETE(m_Iface_factory);
	CHECKED_RELEASE(m_TextureFrame);
	CHECKED_RELEASE(m_SurfaceOffScreen);
	CHECKED_RELEASE(m_TextureSurface);
}

bool IntroMovie::InitMovie( const std::string& moviePath )
{
	m_Clip = m_VideoManager->createVideoClip(moviePath, TH_RGB, 120);

	m_Clip->stop();

	m_Clip->setAutoRestart(false);

	CHECKED_RELEASE(m_TextureFrame);
	
	CORE->GetRenderManager()->GetDevice()->CreateTexture(m_Clip->getWidth(), m_Clip->getHeight(), 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_TextureFrame, NULL);

	assert(m_TextureFrame);

	m_TextureFrame->GetSurfaceLevel(0, &m_TextureSurface);

	CHECKED_RELEASE(m_SurfaceOffScreen);

	CORE->GetRenderManager()->GetDevice()->CreateOffscreenPlainSurface(m_Clip->getWidth(), m_Clip->getHeight(), D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &m_SurfaceOffScreen, NULL);
	
	assert(m_SurfaceOffScreen);

	m_Started = true;

	return true;
}

void IntroMovie::Render()
{
	LPDIRECT3DDEVICE9 dxDevice = CORE->GetRenderManager()->GetDevice();
	CEffectTechnique* technique = CORE->GetEffectManager()->GetEffectTechnique("BasicQuadEffect");

	dxDevice->BeginScene();
	dxDevice->Clear(0, NULL, D3DCLEAR_TARGET, colBLACK.GetUint32Argb(), 1.0f, 0);

	TheoraVideoFrame* frame = m_Clip->getNextFrame();

	if (frame != NULL)
	{
		unsigned char* frameBuffer = frame->getBuffer();

		D3DLOCKED_RECT lockRect;

		m_SurfaceOffScreen->LockRect(&lockRect, NULL, D3DLOCK_DISCARD);

		//ZeroMemory(lockRect.pBits, (m_Clip->getWidth() * m_Clip->getHeight()) * sizeof(UINT) );

		UINT* textBuffer = (UINT*)lockRect.pBits;

		for (int j = 0; j < m_Clip->getHeight(); ++j)
		{
			for (int i = 0; i < m_Clip->getWidth(); ++i)
			{
				int pixPos = (j * m_Clip->getWidth()) + i;

				int framePos = ( j * (m_Clip->getWidth() * 3) ) + (i * 3);

				unsigned char pix1 = frameBuffer[(framePos) + 0];
				unsigned char pix2 = frameBuffer[(framePos) + 1];
				unsigned char pix3 = frameBuffer[(framePos) + 2];
				//unsigned char pix4 = frameBuffer[(framePos) + 3];

				UINT pix = 0;

				pix += pix1 << 16;
				pix += pix2 << 8;
				pix += pix3 << 0;
				//pix += pix4 << 0;
				
				textBuffer[pixPos] = pix; 
			}
		}

		//memcpy(lockRect.pBits, frameBuffer, (m_Clip->getWidth() * m_Clip->getWidth()) * 24);

		m_SurfaceOffScreen->UnlockRect();

		dxDevice->UpdateSurface(m_SurfaceOffScreen, NULL, m_TextureSurface, NULL);
		
		m_Clip->popFrame();
	}
	
	dxDevice->SetTexture(0, m_TextureFrame);
	CORE->GetRenderManager()->DrawQuad2DTexturedInPixelsInFullScreen(technique);

	dxDevice->EndScene();
	dxDevice->Present(NULL, NULL, NULL, NULL);
}

void IntroMovie::Update( float _ElapsedTime )
{
	if (m_Started)
	{
		if (m_Clip->getNumReadyFrames() < 2)
		{
			return;
		}

		m_Started = false;

		m_Clip->play();
	}

	m_VideoManager->update(0.0f);

#ifdef _DEBUG
	std::ostringstream outs;   
	outs.precision(6);
	outs << "Drops: " << m_Clip->getNumDroppedFrames() << "    "
		 << "FPS: " << m_Clip->getFPS() << "    "
		 << "PRE: " << m_Clip->getNumPrecachedFrames() << "    "
		 << "READY: " << m_Clip->getNumReadyFrames();
		
	SetWindowText(CORE->GetRenderManager()->GetHWND(), outs.str().c_str());
#endif

	m_Done = m_Clip->isDone();
}
