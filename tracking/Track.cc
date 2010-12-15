#include "Track.hh"

#include "TrackInformation.hh"
#include "Matrix.hh"

#include <iostream>
#include <cmath>
#include <cfloat>

Track::Track() :
  m_type(None),
  m_information(new TrackInformation(this)),
  m_matrix(0),
  m_verbose(0),
  m_fitGood(0),
  m_chi2(0),
  m_ndf(0)
{
}

Track::~Track()
{
  delete m_information;
}

double Track::pt() const
{
  double alpha = bendingAngle();
  if (alpha == 0.)
    return DBL_MAX;
  double z0_magnet = -40.; // mm
  double z1_magnet =  40.; // mm
  double x0_magnet = x(z0_magnet); // mm
  double x1_magnet = x(z1_magnet); // mm
  double B_estimate = 0.27; // tesla
  double L  = sqrt(pow(x1_magnet - x0_magnet, 2.) + pow(z1_magnet - z0_magnet,2.))*1000.; // convert from mm to m
  double pt = 0.3*B_estimate*L/alpha/1e6; // GeV
  return pt;
}
