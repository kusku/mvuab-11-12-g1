#pragma once

#ifndef _INTRO_MOVIE_H
#define _INTRO_MOVIE_H

#include "TheoraPlayer.h"
#include "OpenAL_AudioInterface.h"
#include <d3dx9.h>

class IntroMovie
{
	bool m_Done;
	bool m_Started;
	bool m_Play;
	
	TheoraVideoManager* m_VideoManager;
	TheoraVideoClip* m_Clip;
	OpenAL_AudioInterfaceFactory* m_Iface_factory;
	LPDIRECT3DTEXTURE9 m_TextureFrame;
	LPDIRECT3DSURFACE9 m_SurfaceOffScreen;
	LPDIRECT3DSURFACE9 m_TextureSurface;


public:
	IntroMovie(void);
	virtual ~IntroMovie(void);

	bool InitMovie(const std::string& moviePath);

	void Update( float _ElapsedTime );
	void Render();

	bool IsDone() const { return m_Done; }
}; 

#endif

