#include "Stadistics.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CStadistics::CStadistics()
	: m_uNumOfDrawCalls(0)
	, m_uNumOfVerticesInScene(0)
	, m_uNumOfVerticesInFrustum(0)
	, m_uNumOfDebugLines(0)
{
}

CStadistics::~CStadistics()
{
}

void CStadistics::ResetAll()
{
	ResetDrawCalls();
	ResetVerticesInScene();
	ResetVerticesInFrustum();
	ResetDebugLines();
}

uint32 CStadistics::GetNumOfDrawCalls()
{
	return m_uNumOfDrawCalls;
}
