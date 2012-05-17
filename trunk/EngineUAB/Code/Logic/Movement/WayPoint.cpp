
#include <assert.h>

#include "WayPoint.h"

CWayPoint::CWayPoint( const std::string& name, Vect3f position )
	: m_Name(name)
	, m_Position(position)
{

}

CWayPoint::~CWayPoint()
{
	m_Brothers.clear();
}

void CWayPoint::AddBrother( CWayPoint* bro )
{
	assert(bro);

	m_Brothers.push_back(bro);
}

void CWayPoint::SetBrothers( std::list<CWayPoint*> bros )
{
	m_Brothers = bros;
}

