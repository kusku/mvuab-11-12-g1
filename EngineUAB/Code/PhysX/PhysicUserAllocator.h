//----------------------------------------------------------------------------------
// CPhysicUserAllocator class
// Author: Enric Vergara
//
// Description:
// Clase con una implementacion de la reserva de memoria dinamica que utilizara physX
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_USERA_ALLOCATOR_H
#define INC_PHYSIC_USERA_ALLOCATOR_H

//needed for sync primitives.
#if defined(WIN32)
#	include <windows.h>
#elif defined(_XBOX)
#	include <xtl.h>
#endif

class CPhysicUserAllocator: public NxUserAllocator
{
public:
	CPhysicUserAllocator();
	virtual ~CPhysicUserAllocator();

	void	reset				();

	void*	malloc			(size_t size);
	void*	malloc			(size_t size, NxMemoryType type);
	void*	mallocDEBUG		(size_t size, const char* file, int line);
	void*	mallocDEBUG		(size_t size, const char* file, int line, const char* className, NxMemoryType type);
	void*	realloc			(void* memory, size_t size);
	void	free			(void* memory);

public:

	size_t*		mMemBlockList;
	NxU32			mMemBlockListSize;
	NxU32			mMemBlockFirstFree;
	NxU32			mMemBlockUsed;

	NxI32			mNbAllocatedBytes;
	NxI32			mHighWaterMark;
	NxI32			mTotalNbAllocs;
	NxI32			mNbAllocs;
	NxI32			mNbReallocs;

#if defined(WIN32) || defined(_XBOX)
	CRITICAL_SECTION		allocatorLock;
	void		LockAlloc		()	{	EnterCriticalSection(&allocatorLock);	}
	void		UnlockAlloc		()	{	LeaveCriticalSection(&allocatorLock);	}
#else
	// TODO: other platforms should have a thread safe allocator as well.
	void		LockAlloc		(){}
	void		UnlockAlloc		(){}
#endif

};

#endif //INC_PHYSIC_USERA_ALLOCATOR_H
