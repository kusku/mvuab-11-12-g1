#include "MathHelper.h"
#include <boost\math\special_functions\fpclassify.hpp>

#if defined _DEBUG
#include "Memory/MemLeaks.h"
#endif

namespace Helper
{
	float AngleFilter(float _angle)
	{
		float l_fAngle	= _angle;
		float l_2PI		= e2PI<float>();

		if( l_fAngle >  l_2PI)
		{
			l_fAngle = l_fAngle - l_2PI;
		}
		else if( l_fAngle < -l_2PI )
		{
			l_fAngle = l_fAngle + l_2PI;
		}

		return l_fAngle;	
	}

	bool CheckNaNValue( float _fValue )
	{
		return boost::math::isnan( _fValue );
	}

	float LimitValue( float _fValue, float _fLowLimit, float _fHighLimit )
	{
		assert(_fLowLimit < _fHighLimit);

		float l_fValue = _fValue;

		if(l_fValue > _fHighLimit)
			l_fValue = _fHighLimit;
		else if(l_fValue < _fLowLimit)
			l_fValue = _fLowLimit;

		return l_fValue;
	}

} //namespace Helper