#pragma once

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <string>
#include "Utils\Singleton.h"

class COptions : public CSingleton<COptions>
{
public:
	COptions();
	~COptions();

	bool	Load	( const std::string &_Filename );
	bool	Reload	();
	void	Save	();

	//---Get Methods------------------------
	inline float	GetCustomGenericVolume		() const			{ return m_fCustomGenericVolume; }
	inline float	GetCustomMusicVolume		() const			{ return m_fCustomMusicVolume; }
	inline float	GetCustomEffectsVolume		() const			{ return m_fCustomEffectsVolume; }
	inline bool		GetCustomInvertedMouse		() const			{ return m_bCustomInvertedMouse; }

	inline float	GetDefaultGenericVolume		() const			{ return m_fDefaultGenericVolume; }
	inline float	GetDefaultMusicVolume		() const			{ return m_fDefaultMusicVolume; }
	inline float	GetDefaultEffectsVolume		() const			{ return m_fDefaultEffectsVolume; }
	inline bool		GetDefaultInvertedMouse		() const			{ return m_bDefaultInvertedMouse; }
	
	//---Set Methods------------------------
	inline void		SetCustomGenericVolume		( float value )		{ m_fCustomGenericVolume = value; }
	inline void		SetCustomMusicVolume		( float value )		{ m_fCustomMusicVolume = value; }
	inline void		SetCustomEffectsVolume		( float value )		{ m_fCustomEffectsVolume = value; }
	inline void		SetCustomMouseInverted		( bool value )		{ m_bCustomInvertedMouse = value; }

private:
	bool	LoadFile();

private:
	std::string	m_Filename;

	// Custom Values
	float		m_fCustomGenericVolume;
	float		m_fCustomMusicVolume;
	float		m_fCustomEffectsVolume;
	bool		m_bCustomInvertedMouse;

	// Default Values
	float		m_fDefaultGenericVolume;
	float		m_fDefaultMusicVolume;
	float		m_fDefaultEffectsVolume;
	bool		m_bDefaultInvertedMouse;
};

#endif //_OPTIONS_H_