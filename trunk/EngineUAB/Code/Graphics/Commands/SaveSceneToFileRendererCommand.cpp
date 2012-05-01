#include "SaveSceneToFileRendererCommand.h"
#include "RenderManager.h"
#include "XML\XMLTreeNode.h"

#if defined( _DEBUG )
	#include "Memory\MemLeaks.h"
#endif //defined(_DEBUG)

// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------

CSaveSceneToFileRendererCommand::CSaveSceneToFileRendererCommand ( CXMLTreeNode &_atts )
	: CSceneRendererCommand ( _atts )
{
	//<save_screen_to_file active="true" filename="C:\sortida.bmp" format="BMP" />
	SetActive ( _atts.GetBoolProperty ( "active", true, false ) );
	
	m_FileName = _atts.GetPszProperty ( "filename", "", false );
	m_Format = _atts.GetPszProperty ( "format", "BMP", false );
}

// -----------------------------------------
//			  MÈTODES PRINCIPALS
// -----------------------------------------

void CSaveSceneToFileRendererCommand::Execute ( CRenderManager &_RM )
{
	if( GetActive() )
	{
		_RM.DebugDumpBuffer ( m_FileName, "C:\\" );
	}
}