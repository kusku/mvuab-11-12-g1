#pragma once

#ifndef __TRIGGER_MANAGER_CLASS_H__
#define __TRIGGER_MANAGER_CLASS_H__

#include <map>
#include <string>
#include "PhysicTriggerReport.h"

// -- FOWARD Declarations -----
class CRenderManager;
class CTrigger;
// ----------------------------

class CTriggersManager : public CPhysicTriggerReport
{
public:
	//--- Init and End protocols------------------------------------------
	CTriggersManager	();
	~CTriggersManager	();

	//----Main Functions ------------------------------------------------
	void			Init					();
	void			Destroy					();
	
	void			Update					( float _ElapsedTime );
	void			Render					( CRenderManager *_RM );
	
	bool			Load					( const std::string &_FileName );
	bool			Reload					();

	//---- Main Interfaces ------------------------------------------------
	virtual void	OnEnter					( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );
	virtual	void	OnLeave					( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );
	virtual	void	OnStay					( CPhysicUserData* _Entity_Trigger1, CPhysicUserData* _Other_Shape );

	//----Private Functions -----------------------------------------------------
private:
	bool			LoadXML					();

	//---- Members -------------------------------------------------------
private:
	std::string		m_szFilename;		// Fitxer xml on trobaré els scripts a registrar

	typedef std::map<std::string, CTrigger*> TTRIGGERMAP;
	TTRIGGERMAP m_TriggerMap;				
};

#endif //__TRIGGER_MANAGER_CLASS_H__