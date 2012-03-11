#include "Stadistics.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CStadistics::CStadistics()
	: m_uNumOfDrawCalls(0)
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
	ResetVerticesInFrustum();
	ResetDebugLines();
	ResetTrianglesInFrustum();
}
