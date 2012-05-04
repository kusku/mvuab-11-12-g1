#pragma once

#ifndef _PROCESS_H
#define _PROCESS_H

class CRenderManager;
class CCamera;

class CEngineProcess
{
public:
	//--- Init and End protocols------------------------------------------
						CEngineProcess	( void );
	virtual				~CEngineProcess	( void );

	virtual bool		Init			( void );
	virtual void		Done			( void );
	virtual bool		IsOk			( void ) const				{ return m_bIsOk; }
	
private:
	virtual void		Release		( void );

	//----Funcions principals--------------------------------------------
public:
	virtual void		Update			(float ElapsedTime);
	virtual void		Render			( CRenderManager &RM );
	
	//----Properties ( get & Set )---------------------------------------
	virtual CCamera*	GetCamera		( void ) const				{ return m_pCamera; }

	//----Members -------------------------------------------------------
protected:
	bool				m_bIsOk;			// em diu si tot és correcte o no en certes operacions

	CCamera*			m_pCamera;			// la càmera activa
};

#endif