#ifndef _BASE_H
#define _BASE_H

#include "Utils\Types.h"
#include <assert.h>
#include <stdio.h>

#define CHECKED_DELETE(x) if(x){ delete x; x=NULL; }
#define CHECKED_RELEASE(x) if(x){ x->Release(); x=NULL; }
#define CHECKED_DELETE_ARRAY(x) if(x){ delete [] x; x=NULL; }

#define CORE		CCore::GetSingletonPtr()
#define LOGGER		CLogger::GetSingletonPtr()
#define SCRIPT		CScriptManager::GetSingletonPtr()
#define ENTMGR		CEntityManager::GetSingletonPtr()
#define DISPATCH	CMessageDispatcher::GetSingletonPtr()

#endif