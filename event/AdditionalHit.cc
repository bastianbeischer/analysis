#include "AdditionalHit.hh"

#include <cmath>
#include <iostream>

ClassImp(AdditionalHit);

AdditionalHit::AdditionalHit()
  : TObject()
  , m_type(Undefined)
  , m_detId(0x0)
  , m_signalHeight(0.)
{
}

AdditionalHit::AdditionalHit(Type type, unsigned short detId, double signalHeight)
  : TObject()
  , m_type(type)
  , m_detId(detId)
  , m_signalHeight(signalHeight)
{
}

AdditionalHit::AdditionalHit(const AdditionalHit& other)
  : TObject(other)
  , m_type(other.m_type)
  , m_detId(other.m_detId)
  , m_signalHeight(other.m_signalHeight)
{
}

AdditionalHit::~AdditionalHit()
{
}
