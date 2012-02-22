#include "Utils/BoundingSphere.h"
#include "Utils/BoundingBox.h"

bool CBoundingSphere::Init(const Vect3f& _vMin, const Vect3f& _vMax)
{
  CalcMiddlePoint(_vMin, _vMax);
  CalcRadius(_vMin, _vMax);

  SetOk(true);

  return IsOk();
}

bool CBoundingSphere::Init(const Vect3f& _vMiddlePoint, float _fRadius)
{
  m_vMiddlePoint = _vMiddlePoint;
  m_fRadius    = _fRadius;

  SetOk(true);
  return IsOk();
}

bool CBoundingSphere::Init(const CBoundingBox& _BBox)
{
  return Init(_BBox.GetMin(), _BBox.GetMax());
}

void CBoundingSphere::CalcMiddlePoint(const Vect3f& _vMin, const Vect3f& _vMax)
{
  Vect3f l_vMidVector = _vMax - _vMin;
  l_vMidVector *= 0.5f;
  m_vMiddlePoint = l_vMidVector + _vMin;
}

void CBoundingSphere::CalcRadius(const Vect3f& _vMin, const Vect3f& _vMax)
{
  m_fRadius = _vMin.Distance(_vMax) * 0.5f;
}

void CBoundingSphere::Release()
{

}

