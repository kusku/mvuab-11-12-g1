#pragma once

#ifndef _PROCESS_H
#define _PROCESS_H

class CRenderManager;
class CCamera;

class CEngineProcess
{
public:
	//--- Init and End protocols------------------------------------------
						CEngineProcess	();
	virtual				~CEngineProcess	();

	virtual bool		Init			();
	virtual void		Done			();
	virtual bool		IsOk			() const				{ return m_bIsOk; }
	
	//----Funcions principals-------------------------------------------
	virtual void		Update			(float ElapsedTime);
	virtual void		Render			( CRenderManager &RM );
	
	//----Properties ( get & Set )---------------------------------------
	virtual CCamera*	GetCamera		() const				{ return m_pCamera; }

private:
	virtual void		Release		();

protected:
	bool				m_bIsOk;			// em diu si tot és correcte o no en certes operacions

	CCamera*			m_pCamera;			// la càmera activa
};

#endif