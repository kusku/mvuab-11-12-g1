//----------------------------------------------------------------------------------
// CWwiseSoundManager class
// Author: Marc Cuscullola
//
// Description:
// Clase que gestiona los sonidos y encapsula Wwise
//----------------------------------------------------------------------------------
#pragma once

#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <string>
#include <AK/SoundEngine/Common/AkCallback.h>
#include "Utils\TemplatedVectorMapManager.h"
#include "Speaker.h"

//--- Foward Declarations ---
class CListener;
class CRenderManager;
//---------------------------

class CSoundManager : public CTemplatedVectorMapManager<CSpeaker>
{
public:	
	//--- Constructor & Destructor -------------	
					CSoundManager();
					~CSoundManager();

	//--- Methods ------------------------------
	bool			Init		();
	void			Terminate	();
	void			Update		();
	void			Render		( CRenderManager *_RM );

	bool			Load		( const std::string &soundbanks_filename, const std::string &speakers_filename );
	bool			Reload		();

	//--- Wwise Controls -----------------------
	bool			LoadSoundBank			( const std::string &bank );
	void			PlayEvent				( const std::string &event_name );
	void			PlayEvent				( const std::string &speaker_name, const std::string &event_name );
	void			SetSwitch				( const std::string &switch_name, const std::string &container_name );
	void			SetRTPCValue			(  const std::string &rtpc_name, float value );

	CSpeaker*		CreateSpeaker			( const std::string &_Name );

	//--- Properties ---------------------------
	inline void		SetPath		( const std::string &path )		{ m_Path = path; }
	inline void		SetListener	( CListener* listener )			{ m_pListener = listener; }

	inline const std::string&	GetPath		() const		{ return m_Path; }
	inline CListener*			GetListener	() const		{ return m_pListener; }		

	inline uint16				GetSpeakerCount () const	{ return m_ResourcesVector.size(); }

private:
	bool	InitBanks			();
	bool	LoadSoundBanksXML	();
	bool	LoadSpeakersXML		();

private:
	std::string		m_Path;
	std::string		m_SoundBanksFilename;
	std::string		m_SpeakersFilename;

	CListener	*m_pListener;
};

#endif