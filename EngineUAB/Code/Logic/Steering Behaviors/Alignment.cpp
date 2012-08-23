#include "Alignment.h"
#include "Steering Behaviors\SteeringBehaviorsDefs.h"
#include "Steering Behaviors\SteeringEntity.h"

#include "PhysicsManager.h"
#include "PhysicController.h"
#include "PhysicUserData.h"
#include "PhysicActor.h"

#include "Math\Vector3.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------

CAlignment::CAlignment( void )
	: CSteering				( ::alignment )
	, m_AlignmentUpdated	( false )
	, m_UserDatas			( NULL )
{
}


CAlignment::~CAlignment(void)
{
	m_UserDatas.clear();
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CAlignment::CalculateSteering( CSteeringEntity *_pEntity )	
{
	// Registra la media de vector dirección de los vecinos
	Vect3f l_AverageHeading;
	l_AverageHeading.SetZero();

	// Registra el nº de agentes en el neighborhood ( barrio --> radio )
	int	   l_NeighborCount = 0;
		
	if ( m_AlignmentUpdated )
	{
		m_AlignmentUpdated = false;

		for ( unsigned int i = 0; i < m_UserDatas.size(); ++i )
		{
			if ( m_UserDatas[i]->GetName() != _pEntity->GetName() ) 
			{
				l_AverageHeading += m_UserDatas[i]->GetSteeringEntity()->GetHeading();

				++l_NeighborCount;
			}
		}		

		// si el barrio contiene uno o más entidades, hacemos la media de sus vectores de dirección
		if ( l_NeighborCount > 0 )
		{
			l_AverageHeading /= (float)l_NeighborCount;

			l_AverageHeading -= _pEntity->GetHeading();
		}
	}
	else
	{
		LOGGER->AddNewLog( ELL_ERROR, "CAlignment::CalculateSteering->Debe llamar primero al método UpdateParameters" );
	}

	// Retorno la fuerza
    return l_AverageHeading; 
}
   
void CAlignment::UpdateParameters ( const std::vector<CPhysicUserData*> &_UserDatas )
{
	m_UserDatas		   = _UserDatas;
	m_AlignmentUpdated = true;
}

