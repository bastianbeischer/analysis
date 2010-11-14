#include "Track.hh"

#include "Matrix.hh"

#include <iostream>

Track::Track() :
  m_type(None),
  m_matrix(0),
  m_verbose(0),
  m_fitGood(0),
  m_chi2(0),
  m_ndf(0)
{
}

Track::~Track()
{
}
