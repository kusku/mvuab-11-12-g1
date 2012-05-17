
#pragma once

#ifndef _WAY_POINT_H
#define _WAY_POINT_H

#include <string>
#include <list>

#include "Math/Vector3.h"

class CWayPoint
{
	private:
		std::string				m_Name;
		Vect3f					m_Position;
		std::list<CWayPoint*>	m_Brothers;

	public:
		CWayPoint(const std::string& name, Vect3f position);
		~CWayPoint();

		void					AddBrother			(CWayPoint* bro);
		void					SetBrothers			(std::list<CWayPoint*> bros);

		std::string				GetName				() const						{ return m_Name; }
		Vect3f					GetPosition			() const						{ return m_Position; }
		std::list<CWayPoint*>&	GetBrothers			()								{ return m_Brothers; }

		void					SetName				(std::string val)				{ m_Name = val; }
		void					SetPosition			(Vect3f val)					{ m_Position = val; }
};

#endif