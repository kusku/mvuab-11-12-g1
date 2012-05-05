#pragma once

#include "EngineProcess.h"
class CGUIManager;
class CRenderManager;

class CGUIEditorProcess : public CEngineProcess
{
public:
					CGUIEditorProcess	( void );
					~CGUIEditorProcess	( void );

	bool			Init				( void );
	void			Done				( void );
	void			Update				( float _ElapsedTime );
	void			Render				( CRenderManager &_RM );

	virtual void	Release				( void );

	CGUIManager*	GetGUIManager		( void ) const				{ return m_pGUIManager; }
protected:

	CGUIManager		*m_pGUIManager;
};

