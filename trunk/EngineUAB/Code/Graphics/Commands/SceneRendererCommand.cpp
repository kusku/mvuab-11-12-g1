#include "SceneRendererCommand.h"
#include "SceneRendererCommandManager.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"
#include "Base.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif

// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------
CSceneRendererCommand::CSceneRendererCommand ( CXMLTreeNode &_Node )
{
	SetName   ( _Node.GetPszProperty  ( "name","",false ) );
	SetActive ( _Node.GetBoolProperty ( "active", true, false ) );

	if ( GetName ( ) == "" )
		SetName ( CORE->GetSceneRendererCommandManager()->GetNextName( _Node ) );
}

CSceneRendererCommand::~CSceneRendererCommand ( void )
{
}

// --------------------------------------------
//			   MÈTODES PRINCIPALS
// --------------------------------------------


// --------------------------------------------
//				    MÈTODES
// --------------------------------------------