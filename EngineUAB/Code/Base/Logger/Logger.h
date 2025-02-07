//----------------------------------------------------------------------------------
// CLogger class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga de almacenar mensajes hasta una capacidad limitada.
// Otra clase obtendr� la informaci�n almacenada en esta para printarla por pantalla, consola o enviarla por red
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include "Utils\Singleton.h"
#include "Utils\Types.h"
#include <vector>
#include <string>

//-----Definicion de nuevos tipos-----------------------
#define CAPACITY 500

enum ELOG_LEVEL
{
	//! Nothing is printed out to the log
	ELL_NONE = 0,

	//! High log level, warnings, errors and important information
	//! texts are printed out.
	ELL_INFORMATION,

	//! Default log level, warnings and errors are printed out
	ELL_WARNING,

	//! Low log level, only errors are printed into the log
	ELL_ERROR
};

struct SLog
{
	std::string		m_sLogText;
	ELOG_LEVEL		m_eLogLevel;
	unsigned int	m_uLogLine;
	bool			m_bLineFeed;
};
//------------------------------------------------------------

namespace Base
{
namespace Logger
{
	void PrintLogger(int Level, const std::string &Msg);
} //namespace Logger
} //namespace Base

class CLogger: public CSingleton<CLogger>
{
public:
	CLogger();
	virtual ~CLogger() {/*Nothing to do*/;}

	void							SetCapacity			(unsigned int capacity)		{m_uCapacity = capacity;}
	unsigned int					GetCapacity			()							{return m_uCapacity;}

	ELOG_LEVEL						GetLogLevel			() const					{return m_eLogLevel;}		
	void							SetLogLevel			(ELOG_LEVEL ll)				{m_eLogLevel = ll;}

	void							AddNewLog			(ELOG_LEVEL ll, const char* format, ... );
	const std::vector<SLog>&		GetLogs				() const					{return m_vLogs;}
	int								GetNumLogs			() const					{ return m_vLogs.size(); }
	const SLog&						GetLog				( uint32 id ) const			{ return m_vLogs[id]; }
	bool							Errors				() const 					{return m_bErrors;}
	bool							Warnings			() const 					{return m_bWarnings;}
	bool							SaveLogsInFile		();
	void							SetPathFile			(const std::string& path)	 {m_sPathFile = path;}

private:
	std::vector<SLog>		m_vLogs;
	unsigned int			m_uLinesCount;
	unsigned int			m_uCapacity;
	ELOG_LEVEL				m_eLogLevel;
	bool					m_bErrors;
	bool					m_bWarnings;
	std::string				m_sPathFile;

};

#endif // INC_LOGGER_H_

