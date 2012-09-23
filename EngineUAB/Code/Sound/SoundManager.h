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
	enum eCallbackType
	{
		EndOfEvent					= 0x0001,	///< Callback triggered when reaching the end of an event.
		EndOfDynamicSequenceItem	= 0x0002,	///< Callback triggered when reaching the end of a dynamic sequence item.
		Marker						= 0x0004,	///< Callback triggered when encountering a marker during playback.
		Duration					= 0x0008,	///< Callback triggered when the duration of the sound is known by the sound engine.

		SpeakerVolumeMatrix			= 0x0010,   ///< Callback triggered at each frame, letting the client modify the speaker volume matrix

		MusicPlayStarted			= 0x0080,	///< Callback triggered when a "Play" or "Seek" command has been executed ("Seek" commands are issued from AK::SoundEngine::SeekOnEvent()). Applies to objects of the Interactive-Music Hierarchy only.
	
		MusicSyncBeat				= 0x0100,	///< Enable notifications on Music Beat.
		MusicSyncBar				= 0x0200,	///< Enable notifications on Music Bar.
		MusicSyncEntry				= 0x0400,	///< Enable notifications on Music Entry Cue.
		MusicSyncExit				= 0x0800,	///< Enable notifications on Music Exit Cue.

		MusicSyncGrid				= 0x1000,	///< Enable notifications on Music Grid.
		MusicSyncUserCue			= 0x2000,	///< Enable notifications on Music Custom Cue.

		MusicSyncPoint				= 0x4000,	///< Enable notifications on Music switch transition synchronisation point.

		MusicSyncAll				= 0xff00,	///< Use this flag if you want to receive all notifications concerning AK_MusicSync registration.
		CallbackBits				= 0xffff,	///< Bitmask for all callback types.

		// Not a callback type, but needs to be part of same bitfield for AK::SoundEngine::PostEvent().
		EnableGetSourcePlayPosition	= 0x10000,	///< Enable play position information for use by AK::SoundEngine::GetSourcePlayPosition().
		EnableGetMusicPlayPosition	= 0x20000	///< Enable play position information of music objects, queried via AK::MusicEngine::GetPlayingSegmentInfo().
	};

	/// Callback information structure used as base for all notifications handled by AkCallbackFunc.
	/// \sa 
	/// - AK::SoundEngine::PostEvent()
	/// - \ref soundengine_events
	struct sCallbackInfo
	{
		void *			pCookie;		///< User data, passed to PostEvent()
		int				gameObjID;		///< Game object ID
	};

	/// Callback information structure corresponding to AK_EndOfEvent and AK_MusicPlayStarted.
	/// \sa 
	/// - AK::SoundEngine::PostEvent()
	/// - \ref soundengine_events
	struct sEventCallbackInfo : public sCallbackInfo
	{
		unsigned int	playingID;		///< Playing ID of Event, returned by PostEvent()
		unsigned int	eventID;		///< Unique ID of Event, passed to PostEvent()
	};

	/// Callback information structure corresponding to AK_Marker.
	/// \sa 
	/// - AK::SoundEngine::PostEvent()
	/// - \ref soundengine_events
	/// - \ref soundengine_markers
	struct sMarkerCallbackInfo : public AkEventCallbackInfo
	{
		unsigned int	uIdentifier;		///< Cue point identifier
		unsigned int	uPosition;			///< Position in the cue point (unit: sample frames)
		const char*		strLabel;			///< Label of the marker, read from the file
	};

	/// Callback information structure corresponding to AK_Duration.
	/// \sa 
	/// - AK::SoundEngine::PostEvent()
	/// - \ref soundengine_events
	struct sDurationCallbackInfo : public AkEventCallbackInfo
	{
		float			fDuration;				///< Duration of the sound (unit: milliseconds )
		float			fEstimatedDuration;		///< Estimated duration of the sound depending on source settings such as pitch. (unit: milliseconds )
		unsigned int	audioNodeID;			///< Audio Node ID of playing item
	};

	/// Callback information structure corresponding to AK_EndOfDynamicSequenceItem.
	/// \sa 
	/// - AK::SoundEngine::PostEvent()
	/// - AK::SoundEngine::DynamicSequence::Open()
	/// - \ref soundengine_events
	struct sDynamicSequenceItemCallbackInfo : public sCallbackInfo
	{
		unsigned int	playingID;			///< Playing ID of Dynamic Sequence, returned by AK::SoundEngine:DynamicSequence::Open()
		unsigned int	audioNodeID;		///< Audio Node ID of finished item
		void*			pCustomInfo;		///< Custom info passed to the DynamicSequence::Open function
	};

	/// Callback information structure corresponding to AK_SpeakerVolumeMatrix.
	/// Volumes are read-write, linear values normalized to the [0..1] range. Source channels are following the 
	/// order defined in AkCommonDefs.h. This callback will be called at every audio frame for every playing sound 
	/// registered to the callback; it is crucial that the processing done in the callback be lightweight and non-blocking.
	/// \sa 
	/// - AK::SoundEngine::PostEvent()
	/// - \ref soundengine_events
	/*struct SpeakerVolumeMatrixCallbackInfo : public AkEventCallbackInfo
	{
		AkSpeakerVolumes * pVolumesDry[ AK_VOICE_MAX_NUM_CHANNELS ]; ///< For each channel in the sound, pointer to AkSpeakerVolume describing contribution to destination channels (dry mix). 
		AkSpeakerVolumes * pVolumesWet[ AK_VOICE_MAX_NUM_CHANNELS ]; ///< For each channel in the sound, pointer to AkSpeakerVolume describing contribution to destination channels (wet mix). Only present if bIsEnvironmental is true
		AkUInt32 uChannelMask;			///< Speaker setup of the sound (for example, AK_SPEAKER_SETUP_STEREO)
		bool bIsEnvironmental;			///< True if the sound is feeding an environmental bus
	};*/

	/// Callback information structure corresponding to Ak_MusicSync.
	/// If you need the Tempo, you can compute it using the fBeatDuration
	/// Tempo (beats per minute) = 60/fBeatDuration
	/// \sa 
	/// - \ref soundengine_events
	/// - AK::SoundEngine::PostEvent()
	/// - \ref soundengine_music_callbacks
	struct sMusicSyncCallbackInfo : public sCallbackInfo
	{
		unsigned int	playingID;			///< Playing ID of Event, returned by PostEvent()
		eCallbackType	musicSyncType;		///< Would be either AK_MusicSyncEntry, AK_MusicSyncBeat, AK_MusicSyncBar, AK_MusicSyncExit, AK_MusicSyncGrid, AK_MusicSyncPoint or AK_MusicSyncUserCue.
		float			fBeatDuration;		///< Beat Duration in seconds.
		float			fBarDuration;		///< Bar Duration in seconds.
		float			fGridDuration;		///< Grid duration in seconds.
		float			fGridOffset;		///< Grid offset in seconds.
		char *			pszUserCueName;		///< Cue name (UTF-8 string). Set for notifications AK_MusicSyncUserCue. NULL if cue has no name.
	};
	
	
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
	virtual void	CallbackReaction		( AkCallbackType in_eType, AkCallbackInfo *in_pCallbackInfo );
	virtual void	CallbackEndReaction		( eCallbackType in_eType, sCallbackInfo* in_pCallbackInfo );
	virtual void	CallbackDurationReaction( eCallbackType in_eType, sDurationCallbackInfo * in_pCallbackInfo );
	CSpeaker*		CreateSpeaker			( const std::string &_Name );
	void			MyCallbackReaction		( AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo );

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