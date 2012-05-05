#pragma once

#ifndef _HWND_MANAGER_H_
#define _HWND_MANAGER_H_

class CHWNDManager
{
public:
	static CHWNDManager* GetInstance();
	void CleanUp();

	void	SetHWNDElements		( HWND hWnd )		{ m_hWndElements = hWnd; }
	void	SetHWNDProperties	( HWND hWnd )		{ m_hWndProperties = hWnd; }
	void	SetHWNDOutput		( HWND hWnd )		{ m_hWndOutput = hWnd; }
	void	SetHWNDFiles		( HWND hWnd )		{ m_hWndFiles = hWnd; }

	HWND	GetHWNDElements		() const			{ return m_hWndElements; }
	HWND	GetHWNDProperties	() const			{ return m_hWndProperties; }
	HWND	GetHWNDOutput		() const			{ return m_hWndOutput; }
	HWND	GetHWNDFiles		() const			{ return m_hWndFiles; }

private:
	CHWNDManager();
	~CHWNDManager();

private:
	static CHWNDManager *m_pHWNDManager;

	HWND m_hWndElements;
	HWND m_hWndProperties;
	HWND m_hWndOutput;
	HWND m_hWndFiles;

};

#endif