#pragma once

#ifndef _ELEMENT_MANAGER_H_
#define _ELEMENT_MANAGER_H_

#include <string>

typedef enum TElement
{
	NONE = 0,
	ANIMATED_IMAGE,
	BUTTON,
	CHECK_BUTTON,
	DIALOG_BOX,
	EDITABLE_TEXT_BOX,
	IMAGE,
	POINTER_MOUSE,
	PROGRESS_BAR,
	RADIO_BOX,
	SLIDER,
	STATIC_TEXT,
	TEXT_BOX
};

class CElementManager
{
public:
	static CElementManager* GetInstance();
	void CleanUp();

	void				SetElementToAdd			( TElement element )			{ m_ElementToAdd = element; }
	TElement			GetElementToAdd			() const						{ return m_ElementToAdd; }

	void				SetWindowToAdd			( const std::string& window )	{ m_WindowToAdd = window; }
	const std::string&	GetWindowToAdd			() const						{ return m_WindowToAdd; }

	void				SetPathToInit			( const std::string& path )		{ m_PathToInit = path; }
	const std::string&	GetPathToInit			() const						{ return m_PathToInit; }

private:
	CElementManager();
	~CElementManager();

private:
	static CElementManager *m_pElementManager;

	TElement		m_ElementToAdd;
	std::string		m_WindowToAdd;
	std::string		m_PathToInit;
};

#endif