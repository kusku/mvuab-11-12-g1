
#pragma once

#ifndef _BOUDING_OBJECTS_H
#define _BOUDING_OBJECTS_H

#include "Math\Vector3.h"
#include <assert.h>

struct TBoundingBox
{
public:
	Vect3f m_MinPos;
	Vect3f m_MaxPos;

	TBoundingBox() :
		m_MinPos(0.0f, 0.0f, 0.0f),
		m_MaxPos(0.0f, 0.0f, 0.0f)
	{
	}

	TBoundingBox(Vect3f minPos, Vect3f maxPos)
	{
		m_MinPos = minPos;
		m_MaxPos = maxPos;
	}

	TBoundingBox(Vect3f points[])
	{
		assert(points);

		m_MinPos = points[0];
		m_MaxPos = points[0];

		for (int i = 1; i < 8; ++i)
		{
			if(m_MinPos.x > points[i].x)
			{
				m_MinPos.x = points[i].x;
			}
			if(m_MinPos.y > points[i].y)
			{
				m_MinPos.y = points[i].y;
			}
			if(m_MinPos.z > points[i].z)
			{
				m_MinPos.z = points[i].z;
			}

			if(m_MaxPos.x < points[i].x)
			{
				m_MaxPos.x = points[i].x;
			}
			if(m_MaxPos.y < points[i].y)
			{
				m_MaxPos.y = points[i].y;
			}
			if(m_MaxPos.z < points[i].z)
			{
				m_MaxPos.z = points[i].z;
			}
		}
	}
};

struct TBoundingSphere
{
public:
	Vect3f m_Center;
	float m_Radius;

	TBoundingSphere() :
		m_Center(0.0f, 0.0f, 0.0f),
		m_Radius(0.0f)
	{ 
	}

	TBoundingSphere(Vect3f center, float radius)
	{
		m_Center = center;
		m_Radius = radius;
	}
};

#endif