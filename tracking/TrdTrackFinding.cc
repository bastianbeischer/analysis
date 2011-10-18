#include "TrdTrackFinding.hh"

#include <TVector3.h>

#include <cmath>

#include <QDebug>

TrdTrackFinding::TrdTrackFinding()
{
  const double trdHeight = 300;
  
  const int nBinsOffset = 12;
  const double offsetMin = -120;
  const double offsetMax = -1 * offsetMin;
  const int nBinsSlope = 22;
  const double slopeMin = (offsetMin - offsetMax) / trdHeight;
  const double slopeMax = -1 * slopeMin;
  
  m_slopeVsOffset = new TH2D("slopeVsOffset", "", nBinsOffset, offsetMin, offsetMax, nBinsSlope, slopeMin, slopeMax);
}

TrdTrackFinding::~TrdTrackFinding()
{
  delete m_slopeVsOffset;
  m_slopeVsOffset = 0;
}

void TrdTrackFinding::apply(const std::vector<Hit*>& hits)
{
  apply(QVector<Hit*>::fromStdVector(hits));
}

void TrdTrackFinding::apply(const QVector<Hit*>& hits)
{
  QVector<Hit*> trdHits;
  for (int i = 0; i < hits.size(); i++) {
    Hit* hit = hits[i];
    if (hit->type() == Hit::trd) {
      trdHits.push_back(hit);
    }
  }
  
//  qDebug()<<"trd size"<<trdHits.size();
  
  for (int a = 0; a < trdHits.size(); ++a) {
    for (int b = a; b < trdHits.size(); ++b) {
      if (a == b) {
        continue;
      }
      double xa = trdHits[a]->position().x();
      double xb = trdHits[b]->position().x();
      double za = trdHits[a]->position().z();
      double zb = trdHits[b]->position().z();
      
      const double slope = (xa - xb) / (za - zb);
      const double offset = xa - slope * za;
      
      m_slopeVsOffset->Fill(offset, slope);
    }
  }
  
  int bin = m_slopeVsOffset->GetMaximumBin();
  int binX;
  int binY;
  int binZ;
  m_slopeVsOffset->GetBinXYZ(bin, binX, binY, binZ);
  
  m_x = m_slopeVsOffset->GetXaxis()->GetBinCenter(binX);
  m_slopeX = m_slopeVsOffset->GetYaxis()->GetBinCenter(binY);
  
  const double maxPull = 8;
  m_nHitsOnTrack = 0;
  for (int i = 0; i < trdHits.size(); ++i) {
    if (isInCorridor(trdHits[i], maxPull)) {
      m_nHitsOnTrack++;
    }
  }
//  qDebug()<<"entries"<<entriesAroundMaximum()<<"hits on track"<<m_nHitsOnTrack<<"e per trackhit"<<entriesAroundMaximum()/m_nHitsOnTrack;
  
}

bool TrdTrackFinding::isInCorridor(Hit* hit, double maxPull)
{
  
  double z = hit->position().z();
  double hitX = 0.5*(hit->position().x() + hit->counterPosition().x());
  double trackX = m_slopeX * z + m_x;
  
  double res = hitX - trackX;
  double resolution = hit->resolutionEstimate();
  
  return (fabs(res/resolution) < maxPull);
}

double TrdTrackFinding::entriesAroundMaximum()
{
  int bin = m_slopeVsOffset->GetMaximumBin();
  int binX;
  int binY;
  int binZ;
  
  int nBinsX = m_slopeVsOffset->GetNbinsX();
  int nBinsY = m_slopeVsOffset->GetNbinsY();
  
  m_slopeVsOffset->GetBinXYZ(bin, binX, binY, binZ);
  double sum = m_slopeVsOffset->GetBinContent(binX, binY);
  
  if (binX > 1) {
    int newBin = binX - 1;
    sum += m_slopeVsOffset->GetBinContent(newBin, binY);
    if (binY > 1)
      sum += m_slopeVsOffset->GetBinContent(newBin, binY-1);
    if (binY < nBinsY)
      sum += m_slopeVsOffset->GetBinContent(newBin, binY+1);
  }
  if (binX < nBinsX) {
    int newBin = binX + 1;
    sum += m_slopeVsOffset->GetBinContent(newBin, binY);
    if (binY > 1)
      sum += m_slopeVsOffset->GetBinContent(newBin, binY-1);
    if (binY < nBinsY)
      sum += m_slopeVsOffset->GetBinContent(newBin, binY+1);
  }
  if (binY > 1) {
    int newBin = binY - 1;
    sum += m_slopeVsOffset->GetBinContent(binX, newBin);
  }
  if (binY < nBinsY) {
    int newBin = binY + 1;
    sum += m_slopeVsOffset->GetBinContent(binX, newBin);
  }
  
  return sum;
}

bool TrdTrackFinding::hasTrdTrack()
{
  if (entriesAroundMaximum() < 6) {
    return false;
  } else {
    return true;
  }
;
}


