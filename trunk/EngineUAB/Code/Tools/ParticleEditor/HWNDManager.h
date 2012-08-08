#pragma once

#ifndef _HWND_MANAGER_H_
#define _HWND_MANAGER_H_

class CHWNDManager
{
public:
	static CHWNDManager* GetInstance();
	void CleanUp();

	void	SetHWNDOutput		( HWND hWnd )		{ m_hWndOutput = hWnd; }
	void	SetHWNDFiles		( HWND hWnd )		{ m_hWndFiles = hWnd; }

	HWND	GetHWNDOutput		() const			{ return m_hWndOutput; }
	HWND	GetHWNDFiles		() const			{ return m_hWndFiles; }

private:
	CHWNDManager();
	~CHWNDManager();

private:
	static CHWNDManager *m_pHWNDManager;

	HWND m_hWndOutput;
	HWND m_hWndFiles;
};

#endif