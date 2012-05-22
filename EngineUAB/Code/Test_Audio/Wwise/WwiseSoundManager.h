#ifndef _WWISE_SOUND_MANAGER_H_
#define _WWISE_SOUND_MANAGER_H_

class CCamera;

#include <string>

class CWwiseSoundManager
{
public:
	CWwiseSoundManager();
	~CWwiseSoundManager();

	bool	Init		();
	void	Terminate	();
	void	Update		( CCamera &camera );

	void	PlayEvent	( const std::string &event_name );
	void	SetSwitch	( const std::string &switch_name, const std::string &container_name );
};

#endif