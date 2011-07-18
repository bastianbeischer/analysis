#include "TOFTimeShiftTriggerPlot.hh"
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

#include <iostream>
#include <QDebug>

TOFTimeShiftTriggerPlot::TOFTimeShiftTriggerPlot()
  : AnalysisPlot(AnalysisPlot::CalibrationTOF)
  , H2DProjectionPlot()
{
  setTitle("time shift tigger");
  int nBins = 80;
  double min = -6;
  double max = 2;
  TH2D* histogram = new TH2D(qPrintable(title()), "", Constants::nTofChannels, 0, Constants::nTofChannels,
    nBins, min-.5*(max-min)/nBins, max-.5*(max-min)/nBins);
  setAxisTitle("", "#Deltat / ns", "");
  addHistogram(histogram);
}

TOFTimeShiftTriggerPlot::~TOFTimeShiftTriggerPlot()
{}

void TOFTimeShiftTriggerPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  const TimeOfFlight* tof = particle->timeOfFlight();
  const QVector<Hit*>& clusters = track->hits();
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;

  const QVector<Hit*>::const_iterator endIt = clusters.end();
  for (QVector<Hit*>::const_iterator it = clusters.begin(); it != endIt; ++it) {
    Hit* cluster = *it;
    if (cluster->type() == Hit::tof) {
      TOFCluster* tofCluster= static_cast<TOFCluster*>(cluster);
      double z = tofCluster->position().z();
      if (qAbs(track->y(z)) > 50.)
        continue;
      for (unsigned int i = 0; i < tofCluster->hits().size(); ++i) {
        TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(tofCluster->hits()[i]);
        if (z > 0) {
          histogram()->Fill(tofHit->detId() - 0x8000, tof->startTime() - tofHit->startTime());
        } else {
          histogram()->Fill(tofHit->detId() - 0x8000, tof->stopTime() - tofHit->startTime());
        }
      }
    }
  }
}

void TOFTimeShiftTriggerPlot::finalize()
{
  double nBinsY = histogram()->GetYaxis()->GetNbins();
  for (int binX = 1; binX <= Constants::nTofChannels; ++binX) {
    double sum = 0;
    for (int binY = 1; binY <= nBinsY; ++binY)
      sum+= histogram()->GetBinContent(binX, binY);
    for (int binY = 1; binY <= nBinsY; ++binY)
      histogram()->SetBinContent(binX, binY, histogram()->GetBinContent(binX, binY) / sum);
  }
}

void TOFTimeShiftTriggerPlot::draw(TCanvas* canvas)
{
  if (m_drawn) {
    H2DPlot::draw(canvas);
  } else {
    H2DPlot::draw(canvas);
    for (int ch = 0; ch < Constants::nTofChannels; ++ch)
      xAxis()->SetBinLabel(ch + 1, qPrintable(QString("0x%1").arg(0x8000 | ch, 0, 16)));
  }
}
