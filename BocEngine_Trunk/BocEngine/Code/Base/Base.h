#ifndef _BASE_H
#define _BASE_H

#include "Utils\Types.h"
#include <assert.h>
#include <stdio.h>
#include <dxerr.h>

#define CHECKED_DELETE(x) if(x){ delete x; x=NULL; }
#define CHECKED_RELEASE(x) if(x){ x->Release(); x=NULL; }
#define CHECKED_DELETE_ARRAY(x) if(x){ delete [] x; x=NULL; }

#define CORE CCore::GetSingletonPtr()
#define LOGGER CLogger::GetSingletonPtr()

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HRDX
	#define HRDX(x, y)                                 \
	{                                                  \
		HRESULT hr = x;                                \
		if(FAILED(hr))                                 \
		{                                              \
			DXTrace(__FILE__, __LINE__, hr, #x, TRUE); \
		}                                              \
		y = x;                                         \
	}
	#endif

#else
	#ifndef HRDX
	#define HRDX(x, y) y = x;
	#endif
#endif

#endif