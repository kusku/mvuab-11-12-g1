#ifndef __BOUNDING_SPHERE_H__
#define __BOUNDING_SPHERE_H__

#include "base.h"
#include "Utils/BaseControl.h"

class CBoundingBox;

class CBoundingSphere:
  public CBaseControl
{
public:
  CBoundingSphere(): m_vMiddlePoint(Vect3f(0.0f)), m_fRadius(0.0f)
                    {};
  ~CBoundingSphere() { Done(); };

  bool Init(const Vect3f& _vMin, const Vect3f& _vMax);

  bool Init(const CBoundingBox& _BBox);

  bool Init(const Vect3f& _vMiddlePoint, float _fRadius);

  const Vect3f& GetMiddlePoint() const { return m_vMiddlePoint; };
  const float GetRadius() const { return m_fRadius; };
  const float GetDimension() const { return m_fRadius * 2.0f; };

protected:
  virtual void Release();
  
private:
  void CalcMiddlePoint(const Vect3f& _vMin, const Vect3f& _vMax);
  void CalcRadius(const Vect3f& _vMin, const Vect3f& _vMax);

  Vect3f m_vMiddlePoint;
  float  m_fRadius;
};


#endif