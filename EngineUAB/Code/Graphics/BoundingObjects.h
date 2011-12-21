
#pragma once

#ifndef _BOUDING_OBJECTS_H
#define _BOUDING_OBJECTS_H

#include "Math\Vector3.h"

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