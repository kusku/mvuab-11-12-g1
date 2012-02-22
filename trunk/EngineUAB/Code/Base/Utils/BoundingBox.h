#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include "base.h"
#include "Utils/BaseControl.h"

class CBoundingBox:
  public CBaseControl
{
public:
  CBoundingBox();
  ~CBoundingBox() { Done(); };

  bool Init(const Vect3f& _vMin, const Vect3f& _vMax);
  bool Init(Vect3f _vPoints[8]);
  bool Init(const Vect3f& _vDimensions);
  
  bool Adjust(const Vect3f& _vPoint);
  bool Adjust(const CBoundingBox& _BBox);
  
  const Vect3f& GetMin() const { return m_vBox[0]; };
  const Vect3f& GetMax() const { return m_vBox[7]; };
  const Vect3f* GetBox() const { return m_vBox; };
  const Vect3f& GetDimension() const { return m_vDimension; };
  const Vect3f& GetMiddlePoint() const { return m_vMiddlePoint; };
  const float GetMaxSideLength() const { return m_fMaxSideLenght; };
  void CalcDimension2();

protected:
  virtual void Release();
  
private:
  void CalcBox(const Vect3f& _vMin, const Vect3f& _vMax);
  void CalcMiddlePoint();
  void CalcDimension();
  void CalcMaxSide();

  Vect3f m_vMiddlePoint;

  float m_fMaxSideLenght;
  Vect3f m_vDimension;

  Vect3f m_vBox[8];
};

#endif