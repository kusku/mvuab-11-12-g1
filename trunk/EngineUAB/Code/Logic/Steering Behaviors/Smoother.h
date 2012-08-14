#pragma once

#ifndef __CLASS_SMOOTHER_H__
#define __CLASS_SMOOTHER_H__

#include <vector>
#include "Base.h"

template <class T>
class CSmoother
{
public:

	//--- Init and End protocols------------------------------------------
	//to instantiate a Smoother pass it the number of samples you want
	//to use in the smoothing, and an exampe of a 'zero' type
	CSmoother ( int _SampleSize, T _ZeroValue )
		: m_History			( _SampleSize, _ZeroValue )
		, m_ZeroValue		( _ZeroValue )
		, m_iNextUpdateSlot	( 0 )
	{}

	//---- Main Functions ---------------------------------------
	// each time you want to get a new average, feed it the most recent value
	// and this method will return an average over the last SampleSize updates
	T Update(const T& MostRecentValue)
	{  
		//overwrite the oldest value with the newest
		m_History[m_iNextUpdateSlot++] = MostRecentValue;

		//make sure m_iNextUpdateSlot wraps around. 
		if ( m_iNextUpdateSlot == m_History.size() ) 
			m_iNextUpdateSlot = 0;

		//now to calculate the average of the history list
		T sum = m_ZeroValue;

		std::vector<T>::iterator it = m_History.begin();

		for (it; it != m_History.end(); ++it)
		{
			sum += *it;
		}

		return sum / (float)m_History.size();
	}

	//---- Register Methods -------------------------------------
	/*static void RegisterLUAMethods( void )
	{
		lua_State *state = SCRIPT->GetLuaState();

		module(state) [
			class_<CSmoother>("CSmoother")
				.def("update", &CSmoother::Update)
		];
	}*/

	//---- Members -----------------------------------------------
private:

	//this holds the history
	std::vector<T>  m_History;

	int				m_iNextUpdateSlot;

	//an example of the 'zero' value of the type to be smoothed. This
	//would be something like Vect3f(0,0,0)
	T				m_ZeroValue;
};


#endif __CLASS_SMOOTHER_H__