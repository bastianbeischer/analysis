#ifndef StraightLine_hh
#define StraightLine_hh

#include "Track.hh"

#include <TVector3.h>

class Hit;

class StraightLine :
  public Track
{
  
public:
  StraightLine();
  virtual ~StraightLine();

public:
  void           setX0(double x0)                {m_x0 = x0;}
  void           setY0(double y0)                {m_y0 = y0;}
  void           setSlopeX(double slopeX)        {m_slopeX = slopeX;}
  void           setSlopeY(double slopeY)        {m_slopeY = slopeY;}

public:
  double         x0()               const {return m_x0;}
  double         y0()               const {return m_y0;}
  double         slopeX()           const {return m_slopeX;}
  double         slopeY()           const {return m_slopeY;}

  double         x(double z)        const {return m_x0 + z*m_slopeX;}
  double         y(double z)        const {return m_y0 + z*m_slopeY;}
  double         slopeX(double)     const {return slopeX();}
  double         slopeY(double)     const {return slopeY();}
  double         bendingAngle()     const {return 0.;}

private:
  int            fit   (const QVector<Hit*>& hits);
  int            fit2D (const QVector<Hit*>& hits);
  int            fitTrd(const QVector<Hit*>& hits);

private:
  double            m_x0;
  double            m_slopeX;
  double            m_y0;
  double            m_slopeY;

};

#endif /* StraightLine_hh */
