#pragma once

#ifndef __FORMATION_MANAGER_CLASS_H__
#define __FORMATION_MANAGER_CLASS_H__

#include <map>
#include <string>

// -- FOWARD Declarations -----
class CCharacter;
// ----------------------------

class CFormationManager
{
public:

	// Almacena la asignación de un simple caracter a un slot
	struct SlotAssignment
	{
		CCharacter * Character;
		float		 SlotNumber;
	};
		
	//--- Init and End protocols------------------------------------------
	CFormationManager	();
	~CFormationManager	();

	//----Main Functions ---------------------------------------------------
	void			Init					();
	void			Destroy					();
	
	void			Update					( float _ElapsedTime );					// Actualitza les asignacions dels slots als caracters
	//void			Render					( CRenderManager *_RM );
	
	bool			Load					( const std::string &_FileName );
	bool			Reload					();

	//---- Main Interfaces ------------------------------------------------
	
	//----Public methods --------------------------------------------------

	//----Private methods -------------------------------------------------
private:
	bool			LoadXML					();

	//---- Members --------------------------------------------------------
private:
	std::string		m_szFilename;		// Fitxer xml on trobaré els scripts a registrar

	float			m_DriftOffset;		// Representa el desplaáment per els slots actuals

	typedef std::map<std::string, SlotAssignment> m_SlotAssignments;

};

#endif //__FORMATION_MANAGER_CLASS_H__