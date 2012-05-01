//----------------------------------------------------------------------------------
// CSoundManager class
// Author: Enric Vergara
// Changes: Jordi Arenas
// Description:
// This secures availability of the necessary sounds functions.
// It internally uses the OpenAL library.
//----------------------------------------------------------------------------------
#pragma once

#ifndef __CLASS_SOUND_MANAGER_H__
#define __CLASS_SOUND_MANAGER_H__

#include <string>
#include <vector>
#include <map>

// ----- openAL -------------//
#include "al.h"
// --------------------------//

#include "Utils/MapManager.h"
//#include "Script/ScriptRegister.h"
#include "Utils/LerpAnimator1D.h"
#include "Math/Vector3.h"

//---Forward Declarations---
class CRenderManager;
class CBillboard;
class CObject3D;
//--------------------------

class CSoundManager
{
public:
	//--- Init and End protocols------------------------------------------
					CSoundManager			( void );
	virtual			~CSoundManager			( void )								{ Done(); }  

	bool			Init					( void );
    void			Done					( void );
    bool			IsOk					( void ) const							{ return m_bIsOk; }

	//----Funcions principals---------------------------------------------
	bool			Load					( const std::string &_XMLSoundsFile );
	void			Update					( float _ElapsedTime );
	void			Render					( CRenderManager &_RM );
	bool			Reload					( void );
	
	//----Funcions -------------------------------------------------------
	   
	//-----------GENERAL FUNCTIONS------------------------
    bool			LoadSounds				( void );
    void			Reset					( void );
    void			Pause					( void );
    void			Stop					( void );
    void			SoundOn					( void )								{ m_bSoundON = true; }
    void			SoundOff				( void )								{ m_bSoundON = false; }
    void			SetGain					( float _Gain  );
    float			GetGain					( void );

    //---------ACTION METHODS---------------------------------
    bool			PlayAction2D			( const std::string& _Action, float _Gain = 1.f );
    bool			PlayAction3D			( const std::string& _Action, const Vect3f& _Position, float _Gain = 1.f );
        
    //---------SOURCE FUNCTIONS-------------------------------
   
	uint32			CreateSource			( void );
    bool			DeleteSource			( uint32 _Source );
    bool			PlaySource2D			( uint32 _Source, const std::string& _Action, bool _Loop, float _Gain = 1.f );
    bool			PlaySource3D			( uint32 _Source, const std::string& _Action, bool _Loop, float _Gain = 1.f );
    bool			PauseSource				( uint32 _Source );
    bool			StopSource				( uint32 _Source );
    bool			SetSourceGain			( uint32 _Source, float _InGain );
    bool			GetSourceGain			( uint32 _Source, float& _OutGain );
    bool			SetSourcePosition		( uint32 _Source, const Vect3f& _InPosition );
    bool			GetSourcePosition		( uint32 _Source, Vect3f& _OutPosition );
    bool			SetSourceVelocity		( uint32 _Source, const Vect3f& _InVelocity );
    bool			GetSourceVelocity		( uint32 _Source, Vect3f& _OutVelocity );
    bool			FadeInSource			( uint32 _Source, float _TotalTime, float _FinalGain = 1.f, ETypeFunction _Type = FUNC_CONSTANT );
    bool			FadeOutSource			( uint32 _Source, float _TotalTime, ETypeFunction _Type = FUNC_CONSTANT );

	//-----LISTENER METHODS---------------------------
    void			SetListenerPosition		( const Vect3f& _InPosition );
    void			GetListenerPosition		( Vect3f& _OutPosition );
    void			SetListenerVelocity		( const Vect3f& _InVelocity );
    void			GetListenerVelocity		( Vect3f& _OutVelocity );
    void			SetListenerOrientation	( const Vect3f& InAt, const Vect3f& _InUp );
    void			GetListenerOrientation	( Vect3f& _OutAt, Vect3f& _OutUp );

private:

	//Types
    typedef struct
    {
        ALuint  m_uSource;
        bool	m_bReserved;
    } tInfoSource;

	typedef std::string					tSoundFile;
    typedef std::string					tAction;
    typedef ALuint						tIdBuffer;

	void			Release					( void );

	bool			_initAL					( void );
    void			_finalizeAL				( void );
    bool			_loadSound				( const std::string& _File, tIdBuffer& _Buffer );
    void			_clear					( void );
    int				_getSource				( bool _Reserved );
    std::string		_getALErrorString		( ALenum _Err );

	//----Properties ( get & Set )----------------------------------------
public:
	
	std::string		GetFile					( void ) const					{ return m_szFileName; }

	bool			IsSourceFinished		( uint32 _Source );
	 
	// Funcions per test
	void			SetSoundVolume			( float _VolumValue )			{ m_fSoundVolume = _VolumValue; }
	float			GetSoundVolume			( void ) const					{ return m_fSoundVolume; }
	
	// Coloco o obtengo el Listener asociado
	void			SetListener				( CObject3D * _pListener )		{ m_pListener = _pListener; }
	CObject3D *		GetListener				( void ) const					{ return m_pListener; }

	//----Members ---------------------------------------
private:
	std::string							m_szFileName;
	float								m_fSoundVolume;
	bool								m_bIsOk;					// Initialization boolean control
    bool								m_bSoundON;
    bool								m_bPause;

	
    std::map<std::string, tIdBuffer>	m_BuffersMap;
    std::vector<tInfoSource>			m_SourcesVector;
    std::map<uint32, CLerpAnimator1D>   m_SourcesFadeInOutMap;

	std::vector<CBillboard*>			m_BillboardVector;			// Para pintar los altavoces en modo debug

	CObject3D						  * m_pListener;
};

#endif __CLASS_SOUND_MANAGER_H__