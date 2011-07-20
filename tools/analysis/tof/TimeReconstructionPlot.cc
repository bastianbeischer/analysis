#include "TimeReconstructionPlot.hh"
#include "BrokenLine.hh"
#include "ParticleInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"
#include "Particle.hh"
#include "Track.hh"
#include "TimeOfFlight.hh"

#include <TH2.h>
#include <TVector3.h>
#include <THStack.h>
#include <TMath.h>

#include <iostream>
#include <QDebug>

TimeReconstructionPlot::TimeReconstructionPlot(Method method)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTOF)
  , H2DProjectionPlot()
  , m_method(method)
{
  QString title = QString("time reconstruction");
  if (m_method == Mean) {
    title+= " Mean";
  } else if (m_method == Median) {
    title+= " Median";
  }
  setTitle(title);
  int nBins = 100;
  double min = -5;
  double max = 5;
  TH2D* histogram = new TH2D(qPrintable(title), "", Constants::nTofChannels, 0, Constants::nTofChannels,
    nBins, min-.5*(max-min)/nBins, max-.5*(max-min)/nBins);
  setAxisTitle("", "#Deltat / ns", "");
  addHistogram(histogram);
}

TimeReconstructionPlot::~TimeReconstructionPlot()
{}

void TimeReconstructionPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  const QVector<Hit*>& clusters = track->hits();
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;

  QVector<double> startTimes;
  QVector<int> startTimeIds;
  QVector<double> stopTimes;
  QVector<int> stopTimeIds;
  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator it = clusters.begin(); it != endIt; ++it) {
    Hit* cluster = *it;
    if (cluster->type() == Hit::tof) {
      TOFCluster* tofCluster= static_cast<TOFCluster*>(cluster);
      Q_ASSERT(tofCluster->hits().size() <= 4);
      double z = tofCluster->position().z();
      if (qAbs(track->y(z)) > 50. || qAbs(track->x(z) - tofCluster->position().x()) > 1.2 * Constants::tofBarWidth / 2.)
        continue;


      std::vector<Hit*>& subHits = tofCluster->hits();
      const std::vector<Hit*>::const_iterator subHitsEndIt = subHits.end();
      for (std::vector<Hit*>::const_iterator subHitsIt = subHits.begin(); subHitsIt != subHitsEndIt; ++subHitsIt) {
        TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(*subHitsIt);
        int id = tofHit->detId();
        double startTime = tofHit->startTime();
        double tot = tofHit->timeOverThreshold();
        if (startTime < Constants::triggerMatchingMinimum || startTime > Constants::triggerMatchingMaximum || tot < Constants::minimalTotPerSipm)
          continue;
        startTime-= tofHit->photonTravelTime();
        if (z > 0) {
          startTimes.append(startTime);
          startTimeIds.append(id);
        } else {
          stopTimes.append(startTime);
          stopTimeIds.append(id);
        }
      }
    }
  }

  int n1 = startTimes.count();
  int n2 = stopTimes.count();
  if (n1 < 8 || n2 < 8)
    return;

  double t0 = -1.;
  double t1 = -1.;

  if (m_method == Mean) {
    t0 = TMath::Mean(n1, startTimes.constData());
    t1 = TMath::Mean(n2, stopTimes.constData());
  } else if (m_method == Median) {
    t0 = TMath::Median(n1, startTimes.constData());
    t1 = TMath::Median(n2, stopTimes.constData());
  }

  for (int i = 0; i < n1; ++i)
    histogram()->Fill(startTimeIds[i] - 0x8000, startTimes[i] - t0);
  for (int i = 0; i < n2; ++i)
    histogram()->Fill(stopTimeIds[i] - 0x8000, stopTimes[i] - t1);
}

void TimeReconstructionPlot::finalize()
{
  return;
  double nBinsY = histogram()->GetYaxis()->GetNbins();
  for (int binX = 1; binX <= Constants::nTofChannels; ++binX) {
    double sum = 0;
    for (int binY = 1; binY <= nBinsY; ++binY)
      sum+= histogram()->GetBinContent(binX, binY);
    for (int binY = 1; binY <= nBinsY; ++binY)
      histogram()->SetBinContent(binX, binY, histogram()->GetBinContent(binX, binY) / sum);
  }
}

void TimeReconstructionPlot::draw(TCanvas* canvas)
{
  if (m_drawn) {
    H2DPlot::draw(canvas);
  } else {
    H2DPlot::draw(canvas);
    for (int ch = 0; ch < Constants::nTofChannels; ++ch)
      xAxis()->SetBinLabel(ch + 1, qPrintable(QString("0x%1").arg(0x8000 | ch, 0, 16)));
  }
}
