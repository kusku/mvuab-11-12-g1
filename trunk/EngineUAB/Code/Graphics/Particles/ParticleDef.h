#ifndef __CLASS_PARTICLE_DEFINITIONS_H__
#define __CLASS_PARTICLE_DEFINITIONS_H__

#include <string>
#include "Math\Vector3.h"
#include "Math\Color.h"

#define TIME_CERO	0.0f

struct TEmitterAtTime
{
	float			m_fLifeTime1;
	float			m_fLifeTime2;

	Vect3f			m_vPosition1;
	Vect3f			m_vPosition2;

	float			m_fMinEmitRate;
	float			m_fMaxEmitRate;

	CColor			m_Color1;
	CColor			m_Color2;

	float			m_fMinSize;
	float			m_fMaxSize;

	Vect3f			m_vSpawnDir1;
	Vect3f			m_vSpawnDir2;

	float			m_fNumNewPartsExcess;
	
	bool			m_bAlive;				// Diu si l'emissor està viu 

	std::string		m_sTexture;

	Vect3f			m_vVelocity;
	
	float			m_fAngle;
	
	bool			m_bGravity;

	float			m_fKeyTime;
};

//
//template<class T>
//class TEmitterElement
//{
//	T*			m_fPreviousValue;
//	/*T* 			m_fNextValue;
//	
//	float		m_fTime;*/
//};
////
//template <typename T>
//struct map_container
//{
//    typedef std::map<T, std::string> type;
//};
//
//////: public CNamed
////template<typename T>
////struct TEmitterElementVector : std::vector<T> {};
//
////
////
////
////struct baseTest {
////	virtual GetNumberOfElems() = 0;
////};
////
////
////template <typename T>
////struct test : public baseTest {
////	std::size_t n;
////	T* value;
////	test(int num_of_elements) { value = new T[n = num_of_elements] };
////	int GetNumberOfElems() { return n; } 
////};
////
//
////template <typename T>
////struct vector_container
////{
////    typedef std::vector<T> type;
////};
//
//
////template <unsigned int x>
////struct factorial 
////{
////  static const unsigned int result = factorial<x-1>::result * x;
////};
////template<>
////struct factorial<0> 
////{
////  static const unsigned int result = 1;
////};

#endif __CLASS_PARTICLE_DEFINITIONS_H__