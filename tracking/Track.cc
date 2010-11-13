#include "Track.hh"

#include "Matrix.hh"

Track::Track() :
  m_matrix(0),
  m_verbose(0),
  m_chi2(0),
  m_ndf(0)
{
}

Track::~Track()
{
  delete m_matrix;
}
