#pragma once

#ifndef _MATH_HELPER_H_
#define _MATH_HELPER_H_

#include "Math\MathTypes.h"

namespace Helper
{
	float	AngleFilter		(float _angle);
	float	LimitValue		( float _fValue, float _fLowLimit, float _fHighLimit );
	bool	CheckNaNValue	( float _fValue );

} //namespace Helper


#endif //_MATH_HELPER_H_
