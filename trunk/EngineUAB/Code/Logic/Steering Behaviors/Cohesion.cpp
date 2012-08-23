#include "Cohesion.h"
#include "Seek.h"

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

CCohesion::CCohesion( void )
	: CSteering				( ::cohesion )
	, m_CohesionUpdated		( false )
	, m_UserDatas			( NULL )
	, m_pSeek				( NULL )
{
	m_pSeek = new CSeek();
}


CCohesion::~CCohesion(void)
{
	m_UserDatas.clear();
	CHECKED_DELETE(m_pSeek);
}

// -----------------------------------------
//				MAIN METHODS
// -----------------------------------------

Vect3f CCohesion::CalculateSteering( CSteeringEntity *_pEntity )	
{
	// Registra la media de vector dirección de los vecinos
	Vect3f l_CenterOfMass, l_SteeringForce;
	l_CenterOfMass.SetZero();
	l_SteeringForce.SetZero();

	// Registra el nº de agentes en el neighborhood ( barrio --> radio )
	int	   l_NeighborCount = 0;
		
	if ( m_CohesionUpdated )
	{
		m_CohesionUpdated = false;

		for ( unsigned int i = 0; i < m_UserDatas.size(); ++i )
		{
			std::string s = m_UserDatas[i]->GetName();
			if ( s.compare( _pEntity->GetName() ) != 0 )
			{
				l_CenterOfMass += m_UserDatas[i]->GetController()->GetPosition();

				++l_NeighborCount;
			}
		}		

		// si el barrio contiene uno o más entidades, hacemos la media de sus vectores de dirección
		if ( l_NeighborCount > 0 )
		{
			// El centro de massas es la media de la suma de posiciones
			l_CenterOfMass /= (float)l_NeighborCount;

			// Buscamos la posición 
			m_pSeek->SetTarget(l_CenterOfMass);
			l_SteeringForce = m_pSeek->CalculateSteering(_pEntity);
		}
	}
	else
	{
		LOGGER->AddNewLog( ELL_ERROR, "CCohesion::CalculateSteering->Debe llamar primero al método UpdateParameters" );
	}

	// Retorno la fuerza
    return l_SteeringForce; 
}
   
void CCohesion::UpdateParameters ( const std::vector<CPhysicUserData*> &_UserDatas )
{
	m_UserDatas			= _UserDatas;
	m_CohesionUpdated = true;
}

