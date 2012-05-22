#pragma once

#ifndef __TRIGGER_MANAGER_CLASS_H__
#define __TRIGGER_MANAGER_CLASS_H__

#include <map>
#include <string>
#include "Utils\MapManager.h"
#include "Math\Vector3.h"
#include "Math\Color.h"

#include "PhysicTriggerReport.h"


// -- FOWARD Declarations -----
class CRenderManager;
class CPhysicUserData;
class CPhysicActor;
// ----------------------------

struct TriggerInstance
{
	std::string		Name;
	std::string		TriggerName;
	std::string		ActorName;
	std::string		LayerName;
	std::string		TriggerType;
	std::string		LuaCode;
	Vect3f			Position;
	Vect3f			Size;
	CColor			Color;
	float			Radius;
	uint32			Group;

	CPhysicActor *	pTriggerActor;

	TriggerInstance::TriggerInstance ()
		: pTriggerActor ( NULL )
	{}

	TriggerInstance::~TriggerInstance ()
	{}
};

class CTriggersManager : public CPhysicTriggerReport
{
public:
	//--- Init and End protocols------------------------------------------
					CTriggersManager		( void );
	virtual			~CTriggersManager		( void );

	//----Main Functions ------------------------------------------------
	void			Init					( void );
	void			Destroy					( void );
	bool			Load					( const std::string &_FileName );
	bool			Reload					( void );
	
	void			Update					( float _ElapsedTime );
	void			Render					( CRenderManager *_RM );
	
	static void		RegisterMethods			();

	//---- Main Interfaces ------------------------------------------------
	virtual void	OnEnter					( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );
	virtual	void	OnLeave					( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );
	virtual	void	OnStay					( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );

	//---- Public Interfaces ------------------------------------------------
	bool			ExistTrigger			( std::string _TriggerName );
	bool			ExistFisicTrigger		( std::string _FisicTriggerName );

	//---- Properties ( get & Set )---------------------------------------
	TriggerInstance	GetTrigger				( std::string _TriggerName );

	//----Private Functions -----------------------------------------------------
private:
	bool			LoadXML					( void );

	//---- Members -------------------------------------------------------
private:
	std::string								m_szFilename;		// Fitxer xml on trobaré els scripts a registrar

	std::map<std::string, TriggerInstance>	m_TriggersMap;
};

#endif __TRIGGER_MANAGER_CLASS_H__