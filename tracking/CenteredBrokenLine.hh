#ifndef CenteredBrokenLine_hh
#define CenteredBrokenLine_hh

#include "Track.hh"

#include <TVector3.h>
#include <cmath>

class Hit;

class CenteredBrokenLine :
  public Track
{
  
public:
  CenteredBrokenLine();
  virtual ~CenteredBrokenLine();
  
public:
  void           setX0(double x0)                {m_x0 = x0;}
  void           setY0(double y0)                {m_y0 = y0;}
  void           setUpperSlopeX(double slopeX)   {m_upperSlopeX = slopeX;}
  void           setLowerSlopeX(double slopeX)   {m_lowerSlopeX = slopeX;}
  void           setSlopeY(double slopeY)        {m_slopeY = slopeY;}

public:
  double         x0()               const {return m_x0;}
  double         y0()               const {return m_y0;}
  double         upperSlopeX()      const {return m_upperSlopeX;}
  double         lowerSlopeX()      const {return m_lowerSlopeX;}
  double         slopeY()           const {return m_slopeY;}
  double         bendingAngle()     const {return atan(m_upperSlopeX) - atan(m_lowerSlopeX);}
  double         zIntersection()    const {return m_zIntersection;}

  double         x(double z)        const;
  double         y(double z)        const;
  double         slopeX(double z)   const;
  double         slopeY(double z)   const;

private:
  void           retrieveFitResults();

private:
  double            m_x0;
  double            m_upperSlopeX;
  double            m_lowerSlopeX;
  double            m_y0;
  double            m_slopeY;

  double            m_zIntersection;

};

#endif /* CenteredBrokenLine_hh */
