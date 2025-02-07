//----------------------------------------------------------------------------------
// CFontManager class
// Author: Enric Vergara
//
// Description:
// Esta clase se encargará de todo lo relacionado con fuentes y pintar texto
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_FONT_MANAGER_H_
#define INC_FONT_MANAGER_H_

#include "Math\MathTypes.h"
#include "Math\Color.h"
#include <d3dx9.h>
#include <string>
#include <map>
#include <vector>

//---Forward Declarations--
class CRenderManager;
//-------------------------

class CFontManager
{
public:
	//--- Init and End protocols------------------------------------------
	CFontManager(): m_bIsOk(false), m_sPathFile("") {}
	virtual ~CFontManager() { Done(); } 

	bool		            Init			        (CRenderManager* rm);
	void		            Done			        ();
	bool		            IsOk			        () const { return m_bIsOk; }

	void							Init							();
	bool							ReloadTTFs						();
	bool							LoadTTFs						(const std::string& pathFile);
	int32							GetTTF_Id						(const std::string& name);
	uint32							CreateFont						(uint32 size, bool bold, bool italica, const std::string& fontName, bool replaceDefault = false);
	uint32							DrawDefaultText   				(uint32 x, uint32 y, CColor color, const char * format, ... );
	uint32							DrawText						(uint32 x, uint32 y, CColor color, uint32 idFont, const char * format, ... );
	uint32							DrawLiteral						(uint32 x, uint32 y, const std::string & lit);
	uint32							SizeX							(const char * format, uint32 idFont = 0);
	uint32							SizeY							(const char * format, uint32 idFont = 0);
	const std::string&				GetTTFName						( uint32 id ) const		{ return m_vTTFsFiles[id]; }
	uint32							GetNumFonts						() const				{ return m_vTTFsFiles.size(); }

private:
	void							Release		();

private:
	bool				    		m_bIsOk;		     					 // Initialization boolean control
	std::vector<LPD3DXFONT>			m_Fonts;										// Font vector
	std::vector<std::string>		m_vTTFsFiles;								// TTF Files loaded
	std::map<std::string, uint32>	m_TTFs;											// Id of TTFs
	LPDIRECT3DDEVICE9				m_pD3DDevice;								// direct3d device
	std::string						m_sPathFile;
};

#endif //INC_FONT_MANAGER_H_