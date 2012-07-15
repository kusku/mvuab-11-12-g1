#pragma once

#ifndef _BOX_TRIGGER_H_
#define _BOX_TRIGGER_H_

#include "Triggers/Trigger.h"

//--Forward Declarations------
class CRenderManager;
class CXMLTreeNode;
//----------------------------

class CBoxTrigger : public CTrigger
{
public:
	//---Constructor & Destructor------------
	CBoxTrigger();
	~CBoxTrigger();

	//---Basic Methods-----------------------
	void	ReadData	( CXMLTreeNode &_Node );
	void	Init		();
	void	Update		( float _fElapsedTime );
	void	Render		( CRenderManager *_RM );

	//---Trigger Methods---------------------
	void	OnEnter		();
	void	OnStay		();
	void	OnExit		();

	//---Get & Set Methods-------------------
	void			SetSize		( const Vect3f & size )		{ m_Size = size; }
	const Vect3f&	GetSize		() const					{ return m_Size; }

private:
	Vect3f		m_Size;
};

#endif //_BOX_TRIGGER_H_
