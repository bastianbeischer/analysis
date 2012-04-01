#include "CalibrationCollection.hh"

#include <cassert>

ClassImp(CalibrationCollection);

CalibrationCollection::CalibrationCollection() :
  TObject()
{
}

CalibrationCollection::CalibrationCollection(const CalibrationCollection& other) :
  TObject(other)
{
  for (std::map<unsigned short, TH1I*>::const_iterator it = other.m_calibrationHistograms.begin(); it != other.m_calibrationHistograms.end(); it++) {
    m_calibrationHistograms[it->first] = new TH1I(*(it->second));
  }
}

CalibrationCollection::~CalibrationCollection()
{
  // for (std::map<unsigned short, TH1I*>::iterator it = m_calibrationHistograms.begin(); it != m_calibrationHistograms.end(); it++)
  //   delete it->second;
}

const TH1I* CalibrationCollection::calibration(unsigned short detId) const
{
  std::map<unsigned short, TH1I*>::const_iterator it = m_calibrationHistograms.find(detId);
  if (it != m_calibrationHistograms.end())
    return it->second;
  return 0;
}

void CalibrationCollection::addHistogram(unsigned short detId)
{
  if (!m_calibrationHistograms[detId]) {
    char title[128];
    sprintf(title, "cali_%x", detId);
    m_calibrationHistograms[detId] = new TH1I(title, title, 4096, 0, 4096);
  }
}

void CalibrationCollection::addValue(unsigned short detId, int value)
{
  TH1I* hist = m_calibrationHistograms[detId];
  assert(hist);
  hist->Fill(value);
}
