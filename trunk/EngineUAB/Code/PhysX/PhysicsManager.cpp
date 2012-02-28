#define __DONT_INCLUDE_MEM_LEAKS__
#include "base.h"

#include "Assert.h"
#include "PhysicsManager.h"
//#include "Logger.h"
#include "ScriptManager.h"
//#include "Base/Math/Matrix44.h"

#include "XML\XMLTreeNode.h"

//#include "luabind.hpp"

////----PhysX Includes-------------
#undef min
#undef max
#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
////--------------------------------

#include "PhysicUserAllocator.h"
#include "PhysicCookingMesh.h"
#include "PhysicController.h"
#include "PhysicActor.h"
#include "PhysicSphericalJoint.h"
#include "PhysicRevoluteJoint.h"
#include "PhysicTriggerReport.h"
#include "PhysicFixedJoint.h"
#include "RenderManager.h"
#include "Exceptions\Exception.h"
#include "Logger\Logger.h"
#include "Base.h"

#include "Memory\MemLeaks.h"

#define MAX_ARISTAS 10

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CPhysicsManager::Init (const string& _physXConfig)
{
	m_szConfigFileName = _physXConfig;

	m_pMyAllocator = new CPhysicUserAllocator;
	assert(m_pMyAllocator);
	m_bIsOk = (m_pMyAllocator != NULL);
	if (m_bIsOk)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: Inicializando la libreria PhysX");
		// Initialize PhysicsSDK
		NxPhysicsSDKDesc desc;
		NxSDKCreateError errorCode = NXCE_NO_ERROR;
		m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, m_pMyAllocator, NULL, desc, &errorCode);

		/*Precompilation Directives*/
#if defined( _DEBUG )
#define USE_DEBUGGER
#ifdef USE_DEBUGGER
		m_pPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect("127.0.0.1");
#endif
#endif

		m_InitParams.m_fSkinWidth = 0.01f; //TODO: Borrar la línea
		m_bIsOk = (m_pPhysicsSDK != NULL);
		if (m_bIsOk)
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> Creado el PhysXSDK");
			LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> -------PhsX Settings---");
			LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> El valor del SkinWidth es: %f", m_InitParams.m_fSkinWidth);


			m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, m_InitParams.m_fSkinWidth);

			//CODI PER PRINTAR INFO DELS JOINTS
			m_pPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
			m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LIMITS, 1);
			m_pPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LOCAL_AXES, 1);
			m_pPhysicsSDK->setParameter(NX_CONTINUOUS_CD, 1);

			// Create a scene
			LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> El valor de la gravedad es: %f", m_InitParams.m_fGravity);
			NxSceneDesc sceneDesc;
			
			m_InitParams.m_fGravity = -9.81f; //TODO: Borrar línea
			sceneDesc.gravity = NxVec3(0.0f, m_InitParams.m_fGravity, 0.0f);
			m_pScene = m_pPhysicsSDK->createScene(sceneDesc);
			m_bIsOk = (m_pScene != NULL);
			if (m_bIsOk)
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> Solo hay un material, con los siguientes params");
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> DefaultMaterial->Restitution %f:",
					m_InitParams.m_Restitution_DefMat);
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> DefaultMaterial->StaticFriction %f:",
					m_InitParams.m_StaticFriction_DefMat);
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> DefaultMaterial->DynamicFriction %f:",
					m_InitParams.m_DynamicFriction_DefMat);
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> ----END PhsX Settings---");
				// Set default material
				//TODO: Borrar líneas
				m_InitParams.m_Restitution_DefMat = 0.5f;
				m_InitParams.m_StaticFriction_DefMat = 0.5f;
				m_InitParams.m_DynamicFriction_DefMat = 0.5f;

				NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0);
				defaultMaterial->setRestitution(m_InitParams.m_Restitution_DefMat);
				defaultMaterial->setStaticFriction(m_InitParams.m_StaticFriction_DefMat);
				defaultMaterial->setDynamicFriction(m_InitParams.m_DynamicFriction_DefMat);



				// Create a controllerManager
				m_pControllerManager = NxCreateControllerManager(m_pMyAllocator);
				m_bIsOk = (m_pControllerManager != NULL);
				if (m_bIsOk)
				{
					LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> Creado el controlador de caracteres");
					m_pCookingMesh = new CPhysicCookingMesh();
					assert(m_pCookingMesh);
					m_bIsOk = m_pCookingMesh->Init(m_pPhysicsSDK, m_pMyAllocator);
					if (m_bIsOk) {
						LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager::Init-> Creado el CookingMesh");
					}

				}// isOk m_pControllerManager?

			}//isOk m_pScene?

		}//isOk m_pPhysicsSDK ?

	}//isOk m_pMyAllocator ?

	if (!m_bIsOk)
	{
		std::string msg_error = "PhysicsManager::Init-> Error en la inicializacion de PhysX";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		Release();
		throw CException(__FILE__, __LINE__, msg_error);
	}
	/*else
	{
		CXMLTreeNode l_xmlPhysX;
		if(!l_xmlPhysX.LoadFile(_physXConfig.c_str()))
		{
			LOGGER->AddNewLog(ELL_WARNING, "PhysicsManager::Init->No se ha podido carger el fichero init de PhysX: \"%s\"", _physXConfig.c_str());
		}
		else
		{
			for(int i = 0; i < ECG_LAST_GROUP; ++i)
			{
				for(int j = i+1; j < ECG_LAST_GROUP; ++j)
				{
					m_pScene->setGroupCollisionFlag(i,j,false);
				}
			}

			LOGGER->AddNewLog(ELL_INFORMATION, "Carregant init del PhysX Manager \"%s\"", _physXConfig.c_str());
			int l_iNumC = l_xmlPhysX.GetNumChildren();
			for(int i = 0; i < l_iNumC; ++i)
			{
				CXMLTreeNode l_xmlCollision = l_xmlPhysX(i);
				if(strcmp(l_xmlCollision.GetName(), "collision") == 0)
				{
					string l_szGroup1 = l_xmlCollision.GetPszISOProperty("group1","",true);
					string l_szGroup2 = l_xmlCollision.GetPszISOProperty("group2","",true);
					int    l_iGroup1  = GetCollisionGroup(l_szGroup1);
					int    l_iGroup2  = GetCollisionGroup(l_szGroup2);

					m_CollisionMasks[l_iGroup1] |= 1 << l_iGroup1;
					m_CollisionMasks[l_iGroup1] |= 1 << l_iGroup2;

					m_CollisionMasks[l_iGroup2] |= 1 << l_iGroup1;
					m_CollisionMasks[l_iGroup2] |= 1 << l_iGroup2;

					m_pScene->setGroupCollisionFlag(l_iGroup1,l_iGroup2,true);
					LOGGER->AddNewLog(ELL_INFORMATION, "Colisió etre el grup %d (%s) i el grup %d (%s)", 
						l_iGroup1, l_szGroup1.c_str(), 
						l_iGroup2, l_szGroup2.c_str());
				}
				else if(!l_xmlCollision.IsComment())
				{
					LOGGER->AddNewLog(ELL_WARNING, "Element no reconegut \"%s\"", l_xmlCollision.GetName());
				}
			}
		}
	}*/

	return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CPhysicsManager::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CPhysicsManager::Release ()
{
	CHECKED_DELETE(m_pCookingMesh);
	if( m_pControllerManager != NULL )
	{
		m_pControllerManager->purgeControllers();
		NxReleaseControllerManager(m_pControllerManager);
	}
	if(m_pScene != NULL)
	{
		m_pPhysicsSDK->releaseScene(*m_pScene);
		m_pScene = NULL;
	}

	if(m_pPhysicsSDK != NULL)
	{				
		NxReleasePhysicsSDK(m_pPhysicsSDK);
		m_pPhysicsSDK = NULL;
	}
	CHECKED_DELETE(m_pMyAllocator);
}


//----------------------------------------------------------------------------
// Debug Render
//----------------------------------------------------------------------------
void CPhysicsManager::DebugRender (CRenderManager* render)
{
	assert( m_pScene != NULL );

	if( !m_bDebugRenderMode ) return; 

	// Render all actors in the scene
	int nbActors = m_pScene->getNbActors();
	NxActor** actors = m_pScene->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		DrawActor(actor, render);
	}
}

void CPhysicsManager::DrawActor (NxActor* actor, CRenderManager* render)
{

	CPhysicUserData* physicUserData = NULL;
	physicUserData =(CPhysicUserData*)actor->userData;
	//Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
	assert(physicUserData);
	if( !physicUserData->GetPaint())
	{
		return;
	}

	NxShape*const* shapes = actor->getShapes();
	NxU32 nShapes = actor->getNbShapes();

	nShapes = actor->getNbShapes();
	while (nShapes--)
	{
		switch(shapes[nShapes]->getType())
		{
		case NX_SHAPE_PLANE:
			{
				CColor color = physicUserData->GetColor();
				float distance = shapes[nShapes]->isPlane()->getPlane().d;
				NxVec3 normal =  shapes[nShapes]->isPlane()->getPlane().normal;
				Vect3f n(normal.x,normal.y,normal.z);
				render->DrawPlane(100.f, n, distance,color,40,40);
			}
			break;
		case NX_SHAPE_BOX:
			{
				NxF32 m_aux[16];
				shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
				Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12], 
					m_aux[1], m_aux[5], m_aux[9], m_aux[13], 
					m_aux[2], m_aux[6], m_aux[10], m_aux[14], 
					m_aux[3], m_aux[7], m_aux[11], m_aux[15]);

				render->SetTransform(m);
				NxVec3 boxDim = shapes[nShapes]->isBox()->getDimensions();
				CColor color = physicUserData->GetColor();
				render->DrawCube(Vect3f(boxDim.x*2,boxDim.y*2,boxDim.z*2), color);
				render->DrawCube(boxDim.y*2,color);
			}
			break;
		case NX_SHAPE_SPHERE:
			{
				NxF32 m_aux[16];
				shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
				Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12], 
					m_aux[1], m_aux[5], m_aux[9], m_aux[13], 
					m_aux[2], m_aux[6], m_aux[10], m_aux[14], 
					m_aux[3], m_aux[7], m_aux[11], m_aux[15]);

				render->SetTransform(m);
				NxReal radius = shapes[nShapes]->isSphere()->getRadius();
				CColor color = physicUserData->GetColor();
				render->DrawSphere(radius,MAX_ARISTAS,color);
			}
			break;
		case NX_SHAPE_CAPSULE:
			{
				NxF32 m_aux[16];
				shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
				Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12], 
					m_aux[1], m_aux[5], m_aux[9], m_aux[13], 
					m_aux[2], m_aux[6], m_aux[10], m_aux[14], 
					m_aux[3], m_aux[7], m_aux[11], m_aux[15]);

				Mat44f translation, total;
				translation.SetIdentity();
				render->SetTransform(m);

				const NxReal & radius = shapes[nShapes]->isCapsule()->getRadius();
				const NxReal & height = shapes[nShapes]->isCapsule()->getHeight();
				CColor color = physicUserData->GetColor();
				translation.Translate(Vect3f(0.f, (height*0.5f), 0.f));

				total = m * translation;
				render->SetTransform(total);
				render->DrawSphere(radius, MAX_ARISTAS, color);

				translation.Translate( Vect3f(0.f, -(height*0.5f), 0.f ));
				total = m * translation;
				render->SetTransform(total);
				render->DrawSphere(radius, MAX_ARISTAS, color);
			}
			break;
		case NX_SHAPE_CONVEX:

			break;
		case NX_SHAPE_MESH:
			{
				NxShape* mesh = shapes[nShapes];

				NxTriangleMeshDesc meshDesc;
				mesh->isTriangleMesh()->getTriangleMesh().saveToDesc(meshDesc);

				typedef NxVec3 Point;
				typedef struct _Triangle { NxU32 p0; NxU32 p1; NxU32 p2; } Triangle;

				NxU32 nbVerts = meshDesc.numVertices;
				NxU32 nbTriangles = meshDesc.numTriangles;

				Point* points = (Point *)meshDesc.points;
				Triangle* triangles = (Triangle *)meshDesc.triangles;

				CColor color = physicUserData->GetColor();
				NxF32 m_aux[16];
				mesh->getGlobalPose().getColumnMajor44(m_aux);
				Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12], 
					m_aux[1], m_aux[5], m_aux[9], m_aux[13], 
					m_aux[2], m_aux[6], m_aux[10], m_aux[14], 
					m_aux[3], m_aux[7], m_aux[11], m_aux[15]);

				render->SetTransform(m);

				Vect3f a,b,c;
				while(nbTriangles--)
				{
					a = Vect3f(points[triangles->p0].x, points[triangles->p0].y,points[triangles->p0].z);
					b = Vect3f(points[triangles->p1].x, points[triangles->p1].y,points[triangles->p1].z);
					c = Vect3f(points[triangles->p2].x, points[triangles->p2].y,points[triangles->p2].z);

					render->DrawLine(a, b, color);
					render->DrawLine(b, c, color);
					render->DrawLine(c, a, color);
					triangles++;

				}
			}
			break;
		case NX_SHAPE_WHEEL:
			{
				//TODO...
			}
			break;
		default:
			{
				//TODO...
			}
			break;
		}
	}
}


//----------------------------------------------------------------------------
// Run Physics
//----------------------------------------------------------------------------
void CPhysicsManager::Update (float elapsedTime)
{
	assert( m_pScene != NULL );
	assert( m_pControllerManager != NULL );

	// Start simulation (non blocking)
	m_pScene->simulate(elapsedTime); 

	// Fetch simulation results
	m_pScene->flushStream();
	//m_pScene->fetchResults(NX_RIGID_BODY_FINISHED,  true);


}

void CPhysicsManager::WaitForSimulation()
{
	m_pScene->fetchResults(NX_RIGID_BODY_FINISHED,  true);

	NxReal maxTimestep;
	NxTimeStepMethod method;
	NxU32 maxIter;
	NxU32 numSubSteps;

	m_pScene->getTiming(maxTimestep, maxIter, method, &numSubSteps);
	if(numSubSteps)
	{
		m_pControllerManager->updateControllers();
	}
}

void CPhysicsManager::SetTriggerReport (CPhysicTriggerReport* report)
{
	assert(m_pScene);
	assert(report);
	NxUserTriggerReport* nxUserTriggerReport = (NxUserTriggerReport*) report;
	m_pScene->setUserTriggerReport(report);
}

void CPhysicsManager::SetCollisionReport(CPhysicCollisionReport* report)
{
	assert(m_pScene);
	assert(report);
	NxUserContactReport* nxContactReport = (NxUserContactReport*) report;
	m_pScene->setUserContactReport(nxContactReport);
}

bool CPhysicsManager::AddPhysicActor (CPhysicActor* actor)
{
	assert(actor != NULL);
	assert( m_pScene != NULL );

	bool isOk = false;
	NxActor* nxActor;
	NxActorDesc* actorDesc = actor->GetActorDesc();

	assert(actorDesc!=NULL);
	nxActor = m_pScene->createActor( *actorDesc );
	if (nxActor!=NULL)
	{
		nxActor->userData = actor->GetUserData();
		actor->CreateActor(nxActor);
		isOk = true;
	}

	return isOk;
}

bool CPhysicsManager::ReleasePhysicActor (CPhysicActor* actor)
{
	assert(actor != NULL);
	assert(m_pScene != NULL);
	assert(m_pPhysicsSDK != NULL);

	bool isOk = false;
	NxActor* nxActor = actor->GetPhXActor();

	if( nxActor != 0)
	{
		NxArray<NxCCDSkeleton*> skeletons;
		for (NxU32 i = 0; i < nxActor->getNbShapes(); i++) 
		{
			NxShape* shape = nxActor->getShapes()[i];
			if (shape->getCCDSkeleton() != NULL) {
				skeletons.pushBack(shape->getCCDSkeleton());
			}
		}

		for (NxU32 i = 0; i < skeletons.size(); i++) 
		{
			m_pPhysicsSDK->releaseCCDSkeleton(*skeletons[i]);
		}
		m_pScene->releaseActor(*nxActor);
		nxActor = 0;
	}
	//TODO!!!!
	return true;
}



bool CPhysicsManager::AddPhysicSphericalJoint (CPhysicSphericalJoint* joint)
{
	assert(joint != NULL);
	assert( m_pScene != NULL );

	bool isOk = false;
	NxJoint* nxJoint;
	NxSphericalJointDesc* jointDesc = joint->GetPhXDescJoint();

	assert(jointDesc!=NULL);
	nxJoint = m_pScene->createJoint(*jointDesc);
	if (nxJoint!=NULL)
	{
		joint->CreateJoint(nxJoint);
		isOk = true;
	}

	return isOk;
}

bool CPhysicsManager::RelasePhysicSphericalJoint (CPhysicSphericalJoint* joint)
{
	assert(joint);
	assert(m_pScene);
	NxJoint* phXJoint = joint->GetPhXJoint();
	m_pScene->releaseJoint(*phXJoint);
	phXJoint = 0;
	return true;
}


bool CPhysicsManager::AddPhysicRevoluteJoint (CPhysicRevoluteJoint* joint)
{
	assert(joint != NULL);
	assert( m_pScene != NULL );

	bool isOk = false;
	NxJoint* nxJoint;
	NxRevoluteJointDesc* jointDesc = joint->GetPhXDescJoint();

	assert(jointDesc!=NULL);
	nxJoint = m_pScene->createJoint(*jointDesc);
	if (nxJoint!=NULL)
	{
		joint->CreateJoint(nxJoint);
		isOk = true;
	}

	return isOk;
}

bool CPhysicsManager::RelasePhysicRevoluteJoint (CPhysicRevoluteJoint* joint)
{
	assert(joint);
	assert(m_pScene);
	NxJoint* phXJoint = joint->GetPhXJoint();
	m_pScene->releaseJoint(*phXJoint);
	phXJoint = 0;
	return true;
}

bool CPhysicsManager::AddPhysicFixedJoint (CPhysicFixedJoint* joint)
{
	assert(joint != NULL);
	assert( m_pScene != NULL );

	bool isOk = false;
	NxJoint* nxJoint;
	NxFixedJointDesc* jointDesc = joint->GetPhXDescJoint();

	assert(jointDesc!=NULL);
	nxJoint = m_pScene->createJoint(*jointDesc);
	if (nxJoint!=NULL)
	{
		joint->CreateJoint(nxJoint);
		isOk = true;
	}

	return isOk;
}

bool CPhysicsManager::RelasePhysicFixedJoint (CPhysicFixedJoint* joint)
{
	assert(joint);
	assert(m_pScene);
	NxJoint* phXJoint = joint->GetPhXJoint();
	m_pScene->releaseJoint(*phXJoint);
	phXJoint = 0;
	return true;
}

bool CPhysicsManager::AddPhysicController (CPhysicController* controller)
{
	assert(controller != NULL);
	assert(m_pScene != NULL);
	assert(m_pControllerManager != NULL);

	bool isOK = false;
	NxController* nxController = controller->GetPhXController();
	NxCapsuleControllerDesc*	nxControllerDesc = controller->GetPhXControllerDesc();
	assert(nxControllerDesc!=NULL);
	assert(nxController==NULL); //Nos aseguramos que no hayan registrado ya un actor en la escena
	nxController = m_pControllerManager->createController(m_pScene, *nxControllerDesc );
	if (m_pControllerManager!= NULL)
	{
		controller->CreateController(nxController, m_pScene);
		nxController->getActor()->userData = controller->GetUserData();
		//NxShape*const* shape = nxController->getActor()->getShapes();
		//shape[0]->setGroup(controller->);

		isOK = true;
	}

	return isOK;

}

bool CPhysicsManager::ReleasePhysicController	(CPhysicController* controller)
{
	assert(controller != NULL);
	assert(m_pControllerManager != NULL);

	bool isOk = false;
	NxController* nxController = controller->GetPhXController();
	if (nxController != NULL)
	{
		m_pControllerManager->releaseController( *nxController );
	}

	//TODO!!!!
	return true;
}


NxCCDSkeleton* CPhysicsManager::CreateCCDSkeleton (float size)
{
	assert( m_pPhysicsSDK != NULL );

	NxU32 triangles[3 * 12] = { 0,1,3, 	0,3,2, 3,7,6, 3,6,2, 1,5,7, 1,7,3, 4,6,7, 4,7,5, 1,0,4,
		5,1,4,	4,0,2,	4,2,6	};

	NxVec3 points[8];

	//static mesh
	points[0].set( size, -size, -size);
	points[1].set( size, -size,  size);
	points[2].set( size,  size, -size);
	points[3].set( size,  size,  size);

	points[4].set(-size, -size, -size);
	points[5].set(-size, -size,  size);
	points[6].set(-size,  size, -size);
	points[7].set(-size,  size,  size);

	NxSimpleTriangleMesh stm;
	stm.numVertices = 8;
	stm.numTriangles = 6*2;
	stm.pointStrideBytes = sizeof(NxVec3);
	stm.triangleStrideBytes = sizeof(NxU32)*3;

	stm.points = points;
	stm.triangles = triangles;
	stm.flags |= NX_MF_FLIPNORMALS;

	return m_pPhysicsSDK->createCCDSkeleton(stm);
}


CPhysicUserData* CPhysicsManager::RaycastClosestActor (const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info )
{
	//NxUserRaycastReport::ALL_SHAPES
	assert(m_pScene != NULL);

	NxRay ray; 
	ray.dir =  NxVec3(dirRay.x, dirRay.y, dirRay.z);
	ray.orig = NxVec3(posRay.x, posRay.y, posRay.z);

	NxRaycastHit hit;
	NxShape* closestShape = NULL;

	closestShape = m_pScene->raycastClosestShape(ray, NX_ALL_SHAPES, hit, impactMask);
	if (!closestShape) 
	{
		//No hemos tocado a ningún objeto físico de la escena.
		return NULL;
	}
	NxActor* actor = &closestShape->getActor();
	CPhysicUserData* impactObject =(CPhysicUserData*)actor->userData;
	//Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
	assert(impactObject);

	info.m_fDistance	= hit.distance;
	info.m_Normal				= Vect3f(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z ); 
	info.m_CollisionPoint	= Vect3f(hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z ); 

	return impactObject;
}


CPhysicUserData* CPhysicsManager::RaycastClosestActorShoot (const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, SCollisionInfo& info, float _fPower)
{

	//NxUserRaycastReport::ALL_SHAPES
	assert(m_pScene != NULL);

	NxRay ray; 
	ray.dir =  NxVec3(dirRay.x, dirRay.y, dirRay.z);
	ray.orig = NxVec3(posRay.x, posRay.y, posRay.z);

	NxRaycastHit hit;
	NxShape* closestShape = NULL;

	closestShape = m_pScene->raycastClosestShape(ray, NX_ALL_SHAPES, hit, impactMask);
	if (!closestShape) 
	{
		//No hemos tokado a ningún objeto físico de la escena.
		return NULL;
	}
	NxActor* actor = &closestShape->getActor();
	CPhysicUserData* impactObject =(CPhysicUserData*)actor->userData;
	//Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle UserData
	assert(impactObject);

	info.m_fDistance	= hit.distance;
	info.m_Normal				= Vect3f(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z ); 
	info.m_CollisionPoint	= Vect3f(hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z ); 

	Vect3f l_vDirection(dirRay.x-posRay.x,dirRay.y-posRay.y,dirRay.z-posRay.z);
	l_vDirection.Normalize();

	NxVec3 l_vDirectionVec(dirRay.x,dirRay.y,dirRay.z); 
	NxF32 coeff = actor->getMass() * _fPower;
	actor->addForceAtLocalPos(l_vDirectionVec*coeff, NxVec3(0,0,0), NX_IMPULSE,true);


	return impactObject;
}

void CPhysicsManager::OverlapSphereActor (float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> &impactObjects, uint32 impactMask)
{
	assert(m_pScene);

	NxSphere worldSphere(NxVec3(posSphere.x,posSphere.y,posSphere.z), radiusSphere);
	NxU32 nbShapes = m_pScene->getNbDynamicShapes();
	NxShape** shapes = new NxShape* [nbShapes];
	for (NxU32 i = 0; i < nbShapes; i++)
	{
		shapes[i] = NULL;
	}

	m_pScene->overlapSphereShapes(worldSphere, NX_ALL_SHAPES, nbShapes, shapes, NULL,impactMask);

	for (NxU32 i = 0; i < nbShapes; i++) 
	{
		if( shapes[i] != NULL )
		{
			NxActor* actor = &shapes[i]->getActor();
			CPhysicUserData* physicObject = (CPhysicUserData*)actor->userData;
			//Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle ID
			assert(physicObject);	
			//Antes de meterlo comprobamos que no exista ya (un objeto fisico puede estar compuesto por varias shapes)
			std::vector<CPhysicUserData*>::iterator it(impactObjects.begin());
			std::vector<CPhysicUserData*>::iterator itEnd(impactObjects.end());
			bool find = false; 
			while (it!=itEnd)
			{
				CPhysicUserData* id = *it;
				if( id == physicObject)
					find = true;
				++it;
			}

			if(!find)
				impactObjects.push_back(physicObject);
		}
	}

	delete shapes;

}

void CPhysicsManager::OverlapSphereActorGrenade (float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects, float _fPower)
{
	assert(m_pScene);

	NxSphere worldSphere(NxVec3(posSphere.x,posSphere.y,posSphere.z), radiusSphere);
	NxU32 nbShapes = m_pScene->getNbDynamicShapes();
	NxShape** shapes = new NxShape* [nbShapes];
	for (NxU32 i = 0; i < nbShapes; i++)
	{
		shapes[i] = NULL;
	}

	//NX_DYNAMIC_SHAPES
	m_pScene->overlapSphereShapes(worldSphere, NX_DYNAMIC_SHAPES, nbShapes, shapes, NULL);

	for (NxU32 i = 0; i < nbShapes; i++) 
	{
		if( shapes[i] != NULL )
		{
			NxActor* actor = &shapes[i]->getActor();
			CPhysicUserData* physicObject = (CPhysicUserData*)actor->userData;
			//Si está petando aquí quiere decir que se ha registrado un objeto físico sin proporcionarle ID
			assert(physicObject);	
			//Antes de meterlo comprobamos que no exista ya (un objeto fisico puede estar compuesto por varias shapes)
			std::vector<CPhysicUserData*>::iterator it(impactObjects.begin());
			std::vector<CPhysicUserData*>::iterator itEnd(impactObjects.end());
			bool find = false; 
			while (it!=itEnd)
			{
				CPhysicUserData* id = *it;
				if( id == physicObject)
					find = true;
				++it;
			}

			if(!find)
			{
				impactObjects.push_back(physicObject);
				physicObject->SetColor(colRED);
				ApplyExplosion(actor,posSphere,radiusSphere,_fPower);
			}
		}
		//delete &shapes[i];
	}

	delete shapes;
	/*for (NxU32 i = 0; i < nbShapes; i++) 
	{
	delete &shapes[i];
	}*/
}

void CPhysicsManager::RegisterFunctions (CScriptManager* scriptManager)
{
	//lua_State* l_pLUAState = scriptManager->GetLuaState();

	//using namespace luabind;
	//
	//// ahora registramos lo que querramos
	// module(l_pLUAState)
	//   [
	//     // registramos la clase CPhysicsManager
	//		class_<CPhysicsManager>(CScriptRegister::SetClassName("CPhysicsManager"))


	//     // registramos su constructor
	//     .def(constructor<>())

	//     // registramos sus funciones publicas
	//		.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

	//		.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
	//					"Muestra todas las funciones de esta clase"),
	//					&CScriptRegister::Help)

	//     .def(	CScriptRegister::PushFunctionName("setDebugRender", "void", "bool flag",
	//					"Setea a true/false la visibilidad de todos los objetos físicos de la escena"),
	//					&CPhysicsManager::SetDebugRenderMode)

	//		.def(	CScriptRegister::PushFunctionName("getDebugRender", "bool", "void", 
	//					"Obtiene si se visualiza o no los objetos físicos de la escena"),	
	//					&CPhysicsManager::GetDebugRenderMode)
	//   ];
}

void CPhysicsManager::ApplyExplosion(NxActor* _pActor,const Vect3f& _vPosSphere, float _fEffectRadius, float _fPower)
{

	Vect3f l_vVelocityDirection;
	Vect3f l_vActorPosition;
	float l_fDistance;
	float l_fTotalPower;

	NxVec3 l_vPos = _pActor->getGlobalPosition();
	l_vActorPosition = Vect3f(l_vPos.x,l_vPos.y,l_vPos.z);

	l_vVelocityDirection = l_vActorPosition-_vPosSphere;
	l_vVelocityDirection.Normalize();

	l_fDistance = _vPosSphere.Distance(l_vActorPosition);
	l_fTotalPower = _fPower*((_fEffectRadius-l_fDistance)/_fEffectRadius);

	NxF32 coeff = _pActor->getMass() * l_fTotalPower;
	NxVec3 l_vDirection(l_vVelocityDirection.x,l_vVelocityDirection.y,l_vVelocityDirection.z);
	_pActor->addForceAtLocalPos(l_vDirection*coeff, NxVec3(0,0,0), NX_IMPULSE,true);

}



int GetCollisionGroup(const string& _szGroup)
{
	if(_szGroup == "escenari")
	{
		return ECG_ESCENARI;
	}
	else if(_szGroup == "personatge")
	{
		return ECG_PERSONATGE;
	}
	else if(_szGroup == "enemic")
	{
		return ECG_ENEMICS;
	}
	else if(_szGroup == "trigger")
	{
		return ECG_TRIGGERS;
	}
	else if(_szGroup == "trigger mal")
	{
		return ECG_MALGLOBAL;
	}
	else if(_szGroup == "cobertura")
	{
		return ECG_COBERTURES;
	}
	else if(_szGroup == "objecte dinamic")
	{
		return ECG_OBJECTES_DINAMICS;
	}
	else if(_szGroup == "explosio")
	{
		return ECG_EXPLOSIONS;
	}
	else if(_szGroup == "ray shoot")
	{
		return ECG_RAY_SHOOT;
	}
	else if(_szGroup == "ray shoot player")
	{
		return ECG_RAY_SHOOT_PLAYER;
	}
	else if(_szGroup == "force")
	{
		return ECG_FORCE;
	}
	else if(_szGroup == "camera")
	{
		return ECG_CAMERA;
	}
	else if(_szGroup == "ragdoll")
	{
		return ECG_RAGDOLL;
	}
	else if(_szGroup == "ragdoll player")
	{
		return ECG_RAGDOLL_PLAYER;
	}
	else if(_szGroup == "ray ia graph")
	{
		return ECG_RAY_IA_GRAPH;
	}
	else if(_szGroup == "vigia")
	{
		return ECG_VIGIA;
	}
	else
	{
		return 0;
	}
}
