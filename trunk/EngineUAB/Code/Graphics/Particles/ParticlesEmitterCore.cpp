#include "ParticlesEmitterCore.h"
#include "Textures\TextureManager.h"
#include "Textures\Texture.h"

#include "XML\XMLTreeNode.h"
#include "Math\Vector3.h"
#include "Logger\Logger.h"
#include "Base.h"
#include "Core.h"
#include <sstream>

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// -----------------------------------------
//		  CONSTRUCTORS / DESTRUCTOR
// -----------------------------------------
CParticlesEmitterCore::CParticlesEmitterCore( const CXMLTreeNode &_Node )
	: m_fLifeTime1			( 0.f )
	, m_fLifeTime2			( 0.f )
	, m_vPosition1			( Vect3f(0.f,0.f,0.f) )				// Aquesta s'actualitza en la instancia
	, m_vPosition2			( Vect3f(0.f,0.f,0.f) )				// Aquesta s'actualitza en la instancia
	, m_vVelocity			( Vect3f(0.f,0.f,0.f) )
	, m_fAngle				( 0.f )
	, m_fMinEmitRate		( 0.f )
	, m_fMaxEmitRate		( 0.f )
	, m_Color1				( CColor(0.f,0.f,0.f,0.f) )
	, m_Color2				( CColor(0.f,0.f,0.f,0.f) )
	, m_fMinSize			( 0.f )
	, m_fMaxSize			( 0.f )
	, m_vSpawnDir1			( Vect3f(0.f,0.f,0.f) )
	, m_vSpawnDir2			( Vect3f(0.f,0.f,0.f) )
	, m_bGravity			( false )
	, m_fNumNewPartsExcess	( 0.f )
	, m_bAlive				( true )
	, m_sTexture			( "" )
	, m_fKeytime			( 0.f )	
{
	CXMLTreeNode l_ParticlesEmitterNode = _Node;

	// Col·loco el nom
	SetName ( l_ParticlesEmitterNode.GetPszProperty("name") );

	// Col·loco els paràmetres del temps 0.0 en primera instancia pq després faré una copia dels seus valors pq si no canvien en el temps aquests es mantinguin i mantinguin la sequencia
	InitXMLTimeZero ( l_ParticlesEmitterNode );

	// Col·loco el altres temps a partir de les propietats en temps 0.0 
	InitXMLOtherTimes ( l_ParticlesEmitterNode );
}

CParticlesEmitterCore::~CParticlesEmitterCore(void)
{
	Destroy();
}

// -----------------------------------------
//			   MÈTODES PRINCIPALS
// -----------------------------------------
void CParticlesEmitterCore::Destroy ( void )
{
	CTemplatedVectorMapManager<CParticlesEmitterProperties>::Destroy();
}

void CParticlesEmitterCore::Init( void )
{}

// -----------------------------------------
//				MÈTODES PRIVATS
// -----------------------------------------
void CParticlesEmitterCore::InitXMLTimeZero( const CXMLTreeNode &_MainNode )
{
	CXMLTreeNode l_NodePrincipal = _MainNode;

	// Creo un patró de les propietats
	CParticlesEmitterProperties * l_InitialProperties = new CParticlesEmitterProperties();
	AddResource( ConvertFloatToString( TIME_CERO ), l_InitialProperties );
	uint16 l_TotalAttributesEmitter = l_NodePrincipal.GetNumChildren ();

	// Guardo també el temps d'aquestes propietats
	l_InitialProperties->SetKeyTime ( TIME_CERO );
	std::string l_Time = ConvertFloatToString (TIME_CERO); 
	
	for ( uint16 i = 0; i < l_TotalAttributesEmitter; ++i )
	{
		std::string l_EmitterNodeName = _MainNode(i).GetName();
		if ( l_EmitterNodeName == "color" ) 
		{
			m_Color1 = static_cast<CColor> ( _MainNode(i).GetVect4fProperty ( "color1", Vect4f ( 0.0, 0.0, 0.0, 0.0 ) ) );
			m_Color2 = static_cast<CColor> ( _MainNode(i).GetVect4fProperty ( "color2", Vect4f ( 0.0, 0.0, 0.0, 0.0 ) ) );
			
			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetColor1 ( m_Color1 );
			l_InitialProperties->SetColor2 ( m_Color2 );
		}
		if ( l_EmitterNodeName == "direction" ) 
		{
			m_vSpawnDir1 = static_cast<Vect3f> ( _MainNode(i).GetVect3fProperty( "d1", Vect3f ( 0.0, 0.0, 0.0 ) ) );
			m_vSpawnDir2 = static_cast<Vect3f> ( _MainNode(i).GetVect3fProperty( "d2", Vect3f ( 0.0, 0.0, 0.0 ) ) );
	
			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetSpawnDirection1 ( m_vSpawnDir1 );
			l_InitialProperties->SetSpawnDirection2 ( m_vSpawnDir2 );
		}
		if ( l_EmitterNodeName == "size" ) 
		{
			m_fMinSize = static_cast<float> ( _MainNode(i).GetFloatProperty( "mins", 0.0f ) );
			m_fMaxSize = static_cast<float> ( _MainNode(i).GetFloatProperty( "maxs", 0.0f ) );
	
			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetMinSize ( m_fMinSize );
			l_InitialProperties->SetMaxSize ( m_fMaxSize );
		}
		if ( l_EmitterNodeName == "life_time" ) 
		{
			m_fLifeTime1 = static_cast<float> ( _MainNode(i).GetFloatProperty( "life1", 0.0f ) );
			m_fLifeTime2 = static_cast<float> ( _MainNode(i).GetFloatProperty( "life2", 0.0f ) );
			
			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetLifeTime1 ( m_fLifeTime1 );
			l_InitialProperties->SetLifeTime2 ( m_fLifeTime2 );
		}
		if ( l_EmitterNodeName == "emit_rate" ) 
		{
			m_fMinEmitRate = static_cast<float> ( _MainNode(i).GetFloatProperty( "er1", 0.0f ) );
			m_fMaxEmitRate = static_cast<float> ( _MainNode(i).GetFloatProperty( "er2", 0.0f ) );
			
			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetMinRate ( m_fMinEmitRate );
			l_InitialProperties->SetMaxRate ( m_fMaxEmitRate );
		}
		if ( l_EmitterNodeName == "gravity" ) 
		{
			m_bGravity = static_cast<bool> ( _MainNode(i).GetBoolProperty( "gr", false ) );
			
			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetGravity ( m_bGravity );
		}
		if ( l_EmitterNodeName == "velocity" ) 
		{
			m_vVelocity = static_cast<Vect3f> ( _MainNode(i).GetVect3fProperty( "vel", Vect3f ( 0.0, 0.0, 0.0 ) ) );

			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetVelocity ( m_vVelocity );
		}
		if ( l_EmitterNodeName == "angle" ) 
		{
			m_fAngle = static_cast<float> ( _MainNode(i).GetFloatProperty( "ang", 0.0f ) );

			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetAngle ( m_fAngle );
		}
		if ( l_EmitterNodeName == "texture" ) 
		{
			m_sTexture = static_cast<std::string> ( _MainNode(i).GetPszProperty( "tex", "" ) );

			l_InitialProperties = GetResource ( l_Time );
			l_InitialProperties->SetTextureName ( m_sTexture );

			CTextureManager *l_TextureManager	= CORE->GetTextureManager();
			CTexture		*l_Texture			= l_TextureManager->GetTexture( m_sTexture );
			if ( l_Texture == l_TextureManager->GetNoTexture() )
			{
				m_sTexture = l_TextureManager->GetNoTextureName();
			}	
			
			l_Texture = NULL;
			l_TextureManager = NULL;
		}
	}
}
 
// Permet buscar el seguent menor temps però que sigui major del anterior que ja s'ha processat
float CParticlesEmitterCore::SearchNextMenorTime ( const CXMLTreeNode &_MainNode, float _MinimTime )
{
	float l_MenorTime = MAX_TIME_TO_SEARCH;
	
	CXMLTreeNode l_ParticlesEmitterNode = _MainNode;

	// Per cada atribut
	uint16 l_TotalAttributesEmitter = l_ParticlesEmitterNode.GetNumChildren ();
	for ( uint16 i = 0; i < l_TotalAttributesEmitter; ++i )
	{
		// Per cada atribut del CORE que trobi cerco tots  els seus fills/valors basats en el temps
		uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
		for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
		{
			float l_Time = static_cast<float> (l_ParticlesEmitterNode(i)(j).GetFloatProperty("t")); 
			// Cerco el temps menor però que alhora sigui major que l'anterior que ja he cercat
			if ( ( l_Time < l_MenorTime ) && ( l_Time > _MinimTime ) )
				l_MenorTime = l_Time;
		}
	}

	return l_MenorTime;
}	

void CParticlesEmitterCore::InitXMLOtherTimes( const CXMLTreeNode &_MainNode )
{
	CXMLTreeNode l_ParticlesEmitterNode = _MainNode;
	
	CParticlesEmitterProperties * l_InitialProperties = GetResource( ConvertFloatToString( TIME_CERO ) );
	
	float l_PreviousTime = TIME_CERO;
	float l_TimeToGetValues = TIME_CERO;

	// Recorro segons els temps cada node i si té el temps assignat pillo el valor
	while ( l_TimeToGetValues != MAX_TIME_TO_SEARCH )
	{
		// Triem quin temps és del que cerquem valors
		l_TimeToGetValues = SearchNextMenorTime ( l_ParticlesEmitterNode, l_PreviousTime );
		if ( l_TimeToGetValues == MAX_TIME_TO_SEARCH ) 
			return;

		uint16 l_TotalAttributesEmitter = l_ParticlesEmitterNode.GetNumChildren ();
		for ( uint16 i = 0; i < l_TotalAttributesEmitter; ++i )
		{
		
			// Per cada atribut del CORE que trobi 
			std::string l_EmitterNodeName = l_ParticlesEmitterNode(i).GetName();
			if ( l_EmitterNodeName == "color" ) 
			{
				// Busco els fills de color
				uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
				{
					float l_CurrentTime = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "t", 0.0f ) );
					// Només obtenim els valors del temps que busquem
					if ( l_CurrentTime == l_TimeToGetValues )
					{
						std::string l_Time = ConvertFloatToString (l_CurrentTime); //static_cast<std::string> (l_ParticlesEmitterNode(i)(j).GetPszProperty("t")); 
						m_Color1 = static_cast<CColor> ( l_ParticlesEmitterNode(i)(j).GetVect4fProperty ( "color1", Vect4f ( 0.0, 0.0, 0.0, 0.0 ) ) );
						m_Color2 = static_cast<CColor> ( l_ParticlesEmitterNode(i)(j).GetVect4fProperty ( "color2", Vect4f ( 0.0, 0.0, 0.0, 0.0 ) ) );

						// Busco les propietats per temps si ja existeixen i sino el creo i l'afegeixo
						CParticlesEmitterProperties * l_Properties = GetResource ( l_Time );
						if ( l_Properties == NULL )
						{
							l_InitialProperties = GetPreviousCore ( l_Time );
							l_Properties = new CParticlesEmitterProperties( l_InitialProperties );
							AddResource( l_Time, l_Properties );
						}

						l_Properties->SetColor1 ( m_Color1 );
						l_Properties->SetColor2 ( m_Color2 );
						l_Properties->SetKeyTime ( l_ParticlesEmitterNode(i)(j).GetFloatProperty("t") );
					}
				}
			}
			if ( l_EmitterNodeName == "direction" ) 
			{
				// Busco els fills de color
				uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
				{
					float l_CurrentTime = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "t", 0.0f ) );
					// Només obtenim els valors del temps que busquem
					if ( l_CurrentTime == l_TimeToGetValues )
					{
						std::string l_Time = ConvertFloatToString (l_CurrentTime); //static_cast<std::string> (l_ParticlesEmitterNode(i)(j).GetPszProperty("t")); 
						m_vSpawnDir1 = static_cast<Vect3f> ( l_ParticlesEmitterNode(i)(j).GetVect3fProperty( "dir1", Vect3f ( 0.0, 0.0, 0.0 ) ) );
						m_vSpawnDir2 = static_cast<Vect3f> ( l_ParticlesEmitterNode(i)(j).GetVect3fProperty( "dir2", Vect3f ( 0.0, 0.0, 0.0 ) ) );

						// Busco les propietats per temps si ja existeixen i sino el creo i l'afegeixo
						CParticlesEmitterProperties * l_Properties = GetResource ( l_Time );
						if ( l_Properties == NULL )
						{
							l_InitialProperties = GetPreviousCore ( l_Time );
							l_Properties = new CParticlesEmitterProperties( l_InitialProperties );
							AddResource( l_Time, l_Properties );
						}
				
						l_Properties->SetSpawnDirection1 ( m_vSpawnDir1 );
						l_Properties->SetSpawnDirection2 ( m_vSpawnDir2 );
						float t = l_ParticlesEmitterNode(i)(j).GetFloatProperty("t");
						l_Properties->SetKeyTime ( l_ParticlesEmitterNode(i)(j).GetFloatProperty("t") );
					}
				}
			}
			if ( l_EmitterNodeName == "size" ) 
			{
				// Busco els fills de color
				uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
				{
					float l_CurrentTime = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "t", 0.0f ) );
					// Només obtenim els valors del temps que busquem
					if ( l_CurrentTime == l_TimeToGetValues )
					{
						std::string l_Time = ConvertFloatToString (l_CurrentTime); //static_cast<std::string> (l_ParticlesEmitterNode(i)(j).GetPszProperty("t")); 
						m_fMinSize = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "mins", 0.0f ) );
						m_fMaxSize = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "maxs", 0.0f ) );
			
						// Busco les propietats per temps si ja existeixen i sino el creo i l'afegeixo
						CParticlesEmitterProperties * l_Properties = GetResource ( l_Time );
						if ( l_Properties == NULL )
						{
							l_InitialProperties = GetPreviousCore ( l_Time );
							l_Properties = new CParticlesEmitterProperties( l_InitialProperties );
							AddResource( l_Time, l_Properties );
						}

						l_Properties->SetSpawnDirection1 ( m_fMinSize );
						l_Properties->SetSpawnDirection2 ( m_fMaxSize );
						l_Properties->SetKeyTime ( l_ParticlesEmitterNode(i)(j).GetFloatProperty("t") );
					}
				}
			}
			/*if ( l_EmitterNodeName == "life_time" ) 
			{
				m_fLifeTime1 = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "life1", 0.0f ) );
				m_fLifeTime2 = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "life2", 0.0f ) );
			}*/
			if ( l_EmitterNodeName == "emit_rate" ) 
			{
				// Busco els fills de color
				uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
				{
					float l_CurrentTime = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "t", 0.0f ) );
					// Només obtenim els valors del temps que busquem
					if ( l_CurrentTime == l_TimeToGetValues )
					{
						std::string l_Time = ConvertFloatToString (l_CurrentTime); //static_cast<std::string> (l_ParticlesEmitterNode(i)(j).GetPszProperty("t")); 
						m_fMinEmitRate = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "er1", 0.0f ) );
						m_fMaxEmitRate = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "er2", 0.0f ) );
			
						// Busco les propietats per temps si ja existeixen i sino el creo i l'afegeixo
						CParticlesEmitterProperties * l_Properties = GetResource ( l_Time );
						if ( l_Properties == NULL )
						{
							l_InitialProperties = GetPreviousCore ( l_Time );
							l_Properties = new CParticlesEmitterProperties( l_InitialProperties );
							AddResource( l_Time, l_Properties );
						}

						l_Properties->SetSpawnDirection1 ( m_fMinEmitRate );
						l_Properties->SetSpawnDirection2 ( m_fMaxEmitRate );
						l_Properties->SetKeyTime ( l_ParticlesEmitterNode(i)(j).GetFloatProperty("t") );
					}
				}
			}
			if ( l_EmitterNodeName == "gravity" ) 
			{
				// Busco els fills de color
				uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
				{
					float l_CurrentTime = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "t", 0.0f ) );
					// Només obtenim els valors del temps que busquem
					if ( l_CurrentTime == l_TimeToGetValues )
					{
						std::string l_Time = ConvertFloatToString (l_CurrentTime); //static_cast<std::string> (l_ParticlesEmitterNode(i)(j).GetPszProperty("t")); 
						// Busco les propietats per temps si ja existeixen i sino el creo i l'afegeixo
						CParticlesEmitterProperties * l_Properties = GetResource ( l_Time );
						if ( l_Properties == NULL )
						{
							l_InitialProperties = GetPreviousCore ( l_Time );
							l_Properties = new CParticlesEmitterProperties( l_InitialProperties );
							AddResource( l_Time, l_Properties );
						}

						l_Properties->SetGravity ( m_bGravity );
						l_Properties->SetKeyTime ( l_ParticlesEmitterNode(i)(j).GetFloatProperty("t") );
					}
				}
			}
			if ( l_EmitterNodeName == "velocity" ) 
			{
				// Busco els fills de color
				uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
				{
					float l_CurrentTime = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "t", 0.0f ) );
					// Només obtenim els valors del temps que busquem
					if ( l_CurrentTime == l_TimeToGetValues )
					{
						std::string l_Time = ConvertFloatToString (l_CurrentTime); //static_cast<std::string> (l_ParticlesEmitterNode(i)(j).GetPszProperty("t")); 
						m_vVelocity = static_cast<Vect3f> ( l_ParticlesEmitterNode(i)(j).GetVect3fProperty( "v", Vect3f ( 0.0, 0.0, 0.0 ) ) );
			
						// Busco les propietats per temps si ja existeixen i sino el creo i l'afegeixo
						CParticlesEmitterProperties * l_Properties = GetResource ( l_Time );
						if ( l_Properties == NULL )
						{
							l_InitialProperties = GetPreviousCore ( l_Time );
							l_Properties = new CParticlesEmitterProperties( l_InitialProperties );
							AddResource( l_Time, l_Properties );
						}

						l_Properties->SetVelocity ( m_vVelocity );
						l_Properties->SetKeyTime ( l_ParticlesEmitterNode(i)(j).GetFloatProperty("t") );
					}
				}
			}
			if ( l_EmitterNodeName == "angle" ) 
			{
				// Busco els fills de color
				uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
				{
					float l_CurrentTime = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "t", 0.0f ) );
					// Només obtenim els valors del temps que busquem
					if ( l_CurrentTime == l_TimeToGetValues )
					{
						std::string l_Time = ConvertFloatToString (l_CurrentTime); //static_cast<std::string> (l_ParticlesEmitterNode(i)(j).GetPszProperty("t")); 
						m_fAngle = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "ang", 0.0f ) );
			
						// Busco les propietats per temps si ja existeixen i sino el creo i l'afegeixo
						CParticlesEmitterProperties * l_Properties = GetResource ( l_Time );
						if ( l_Properties == NULL )
						{
							l_InitialProperties = GetPreviousCore ( l_Time );
							l_Properties = new CParticlesEmitterProperties( l_InitialProperties );
							AddResource( l_Time, l_Properties );
						}

						l_Properties->SetAngle ( m_fAngle );
						l_Properties->SetKeyTime ( l_ParticlesEmitterNode(i)(j).GetFloatProperty("t") );
					}
				}
			}
			if ( l_EmitterNodeName == "texture" ) 
			{
				// Busco els fills de color
				uint16 l_TotalSubAttributesEmitter = l_ParticlesEmitterNode(i).GetNumChildren ();
				for ( uint16 j = 0; j < l_TotalSubAttributesEmitter; ++j )
				{
					float l_CurrentTime = static_cast<float> ( l_ParticlesEmitterNode(i)(j).GetFloatProperty( "t", 0.0f ) );
					// Només obtenim els valors del temps que busquem
					if ( l_CurrentTime == l_TimeToGetValues )
					{
						std::string l_Time = ConvertFloatToString (l_CurrentTime); //static_cast<std::string> (l_ParticlesEmitterNode(i)(j).GetPszProperty("t")); 
						m_sTexture = static_cast<std::string> ( l_ParticlesEmitterNode(i)(j).GetPszProperty( "tex", "" ) );
			
						// Busco les propietats per temps si ja existeixen i sino el creo i l'afegeixo
						CParticlesEmitterProperties * l_Properties = GetResource ( l_Time );
						if ( l_Properties == NULL )
						{
							l_InitialProperties = GetPreviousCore ( l_Time );
							l_Properties = new CParticlesEmitterProperties( l_InitialProperties );
							AddResource( l_Time, l_Properties );
						}

						CTextureManager *l_TextureManager	= CORE->GetTextureManager();
						CTexture		*l_Texture			= l_TextureManager->GetResource ( m_sTexture );
						if ( l_Texture == NULL )
						{
							l_Texture = new CTexture();
							l_Texture->SetName ( m_sTexture );
							if ( l_Texture->Load( m_sTexture ) )
							l_TextureManager->AddResource( m_sTexture, l_Texture );
						}
						else
							m_sTexture = l_TextureManager->GetNoTextureName();
			
						l_Texture = 0;
						l_TextureManager = 0;

						l_Properties->SetKeyTime ( l_ParticlesEmitterNode(i)(j).GetFloatProperty("t") );
					}
				}
			}
		}
		l_PreviousTime = l_TimeToGetValues;
	}
}		

std::string CParticlesEmitterCore::ConvertFloatToString	( float _Value )
{
	std::stringstream out;

	out << _Value;
	std::string str = out.str();
	return ( str );
}

float CParticlesEmitterCore::CutAndRoundNumberToNDecimals (float dValue, int nDec) 
{ 
	float dTemp = 0, dFract = 0, dInt = 0, dRes = 0; 

	int nCount = 0; 

	dTemp = dValue * pow( 10.f,nDec ); 

	dFract = modf (dTemp, &dInt); 

	if (dFract >= 0.5) 
	dInt++; 

	dRes = dInt/pow(10.f,nDec); 

	return dRes;


	//int i = 0;
	//float j;
	//j = 1.6667f;
	//j=j*10; /* 16.677 */
	//i=ceil(j); /*17*/ 
	//i=floor(j);/*16*/
	//j=i/10; /*1.7*/ j=i/10; /*1.6*/
	//return j;
} 


// -----------------------------------------
//				 PROPIETATS 
// -----------------------------------------

CParticlesEmitterProperties * CParticlesEmitterCore::GetPreviousCore ( const std::string &_Time )
{
	// Si tenim 1 o 2 elements ja ser que el valor de temps 0.0 és el previ i que ja el tinc inclòs
	if ( m_ResourcesVector.size() < 2 )
		return m_ResourcesVector[0];

	// el altra cas el busco exactament i si no el trobo buscaré el resources amb el temps més proper però que no sigui més gran que el temps passat
	TMapResources::iterator l_It = m_ResourcesMap.find ( _Time );
	if ( l_It != m_ResourcesMap.end () )
	{
		--l_It;
		CParticlesEmitterProperties *l_Resource = ((*(&(l_It)._Ptr->_Myval)).second).m_Value;
		return l_Resource; //static_cast<CParticlesEmitterProperties *> ( l_It->second () );
	}
	else 
	{
		// En altra cas cerquem
		TMapResources::iterator l_It = m_ResourcesMap.begin();
		for ( ; l_It != m_ResourcesMap.end (); l_It++ )
		{
			std::string l = (l_It)->first;
			if ( l > _Time ) 
			{
				l_It--;
				return ( l_It->second.m_Value );
			}
		}

		if ( l_It == m_ResourcesMap.end () )
		{
			l_It--;
			return (l_It->second.m_Value) ;
		}

		//while ( l_It->first < l_End->first )
		//{
		//	if ( ( (l_It)->first) > _Time ) 
		//	//if ( ((*(&(l_It)._Ptr->_Myval)).second).m_Value > _Time ) 
		//	{
		//		return NULL;
		//		//return ( ((*(&(--l_It)._Ptr->_Myval)).second).m_Value > _Time );
		//	}

		//}
		//((*(&(l_It)._Ptr->_Myval)).second).m_Value
		//for ( ; l_It < l_End ; ++l_It )
		//{
		//////	
		////////	GetResourceById(
		//}
		return NULL;
	}

	return NULL;
}

TEmitterAtTime CParticlesEmitterCore::GetPropertiesAtTime ( float _Time )
{
	//_Time = round( _Time );
	_Time = CutAndRoundNumberToNDecimals( _Time, 1 );

	//sprintf(_Time,"%.1f\n",_Time);
	TEmitterAtTime l_PropietatsEmissor;
	l_PropietatsEmissor.m_fLifeTime1			= GetLifeTime1AtTime		( _Time );
	l_PropietatsEmissor.m_fLifeTime2			= GetLifeTime2AtTime		( _Time );
	l_PropietatsEmissor.m_bAlive				= IsALiveAtTime				( _Time );
	l_PropietatsEmissor.m_Color1				= GetColor1AtTime			( _Time );
	l_PropietatsEmissor.m_Color2				= GetColor2AtTime			( _Time );
	l_PropietatsEmissor.m_fMaxEmitRate			= GetMaxRateAtTime			( _Time );
	l_PropietatsEmissor.m_fMinEmitRate			= GetMinRateAtTime			( _Time );
	l_PropietatsEmissor.m_fMaxSize				= GetMaxSizeAtTime			( _Time );
	l_PropietatsEmissor.m_fMinSize				= GetMinSizeAtTime			( _Time );
	l_PropietatsEmissor.m_fNumNewPartsExcess	= GetNumExcessAtTime		( _Time );
	l_PropietatsEmissor.m_sTexture				= GetTextureNameAtTime		( _Time );
	l_PropietatsEmissor.m_vSpawnDir1			= GetSpawnDirection1AtTime  ( _Time );
	l_PropietatsEmissor.m_vSpawnDir2			= GetSpawnDirection2AtTime  ( _Time );
	l_PropietatsEmissor.m_vPosition1			= GetPosition1AtTime		( _Time );
	l_PropietatsEmissor.m_vPosition2			= GetPosition2AtTime		( _Time );
	l_PropietatsEmissor.m_fAngle				= GetAngleAtTime			( _Time );
	l_PropietatsEmissor.m_vVelocity				= GetVelocityAtTime			( _Time );
	l_PropietatsEmissor.m_fKeyTime				= GetKeyTime				( _Time );

	return l_PropietatsEmissor;
}

TEmitterAtTime CParticlesEmitterCore::GetPropertiesAtIndex ( size_t _Index )
{
	TEmitterAtTime l_PropietatsEmissor;
	l_PropietatsEmissor = m_ResourcesVector[_Index]->GetProperties();

	return l_PropietatsEmissor;
}
