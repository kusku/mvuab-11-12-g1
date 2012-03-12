//----------------------------------------------------
// CDebugGUIManager class
// Author: Marc Cuscullola
//
// Description: Manage the different information interfaces.
//----------------------------------------------------
#pragma once

#ifndef _DEBUG_GUI_MANAGER_H
#define _DEBUG_GUI_MANAGER_H

class CDebugOptions;
class CConsole;
class CRenderManager;
class CFontManager;
class CXMLTreeNode;
class CDebugRender;
class CLogRender;
class CTimer;

class CDebugGUIManager
{
public:
	CDebugGUIManager();
	~CDebugGUIManager();

	bool			Init			( /*CXMLTreeNode &_Node*/ );
	void			Destroy			();

	void			Update			( float _ElapsedTime );
	void			Render			( CRenderManager &_RM, CFontManager &_FM, CTimer *_Timer );

	//--- Get Methods ----------
	inline CDebugOptions*			GetDebugOptions			() const		{ return m_pDebugOptions; }
	inline CConsole*				GetConsole				() const		{ return m_pConsole; }	
	inline CDebugRender*			GetDebugRender			() const		{ return m_pDebugRender; }
	inline CLogRender*				GetLogRender			() const		{ return m_pLogRender; }

private:
	CDebugOptions			*m_pDebugOptions;
	CConsole				*m_pConsole;
	CDebugRender			*m_pDebugRender;
	CLogRender				*m_pLogRender;
};

#endif
