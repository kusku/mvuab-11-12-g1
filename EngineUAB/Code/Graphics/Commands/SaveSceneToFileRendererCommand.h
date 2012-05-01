#pragma once

#ifndef __CLASS_SAVE_SCREEN_TO_FILE_RENDERER_COMMAND_H__
#define __CLASS_SAVE_SCREEN_TO_FILE_RENDERER_COMMAND_H__

#include <string>
#include "Commands\SceneRendererCommand.h"


//---Foward Declarations---//
class CRenderManager;
class CXMLTreeNode;
//-------------------------//


class CSaveSceneToFileRendererCommand : public CSceneRendererCommand
{
public:
	//--- Init and End protocols------------------------------------------
			CSaveSceneToFileRendererCommand ( CXMLTreeNode &_atts );
	
	//----Funcions principals---------------------------------------
	void	Execute							( CRenderManager &_RM );

	//----Membres ---------------------------------------
private:
	std::string		m_FileName;
	std::string		m_Format;
};

#endif __CLASS_SAVE_SCREEN_TO_FILE_RENDERER_COMMAND_H__