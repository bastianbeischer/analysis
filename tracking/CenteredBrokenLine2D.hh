#ifndef CenteredBrokenLine2D_hh
#define CenteredBrokenLine2D_hh

#include "Track.hh"

#include <TVector3.h>
#include <cmath>

class Hit;

class CenteredBrokenLine2D : public Track {
public:
  CenteredBrokenLine2D();
  virtual ~CenteredBrokenLine2D();

public:
  void setX0(double x0) {m_x0 = x0;}
  void setY0(double y0) {m_y0 = y0;}
  void setUpperSlopeX(double slopeX) {m_upperSlopeX = slopeX;}
  void setLowerSlopeX(double slopeX) {m_lowerSlopeX = slopeX;}
  void setUpperSlopeY(double slopeY) {m_upperSlopeY = slopeY;}
  void setLowerSlopeY(double slopeY) {m_lowerSlopeY = slopeY;}

public:
  double x0() const {return m_x0;}
  double y0() const {return m_y0;}
  double upperSlopeX() const {return m_upperSlopeX;}
  double lowerSlopeX() const {return m_lowerSlopeX;}
  double upperSlopeY() const {return m_upperSlopeY;}
  double lowerSlopeY() const {return m_lowerSlopeY;}
  double bendingAngle() const {return atan(m_upperSlopeX) - atan(m_lowerSlopeX);}
  double trackLength() const;
  double zIntersection() const {return m_zIntersection;}

  double x(double z) const;
  double y(double z) const;
  double slopeX(double z) const;
  double slopeY(double z) const;

private:
  void retrieveFitResults();


private:
  double m_x0;
  double m_upperSlopeX;
  double m_lowerSlopeX;
  double m_y0;
  double m_upperSlopeY;
  double m_lowerSlopeY;

  double m_zIntersection;

};

#endif /* CenteredBrokenLine2D_hh */
