//----------------------------------------------------
// CDebugOptions class
// Author: Marc Cuscullola
//
// Description: Shows a interface to configure the debug options of the project
//----------------------------------------------------
#pragma once
#ifndef _DEBUG_OPTIONS_H
#define _DEBUG_OPTIONS_H

class CRenderManager;
class CFontManager;

#include "DebugOptionsDefs.h"
#include "Math\Color.h"
#include "Utils\Active.h"
#include <string>
#include <map>
#include <vector>

class CDebugOptions : public CActive
{
public:
	CDebugOptions();
	virtual ~CDebugOptions();

	bool		Load			( const std::string &_Filename );
	bool		Reload			();
	void		CleanUp			();

	void		Render			( CRenderManager &RM, CFontManager &FM, CColor = colWHITE );

	uint16		GetActualPage	() const		{ return m_uNowPage; }
	
	//--- Set Methods -------
	void		SetBool			( bool value )		{ m_bBoolValue = value; }
	void		SetInt			( int value )		{ m_iIntValue = value; }
	void		SetFloat		( float value )		{ m_fFloatValue = value; }

private:
	bool			LoadFile			();
	TETypePage		String2TypePage		( const std::string &Type );
	TETypeOfValues	String2TypeValue	( const std::string &Type );
	void			RenderQuad			( CRenderManager &RM, const Vect2i &Init, const Vect2i &End );

	std::string							m_FileName;
	std::vector<SPage>					m_PagesVector;
	uint16								m_uNowPage;

	bool								m_bBoolValue;
	int									m_iIntValue;
	float								m_fFloatValue;
};

#endif

