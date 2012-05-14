#pragma once

#include "TestProcess.h"

class CScene;
class CCharactersManager;


class CMain : public CTestProcess
{
public:
	// ------------- Constructors i Destructors ----------------
					CMain(void);
					~CMain(void);

	// ------------- Mètodes Principals ---------------------
	bool			Init				( void );
	virtual void	Update				( float _ElapsedTime );
	virtual void	Render				( CRenderManager *_RM );

	void			UpdateInputs		( float _ElapsedTime );
	
	//----Properties ( get & Set )----------------------------------------
																		 
	//----Members --------------------------------------------------------
private:
	CCharactersManager		* m_pCharactersManager;
	CScene					* m_pScene;
};

