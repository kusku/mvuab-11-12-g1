#pragma once

#ifndef __CLASS_TEST_PROCESS_H__
#define __CLASS_TEST_PROCESS_H__

#include <vector>

#include "TestDef.h"

#include "EngineProcess.h"
#include "Math\Vector2.h"

//---FOWARD DECLARATIONS---//
//class CPlayer; 
class CCamera;
class CThPSCamera;
class CFPSCamera;
class CRenderManager;
class CActionToInput;
//-------------------------//

class CTestProcess : public CEngineProcess
{
public:
	//--- Init and End protocols------------------------------------------
					CTestProcess		( void );
	virtual			~CTestProcess		( void );

	void			Done				( void );
	virtual bool	isOK				( void ) const { return m_bIsOk; };
	virtual bool	Init				( void );
	virtual void	Release				( void );

	//----Main Functions -------------------------------------------------
	virtual void	Update				( float _ElapsedTime );
	virtual void	Render				( CRenderManager *_RM );
	//virtual void	RenderScene			( void );

	void			CargarProcesos		( void );

	//----Functions ------------------------------------------------------
protected:
	virtual void	UpdateInputs		( float _ElapsedTime );
	virtual void	AddPlayer			( std::string _Name,	Vect3f _vPosition, bool _Automatic );

	//----Test Functions -------------------------------------------------
																		 
	//----Properties ( get & Set )----------------------------------------
																		 
	//----Members --------------------------------------------------------
	Vect2i							m_VectRatoli;		// vector de la posició del ratolí 
	Vect2i							m_VectScreen;		// vector de la resolució de la pantalla

	float							m_fTempsTotal;		// temps global de l'aplicació desde l'inici del motor
	float							m_fPosX;
	float							m_fAngleX;
	float							m_fAngleY;
	float							m_fAngleZ;
	
	

	/*std::vector<CPlayer*>			m_pPlayersList;
	CPlayer*						m_pActivePlayer;
*/
	CThPSCamera*					m_pThPSCamera;
	CFPSCamera*						m_pFPSCamera;
	unsigned int					m_uiIndicePlayerCamera;
};

#endif __CLASS_TEST_PROCESS_H__