#include "Scene.h"

#include "Base.h"
#include "Core.h"

#include "PhysicsManager.h"
#include "PhysicActor.h"
#include "PhysicTriggerReport.h"
#include "PhysicCookingMesh.h"
#include "PhysicUserData.h"


// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CScene::CScene( void )
{
}


CScene::~CScene( void )
{
	Done();
}

// -----------------------------------------
//				 MAIN METHODS 
// -----------------------------------------
void CScene::Done ( void )
{
	if ( IsOK ( ) )
	{
		Release();
		m_bIsOk = false;
	}
}

bool CScene::Init ( void )
{
	//CreateScene();
	return m_bIsOk;
}

void CScene::Release ( void )
{
}


void CScene::Update( float ElapsedTime )
{

}

void CScene::Render( CRenderManager *_RM )
{
}

void CScene::RenderScene( void )
{
}


// -----------------------------------------
//				   METHODS 
// -----------------------------------------
void CScene::CreateScene( void )
{
	// Creem el pla de suport
	CreateGrid();

	// Creem unes escales
	CreateStairs();

	// Box que fa de pla
	CreateBox( Vect3f(0.f, 0.0f, 0.f), Vect3f(50, 0.0001f, 50), 0.f, 10.f, true, ::ECG_OBJECTES_DINAMICS );
}

// ------------ Crea unes escales ----------
void CScene::CreateStairs ( void )
{
	float i;

	for ( i = 0; i < 20; i++ )
	{
		CreateBox ( Vect3f ( 10.f + i, -3.5f + i * 0.5f, 0), Vect3f(2.f,2.f,2.f), Vect3f(0.f,2.f,0.f), 10.f, true, ::ECG_ESCENARI );
		//SetActorCollisionGroup(box, GROUP_COLLIDABLE_NON_PUSHABLE);
	}
}

// ------------ Crea el pla base -----------
void CScene::CreateGrid ( void )
{
	// Dibuixem el pla
	m_pPhysicUserDataPla = new CPhysicUserData ( "PLA" );
	m_pPhysicUserDataPla->SetColor ( colBLACK );
	m_pPhysicUserDataPla->SetPaint ( true );
	
	m_pPlaneActor = new CPhysicActor ( m_pPhysicUserDataPla );
	m_pPlaneActor->AddPlaneShape ( Vect3f( 0.0f,1.0f,0.0f ), 0.01f, ::ECG_ESCENARI );
	//m_pPlaneActor->CreateBoxTrigger Trigger ( Vect3f( 0.0f,1.0f,0.0f ), Vect3f( 100.0f,0.1f,100.0f), ::ECG_ESCENARI );
	CORE->GetPhysicsManager ()->AddPhysicActor ( m_pPlaneActor );
}

CPhysicActor* CScene::CreateBox( const Vect3f & _Pos, const Vect3f &_BoxDim, const Vect3f &_LocalPos, float _Density, bool _StaticShape, ECollisionGroup _ColisionGroup, const CColor &_Color )
{   
	CPhysicUserData*	l_pPhysicUserDataBox;
	CPhysicActor*		l_pBoxActor;

	//Create a dynamic actor with a sphere shape.    
	l_pPhysicUserDataBox = new CPhysicUserData ( "BOX" );
	l_pPhysicUserDataBox->SetColor ( _Color );
	l_pPhysicUserDataBox->SetPaint ( true );

	l_pBoxActor = new CPhysicActor ( l_pPhysicUserDataBox );
	//l_pBoxActor->CreateBoxTrigger ( _Pos, _BoxDim, CollisionGroup::GROUP_COLLIDABLE_NON_PUSHABLE );
	l_pBoxActor->AddBoxSphape ( _BoxDim, _Pos, _LocalPos, NULL, _ColisionGroup );

	// Si es pot moure creem un body, en altra cas serà una esfera inmovil
	if ( !_StaticShape )
		l_pBoxActor->CreateBody ( _Density );

	CORE->GetPhysicsManager ()->AddPhysicActor ( l_pBoxActor );
	
	m_ActorVector.push_back ( l_pBoxActor );

	return l_pBoxActor;

	/*l_pBoxActor = NULL;
	l_pPhysicUserDataBox = NULL;*/
}

void CScene::CreateSphere(const Vect3f &_Pos, float _Radius, const Vect3f &_LocalPos, float _Density, const Vect3f &_VelocitatInicial, bool _StaticShape, ECollisionGroup _ColisionGroup, const CColor &_Color ) 
{   
	CPhysicUserData*	l_pPhysicUserDataBola;
	CPhysicActor*		l_pBolaActor;

	//Create a dynamic actor with a sphere shape.    
	l_pPhysicUserDataBola = new CPhysicUserData ( "BOLA" );
	l_pPhysicUserDataBola->SetColor ( _Color );
	l_pPhysicUserDataBola->SetPaint ( true );

	l_pBolaActor = new CPhysicActor ( l_pPhysicUserDataBola );

	// Si es pot moure creem un body, en altra cas serà una esfera inmovil
	if ( !_StaticShape )
		l_pBolaActor->CreateBody( _Density );

	// Si tinc velocitat inicial li fico
	if ( _VelocitatInicial == NULL )	
		l_pBolaActor->SetLinearVelocity ( l_pBolaActor->GetLinearVelocity() * _VelocitatInicial );

	// Creem la forma 
	l_pBolaActor->AddSphereShape ( _Radius, _Pos, _LocalPos, NULL, _ColisionGroup );

	CORE->GetPhysicsManager ()->AddPhysicActor ( l_pBolaActor );

	m_ActorVector.push_back ( l_pBolaActor );

	l_pBolaActor = NULL;
	l_pPhysicUserDataBola = NULL;
}

void CScene::CreatePlane( const Vect3f &_Pos, float _Distance, float _Density, const Vect3f &_LocalPos, bool _StaticShape, ECollisionGroup _ColisionGroup, const CColor &_Color )
{
	CPhysicUserData*	l_pPhysicUserPla;
	CPhysicActor*		l_pPlaneActor;

	//Create a dynamic actor with a sphere shape.    
	l_pPhysicUserPla = new CPhysicUserData ( "PLA" );
	l_pPhysicUserPla->SetColor ( _Color );
	l_pPhysicUserPla->SetPaint ( true );

	l_pPlaneActor = new CPhysicActor ( l_pPhysicUserPla );

	// Si es pot moure creem un body, en altra cas serà una esfera inmovil
	if ( !_StaticShape )
		l_pPlaneActor->CreateBody( _Density );

	// Creem la forma 
	l_pPlaneActor->AddPlaneShape ( Vect3f(0.0f,0.1f,0.0f), _Distance, _ColisionGroup );

	CORE->GetPhysicsManager ()->AddPhysicActor ( l_pPlaneActor );

	m_ActorVector.push_back ( l_pPlaneActor );

	l_pPlaneActor = NULL;
	l_pPhysicUserPla = NULL;
}
	
//void CMainFisicsProcess::CreateCapsule (const Vect3f &_Pos, float _Radius, const Vect3f &_LocalPos, float _Density, const Vect3f &_VelocitatInicial, bool _StaticShape, ECollisionGroup _ColisionGroup ) 
//{   
//	CPhysicUserData*	l_pPhysicUserDataBola;
//	CPhysicActor*		l_pBolaActor;
//
//	//Create a dynamic actor with a sphere shape.    
//	l_pPhysicUserDataBola = new CPhysicUserData ( "BOLA" );
//	l_pPhysicUserDataBola->SetColor ( colRED );
//	l_pPhysicUserDataBola->SetPaint ( true );
//
//	l_pBolaActor = new CPhysicActor ( l_pPhysicUserDataBola );
//
//	// Si es pot moure creem un body, en altra cas serà una esfera inmovil
//	if ( !_StaticShape )
//		l_pBolaActor->CreateBody( _Density );
//
//	// Si tinc velocitat inicial li fico
//	if ( _VelocitatInicial == NULL )	
//		l_pBolaActor->SetLinearVelocity ( l_pBolaActor->GetLinearVelocity() * _VelocitatInicial );
//
//	// Creem la forma 
//	l_pBolaActor->AddSphereShape ( _Radius, _Pos, _LocalPos, NULL, _ColisionGroup );
//
//	CORE->GetPhysicsManager ()->AddPhysicActor ( l_pBolaActor );
//
//	m_ActorVector.push_back ( l_pBolaActor );
//
//	l_pBolaActor = NULL;
//	l_pPhysicUserDataBola = NULL;
//}

void CScene::CreateWalls ( void )
{
	// Create walls
	const int NB		= 20;
	const int SIZE		= 2;
	const float MAXSIZE = NB * float(SIZE) * 2.0f;

	for( int i = 0; i < NB; i++ )
	{
		CreateBox( Vect3f ((float(i)/float(NB - 1)) * MAXSIZE - MAXSIZE * 0.5f, float(SIZE), MAXSIZE * 0.5f), (float)SIZE, 0.f, 10.f, true, ECG_ESCENARI );
	}

	for( int i = 0; i < NB; i++ )
	{
		CreateBox(Vect3f((float(i)/float(NB-1))*MAXSIZE - MAXSIZE*0.5f, float(SIZE), -MAXSIZE*0.5f), (float)SIZE, 0.f, 10.f, true, ECG_ESCENARI );
	}

	for( int i = 0; i < NB; i++ )
	{
		CreateBox(Vect3f(-MAXSIZE*0.5f, float(SIZE), (float(i)/float(NB-1))*MAXSIZE - MAXSIZE*0.5f), (float)SIZE, 0.f, 10.f, true, ECG_ESCENARI );
	}

	for( int i = 0; i < NB; i++ )
	{
		CreateBox(Vect3f(MAXSIZE*0.5f, float(SIZE), (float(i)/float(NB-1))*MAXSIZE - MAXSIZE*0.5f), (float)SIZE, 0.f, 10.f, true, ECG_ESCENARI );
	}
}

// Cooking Mesh  "D:/BoxAse.ASE"
void CScene::CreateASEMesh ( const std::string &_Filename, const std::string &_Name  )
{
	CPhysicUserData* l_pPhysicUserDataASEMesh;
	CPhysicActor*	 l_AseMeshActor;
	
	CPhysicCookingMesh* l_pMeshes = CORE->GetPhysicsManager()->GetCookingMesh();
	if ( l_pMeshes->CreateMeshFromASE ( _Filename, _Name ) )
	{
		l_pPhysicUserDataASEMesh = new CPhysicUserData( _Name );
		l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );
		l_AseMeshActor->AddMeshShape ( l_pMeshes->GetPhysicMesh(_Name), Vect3f (34.f, 1.f, 4.f ) );
		//m_AseMeshActor->CreateBody ( 10.f );
		CORE->GetPhysicsManager()->AddPhysicActor ( l_AseMeshActor );
	}

	l_pMeshes					= NULL;
	l_pPhysicUserDataASEMesh	= NULL;
	l_AseMeshActor				= NULL;
}
