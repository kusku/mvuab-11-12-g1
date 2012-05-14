#pragma once

#include "Utils\MapManager.h"

#include "Base.h"
#include "Core.h"

#include "StatesMachine\BaseGameEntity.h"
#include "StatesMachine\StateMachine.h"
#include "StatesMachine\State.h"

#include "RenderableObjects\AnimatedModel\AnimatedInstanceModel.h"

//--- Foward Declarations ---//
//class CState;
class CPlayerControler;
struct Telegram;

//---------------------------//

class CCharacter : public CBaseGameEntity //, public CMapManager<int>
{
public:
	// ------------- Constructors i Destructors --------------
									CCharacter			( int _Id );
									CCharacter			( int _Id, const std::string &_Name );
	virtual							~CCharacter			( void );


	//----Main Functions --------------------------------------
	virtual bool					Init				( const std::string &_Name, const Vect3f &_InitialPosicion );
	virtual bool					HandleMessage		( const Telegram& _Msg, bool _Logic = true, bool _Graphic = true  );		// Envia telegramas a las m�quinas de estados
	virtual bool					HandleMessage		( const Telegram& _Msg );
	virtual void					Update				( float _ElapsedTime );

	//----Functions -------------------------------------------


	//----Properties ( get & Set )-----------------------------
	virtual inline CStateMachine<CCharacter>*		GetLogicFSM			( void ) const			{ return m_pLogicStateMachine; }
	virtual inline CStateMachine<CCharacter>*		GetGraphicFSM		( void ) const			{ return m_pGraphicStateMachine; }
	virtual inline CPlayerControler*				GetControler		( void ) const			{ return m_pController; }
	virtual inline CAnimatedInstanceModel*			GetAnimatedModel	( void ) const			{ return m_pCurrentAnimatedModel; }

	//----Members )--------------------------------------------
private:
	bool							m_bIsOk;
	
protected:
	CStateMachine<CCharacter>	  *	m_pLogicStateMachine;		// Part de l�gica. Control de la m�quina d'estats del personatge
	CPlayerControler			  * m_pController;				// Part de f�sica. Control del player en el escenari
	CStateMachine<CCharacter>	  *	m_pGraphicStateMachine;		// Part de gr�fics. Control de la m�quina d'estats d'animaci� del personatge
	CAnimatedInstanceModel		  * m_pCurrentAnimatedModel;	// Part de gr�fics. Mant� un punt� a l'animaci� en curs
};

