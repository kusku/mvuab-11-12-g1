#include "Steering.h"


CSteering::CSteering( BehaviorType _Type )
	: m_Type ( _Type )
{}


CSteering::~CSteering(void)
{}

void CSteering::RegisterLUAMethods( void )
{
}