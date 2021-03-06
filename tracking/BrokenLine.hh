#ifndef BrokenLine_hh
#define BrokenLine_hh

#include "Track.hh"

#include <TVector3.h>
#include <cmath>

class Hit;

class BrokenLine : public Track {
public:
  BrokenLine();
  virtual ~BrokenLine();

public:
  void setUpperX0(double upperX0) {m_upperX0 = upperX0;}
  void setLowerX0(double lowerX0) {m_lowerX0 = lowerX0;}
  void setY0(double y0) {m_y0 = y0;}
  void setUpperSlopeX(double slopeX) {m_upperSlopeX = slopeX;}
  void setLowerSlopeX(double slopeX) {m_lowerSlopeX = slopeX;}
  void setSlopeY(double slopeY) {m_slopeY = slopeY;}

public:
  double upperX0() const {return m_upperX0;}
  double lowerX0() const {return m_lowerX0;}
  double y0() const {return m_y0;}
  double upperSlopeX() const {return m_upperSlopeX;}
  double lowerSlopeX() const {return m_lowerSlopeX;}
  double slopeY() const {return m_slopeY;}
  double bendingAngle() const {return atan(m_upperSlopeX) - atan(m_lowerSlopeX);}
  double trackLength() const;
  double zIntersection() const {return m_zIntersection;}

  double x(double z) const;
  double y(double z) const;
  double slopeX(double) const;
  double slopeY(double) const;

private:
  void retrieveFitResults();

private:
  double m_upperX0;
  double m_lowerX0;
  double m_upperSlopeX;
  double m_lowerSlopeX;
  double m_y0;
  double m_slopeY;

  double m_zIntersection;

};

#endif /* BrokenLine_hh */
