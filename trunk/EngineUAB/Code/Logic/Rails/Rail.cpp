#include "Rail.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"
#include "Math\MathTypes.h"
#include "Object3D.h"
#include <assert.h>

#if defined (_DEBUG)
#include "Memory/MemLeaks.h"
#endif

//--------------------------------------------------
CRail::CRail()
	: m_bHasFinished(false)
	, m_bHasStarted(false)
	, m_fVelocity(1.0f)
	, m_RailState(STOP)
	, m_pObject(NULL)
	, m_uNextPoint(0)
{

}

//--------------------------------------------------
CRail::CRail( CXMLTreeNode &_Node )
	: m_bHasFinished(false)
	, m_bHasStarted(false)
	, m_fVelocity(1.0f)
	, m_RailState(STOP)
	, m_pObject(NULL)
	, m_uNextPoint(0)
{
	uint16 l_PointCount = _Node.GetNumChildren();
	for(uint16 i=0; i<l_PointCount; ++i)
	{
		std::string l_Type = _Node(i).GetName();
		if( l_Type == "point" )
		{
			Vect3f l_Point  =_Node(i).GetVect3fProperty("value", v3fZERO);

			m_PointVector.push_back( l_Point );
		}
	}
}

//--------------------------------------------------
CRail::~CRail()
{
	m_PointVector.clear();
}

//--------------------------------------------------
void CRail::Update( float _fElapsedTime )
{
	if( m_RailState == PLAY && !m_bHasFinished)
	{
		assert( m_pObject );

		if(m_bHasStarted)
		{
			Vect3f l_Position	= m_pObject->GetPosition();
			Vect3f l_Dir		= m_PointVector[m_uNextPoint] - m_PointVector[m_uNextPoint - 1];
			l_Dir.Normalize();

			l_Position = l_Position + l_Dir * m_fVelocity * _fElapsedTime;

			//Mira si nos pasamos del punto y tiene que corregir la dirección
			Vect3f l_CorrectDirection = m_PointVector[m_uNextPoint] - l_Position;
			l_CorrectDirection.Normalize();

			float angle = l_Dir.Dot(l_CorrectDirection);
			if(angle < -0.95f)
			{
				//Mira si es el último punto
				if( (m_uNextPoint + 1) == m_PointVector.size() )
				{
					m_RailState = STOP;
					m_bHasStarted = false;
					m_bHasFinished = true;
					return;
				}

				//Necesita corregir la dirección
				++m_uNextPoint;
				l_Dir = m_PointVector[m_uNextPoint] - m_PointVector[m_uNextPoint - 1];
				l_Position = l_Position + l_Dir * ( m_fVelocity / 2.f) * _fElapsedTime;
			}

			////Calcula el Yaw
			////---------------------
			Vect2f l_XZDir	=	Vect2f( l_Dir.x, l_Dir.z );
			l_XZDir.Normalize();

			float l_Yaw		=	l_XZDir.Dot( Vect2f(1.f, 0.f) );
			l_Yaw			=	mathUtils::ACos( l_Yaw );
			if( l_XZDir.y < 0.f )
			{
				l_Yaw = -l_Yaw;
			}

			////Calcula el Pitch
			////---------------------
			float l_Pitch = 0.f;
			if(l_Dir.x != 0.f)
			{
				Vect2f l_XYDir = Vect2f(l_Dir.x, l_Dir.y);
				l_XYDir.Normalize();

				if(l_Dir.x > 0.f)
				{
					l_Pitch = l_XYDir.Dot(Vect2f(1.f, 0.f));
					l_Pitch = - mathUtils::ACos(l_Pitch);
				}
				else
				{
					l_Pitch = l_XYDir.Dot(Vect2f(-1.f, 0.f));

					if( l_Dir.y < 0.f)
						l_Pitch = - mathUtils::ACos(l_Pitch);
					else
						l_Pitch = mathUtils::ACos(l_Pitch);
				}
			}
			else
			{
				Vect2f l_YZDir = Vect2f(l_Dir.y, l_Dir.z);
				l_YZDir.Normalize();

				if(l_Dir.z > 0.f)
				{
					l_Pitch = l_YZDir.Dot(Vect2f(0.f, 1.f));
					l_Pitch = - mathUtils::ACos(l_Pitch);
				}
				else
				{
					l_Pitch = l_YZDir.Dot(Vect2f(0.f, -1.f));
					l_Pitch = mathUtils::ACos(l_Pitch);
				}
			}
			

			//l_Pitch = 0.0f;

			//Asigna la nueva posición y los nuevos ángulos
			m_pObject->SetPosition( l_Position );
			m_pObject->SetYaw( l_Yaw );
			m_pObject->SetPitch( l_Pitch );
		}
		else
		{
			m_pObject->SetPosition( m_PointVector[0] );
			m_bHasStarted = true;
			m_uNextPoint = 1;
		}
	}
}

//--------------------------------------------------
void CRail::Start()
{
	if( m_RailState == STOP )
	{
		m_bHasStarted = false;
		m_bHasFinished = false;
	}

	m_RailState = PLAY;
}

//--------------------------------------------------
void CRail::Stop()
{
	m_RailState = STOP;

	m_bHasStarted = false;
	m_bHasFinished = false;

	m_pObject->SetPosition( m_PointVector[0] );
}

//--------------------------------------------------
void CRail::Pause()
{
	m_RailState = PAUSE;
}

//--------------------------------------------------
void CRail::Render( CRenderManager &_RM )
{
	Vect3f l_PreviousPoint;
	Vect3f l_CurrentPoint;
	uint16 l_uCount = m_PointVector.size();
	if( l_uCount > 0 )
	{
		l_CurrentPoint		= m_PointVector[0];
		l_PreviousPoint		= l_CurrentPoint;

		//Aplica la transformación correspondiente
		Mat44f l_TransformMatrix = m44fIDENTITY;
		l_TransformMatrix.Translate( l_CurrentPoint );
		_RM.SetTransform( l_TransformMatrix );

		//Dibuja el vértice inicial como una esfera
		_RM.DrawSphere(0.4f, 10, colBLACK);

		for(uint16 i=1; i<l_uCount; ++i)
		{
			l_CurrentPoint = m_PointVector[i];
			
			//Aplica la transformación correspondiente
			l_TransformMatrix = m44fIDENTITY;
			_RM.SetTransform( l_TransformMatrix );

			//Dibuja el raíl
			_RM.DrawLine(l_PreviousPoint, l_CurrentPoint, colBLACK);

			l_PreviousPoint = l_CurrentPoint;
		}
	}
}