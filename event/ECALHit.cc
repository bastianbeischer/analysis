#include "ECALHit.hh"

#include <cmath>
#include <iostream>

ClassImp(ECALHit);

ECALHit::ECALHit()
  : TObject()
  , m_detId(0x0)
  , m_signalHeight(0.)
{
}

ECALHit::ECALHit(unsigned short detId, double signalHeight)
  : TObject()
  , m_detId(detId)
  , m_signalHeight(signalHeight)
{
}

ECALHit::ECALHit(const ECALHit& other)
  : TObject(other)
  , m_detId(other.m_detId)
  , m_signalHeight(other.m_signalHeight)
{
}

ECALHit::~ECALHit()
{
}
