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
  for (std::map<unsigned short, TH1I*>::const_iterator it = other.m_pedestalHistograms.begin(); it != other.m_pedestalHistograms.end(); it++)
    m_pedestalHistograms[it->first] = new TH1I(*(it->second));
  for (std::map<unsigned short, TH1I*>::const_iterator it = other.m_ledHistograms.begin(); it != other.m_ledHistograms.end(); it++)
    m_ledHistograms[it->first] = new TH1I(*(it->second));
}

CalibrationCollection::~CalibrationCollection()
{
  // for (std::map<unsigned short, TH1I*>::iterator it = m_pedestalHistograms.begin(); it != m_pedestalHistograms.end(); it++)
  //   delete it->second;
}

const TH1I* CalibrationCollection::pedestalHistogram(unsigned short detId) const
{
  std::map<unsigned short, TH1I*>::const_iterator it = m_pedestalHistograms.find(detId);
  if (it != m_pedestalHistograms.end())
    return it->second;
  return 0;
}

void CalibrationCollection::addPedestalHistogram(unsigned short detId)
{
  if (!m_pedestalHistograms[detId]) {
    char title[128];
    sprintf(title, "pedestal_%x", detId);
    m_pedestalHistograms[detId] = new TH1I(title, title, 4096, 0, 4096);
  }
}

void CalibrationCollection::addPedestalValue(unsigned short detId, int value)
{
  TH1I* hist = m_pedestalHistograms[detId];
  assert(hist);
  hist->Fill(value);
}

std::vector<unsigned short> CalibrationCollection::pedestalIds() const
{
  std::vector<unsigned short> idVector(m_pedestalHistograms.size(), 0);
  std::vector<unsigned short>::iterator idVectorIt = idVector.begin();
  std::map<unsigned short, TH1I*>::const_iterator mapIt = m_pedestalHistograms.begin();
  std::map<unsigned short, TH1I*>::const_iterator mapEnd = m_pedestalHistograms.end();
  while (mapIt != mapEnd) {
    *idVectorIt = mapIt->first;
    ++idVectorIt;
    ++mapIt;
  }
  return idVector;
}

const TH1I* CalibrationCollection::ledHistogram(unsigned short detId) const
{
  std::map<unsigned short, TH1I*>::const_iterator it = m_ledHistograms.find(detId);
  if (it != m_ledHistograms.end())
    return it->second;
  return 0;
}

void CalibrationCollection::addLedHistogram(unsigned short detId)
{
  if (!m_ledHistograms[detId]) {
    char title[128];
    sprintf(title, "led_%x", detId);
    m_ledHistograms[detId] = new TH1I(title, title, 4096, 0, 4096);
  }
}

void CalibrationCollection::addLedValue(unsigned short detId, int value)
{
  TH1I* hist = m_ledHistograms[detId];
  assert(hist);
  hist->Fill(value);
}

std::vector<unsigned short> CalibrationCollection::ledIds() const
{
  std::vector<unsigned short> idVector(m_ledHistograms.size(), 0);
  std::vector<unsigned short>::iterator idVectorIt = idVector.begin();
  std::map<unsigned short, TH1I*>::const_iterator mapIt = m_ledHistograms.begin();
  std::map<unsigned short, TH1I*>::const_iterator mapEnd = m_ledHistograms.end();
  while (mapIt != mapEnd) {
    *idVectorIt = mapIt->first;
    ++idVectorIt;
    ++mapIt;
  }
  return idVector;
}
