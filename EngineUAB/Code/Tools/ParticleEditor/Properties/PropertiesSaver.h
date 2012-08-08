#pragma once

#ifndef _PROPERTIES_SAVER_H_
#define _PROPERTIES_SAVER_H_

#include "Particles\ParticleDef.h"

class CPropertiesSaver
{
public:
	static CPropertiesSaver* GetInstance();
	void CleanUp();

	void					SetProperties	( CMFCPropertyGridCtrl *prop )		{ m_pProperties = prop; }
	CMFCPropertyGridCtrl*	GetProperties	() const							{ return m_pProperties; }

	void	SaveProperties( TParticleSystemSettings *_ParticleSystem );
	void	LoadProperties( TParticleSystemSettings *_ParticleSystem );

private:
	CPropertiesSaver();
	~CPropertiesSaver();

private:
	static CPropertiesSaver *m_pPropertiesSaver;
	CMFCPropertyGridCtrl *m_pProperties;
};

#endif //_PROPERTIES_SAVER_H_