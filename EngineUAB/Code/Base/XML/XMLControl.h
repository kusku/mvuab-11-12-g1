#pragma once

#ifndef _XML_CONTROL_H_
#define _XML_CONTROL_H_

#include "libxml\threads.h"

namespace XML
{
	inline void InitParser()
	{
		xmlInitParser();
	}

	inline void FinalizeParser()
	{
		xmlCleanupParser();
	}

	inline void InitThreads()
	{
		xmlInitThreads();
	}

	inline void FinalizeThreads()
	{
		xmlCleanupThreads();
	}

	inline void LockLibrary()
	{
		xmlLockLibrary();
	}

	inline void UnlockLibrary()
	{
		xmlUnlockLibrary();
	}
}

#endif