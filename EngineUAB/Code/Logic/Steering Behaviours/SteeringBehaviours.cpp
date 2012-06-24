#include <algorithm>
#include "SteeringBehaviours.h"
#include "SteeringBehavioursDefs.h"

#include "Seek.h"
#include "Flee.h"
#include "Pursuit.h"
#include "Arrive.h"

#include "Scripting\ScriptManager.h"
#include "Base.h"

#if defined (_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CSteeringBehaviours::CSteeringBehaviours( float _MaxForce )
	: m_SteeringForce		( Vect2f(0.f, 0.f) )
	, m_pSeek				( NULL )
	, m_pPursuit			( NULL )
	, m_pArrive				( NULL )
	, m_MaxForce			( _MaxForce )
{
	m_Behaviors = new std::list<eBehaviorType>();

	Initialize();
}

CSteeringBehaviours::~CSteeringBehaviours(void)
{

}


// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------
void CSteeringBehaviours::Initialize( void )
{
	m_Behaviors->clear();
	
	m_WeightObstacleAvoidance	= 0.82f;
	m_WeightSeek				= 0.22f;
	m_WeightEvade				= 0.22f;
	m_WeightFlee				= 0.22f;
	m_WeightArrive				= 0.22f;
	m_WeightPursuit				= 0.22f;
	m_WeightWander				= 0.22f;
}

void CSteeringBehaviours::Destroy( void )
{
	m_Behaviors->clear();
}

const Vect2f& CSteeringBehaviours::Update( float _ElapsedTime, CSteeringEntity * _pEntity)  
{
    m_SteeringForce.SetZero();

    if ( HasBehavior(::seek) )
        m_SteeringForce += m_pSeek->CalculateSteering(_pEntity) * m_WeightSeek;

	if ( HasBehavior(::flee) )
		m_SteeringForce += m_pFlee->CalculateSteering(_pEntity) * m_WeightFlee;

	 if ( HasBehavior(::arrive) )
        m_SteeringForce += m_pArrive->CalculateSteering(_pEntity) * m_WeightArrive;

	 if ( HasBehavior(::pursuit) )
        m_SteeringForce += m_pPursuit->CalculateSteering(_pEntity) * m_WeightPursuit;

    //if (this.HasBehavior(BehaviorType.evade))
    //    this._steeringForce += this._evade.CalculateSteering(entity) * this.weightEvade;

    //if (this.HasBehavior(BehaviorType.wander))
    //{
    //    this._wander._gameTime = gameTime;
    //    this._steeringForce += this._wander.CalculateSteering(entity) * this.weightWander;
    //}

    //if (this.HasBehavior(BehaviorType.obstacle_avoidance))
    //    this._steeringForce += this._obstacleAvoidance.CalculateSteering(entity) * this.weightObstacleAvoidance;
    //        
    //// Devolvemos la fuerza producira por el steering truncada a la limitación del valor máximo
    //return GameHelper.VectorHelper.TruncateVector(this._steeringForce, this._maxForce);
	//const Vect2f& v = Truncate(m_SteeringForce, m_MaxForce);
	return m_SteeringForce;
}

//template<typename T>
//inline Vector2<T> Truncate( float _Max )
//{
//	if ((*this).Length() > _Max)
//    {
//        (*this).Normalize();
//
//        (*this) *= _Max;
//    }
//
//    return Vector2<T>(*this);
//}

inline Vect2f CSteeringBehaviours::Truncate( Vect2f v, float _Max )
{
	if ( v.Length() > _Max )
    {
		v.Normalize();

        v *= _Max;
    }

    return v;
}

// -----------------------------------------
//				MÈTODES
// -----------------------------------------
 
void CSteeringBehaviours::AddBehavior( CSeek *_pSteering ) 
{
    if ( !HasBehavior(_pSteering->GetType() ) )
		m_Behaviors->push_back(_pSteering->GetType());

    m_pSeek = _pSteering;
}

void CSteeringBehaviours::AddBehavior( CPursuit *_pSteering ) 
{
    if ( !HasBehavior(_pSteering->GetType() ) )
		m_Behaviors->push_back(_pSteering->GetType());

    m_pPursuit = _pSteering;
}

void CSteeringBehaviours::AddBehavior( CArrive *_pSteering ) 
{
    if ( !HasBehavior(_pSteering->GetType() ) ) 
		m_Behaviors->push_back(_pSteering->GetType());

    m_pArrive = _pSteering;
}

void CSteeringBehaviours::RegisterLUAMethods( void )
{
	lua_State* l_pLuaState = SCRIPT->GetLuaState();
	
	module(l_pLuaState) [
		class_<CSteeringBehaviours>("CSteeringBehaviours")
			.def("has_behavior", &CSteeringBehaviours::HasBehavior)
			.def("update", &CSteeringBehaviours::Update)
			.def("add_behaviour", (void(CSteeringBehaviours::*)(CSeek*)) &CSteeringBehaviours::AddBehavior)
			.def("add_behaviour", (void(CSteeringBehaviours::*)(CPursuit*)) &CSteeringBehaviours::AddBehavior)
			.def("add_behaviour", (void(CSteeringBehaviours::*)(CArrive*)) &CSteeringBehaviours::AddBehavior)
			.property("seek", &CSteeringBehaviours::GetSeek, &CSteeringBehaviours::SetSeek)
			.property("pursuit", &CSteeringBehaviours::GetPursuit, &CSteeringBehaviours::SetPursuit)
			.property("arrive", &CSteeringBehaviours::GetArrive, &CSteeringBehaviours::SetArrive)
	];

	CSteering::RegisterLUAMethods();
	CSeek::RegisterLUAMethods();
	CPursuit::RegisterLUAMethods();
	CArrive::RegisterLUAMethods();
}

// -----------------------------------------
//				PROPERTIES
// -----------------------------------------

int CSteeringBehaviours::HasBehavior( eBehaviorType _Type )  
{
	tBehaviourType::iterator l_Found = m_Behaviors->begin();
	
	int l_NumBehavioursFound = 0;

	while (true) 
	{
		l_Found = std::find ( l_Found, m_Behaviors->end(), _Type );
		if (l_Found == m_Behaviors->end()) break;

		l_NumBehavioursFound++;
		l_Found++;
	}
    
	return l_NumBehavioursFound;
}


CSeek* CSteeringBehaviours::GetSeek( void ) 
{
	if ( m_pSeek == NULL && HasBehavior(::seek) )
	{
			m_pSeek = new CSeek();
    }

	return m_pSeek;
}

CPursuit* CSteeringBehaviours::GetPursuit( void ) 
{
	if ( m_pPursuit == NULL && HasBehavior(::pursuit) )
	{
			m_pPursuit = new CPursuit( ::normal, 10.f );
    }

	return m_pPursuit;
}

CArrive* CSteeringBehaviours::GetArrive( void )
{
	if ( m_pArrive == NULL && HasBehavior(::arrive) )
	{
			m_pArrive = new CArrive( ::normal, 10.f );
    }

	return m_pArrive;
}