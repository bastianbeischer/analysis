#include "ChannelTriggerProbabilityPlot.hh"
#include "BrokenLine.hh"
#include "TrackInformation.hh"
#include "Hit.hh"
#include "Constants.hh"
#include "TOFCluster.hh"
#include "TOFSipmHit.hh"
#include "Setup.hh"
#include "TOFBar.hh"

#include <TH1.h>
#include <TVector3.h>
#include <TLatex.h>
#include <TF1.h>
#include <THStack.h>

#include <QDebug>

ChannelTriggerProbabilityPlot::ChannelTriggerProbabilityPlot()
  : AnalysisPlot(AnalysisPlot::CalibrationTOF)
  , H1DPlot()
  , m_normalizationHistogram(new TH1D("ChannelTriggerProbabilityPlotNormalizationHistogram", "", Constants::nTofChannels, 0, Constants::nTofChannels))
{
  setTitle("channel trigger probability");
  TH1D* histogram = new TH1D(qPrintable(title()), "", Constants::nTofChannels, 0, Constants::nTofChannels);
  addHistogram(histogram);
}

ChannelTriggerProbabilityPlot::~ChannelTriggerProbabilityPlot()
{
  delete m_normalizationHistogram;
}

void ChannelTriggerProbabilityPlot::processEvent(const QVector<Hit*>& clusters, Track* track, SimpleEvent*)
{
  // QMutexLocker locker(&m_mutex);
  if (!track || !track->fitGood())
    return;
  TrackInformation::Flags flags = track->information()->flags();
  if (!(flags & TrackInformation::Chi2Good))
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
          if (qFuzzyCompare(track->startTime(), tofHit->startTime())) {
            histogram()->Fill(tofHit->detId() - 0x8000);
          }
        } else {
          if (qFuzzyCompare(track->stopTime(), tofHit->startTime())) {
            histogram()->Fill(tofHit->detId() - 0x8000);
          }
        }
      }
      for (int sipm = 0; sipm < Constants::nTofSipmsPerBar; ++sipm)
        m_normalizationHistogram->Fill(tofCluster->detId() - 0x8000 + sipm);
    }
  }
}

void ChannelTriggerProbabilityPlot::finalize()
{
  histogram()->Divide(m_normalizationHistogram);
}
  
void ChannelTriggerProbabilityPlot::draw(TCanvas* canvas)
{
  if (m_drawn) {
    H1DPlot::draw(canvas);
  } else {
    H1DPlot::draw(canvas);
    for (int ch = 0; ch < Constants::nTofChannels; ++ch)
      xAxis()->SetBinLabel(ch + 1, qPrintable(QString("0x%1").arg(0x8000 | ch, 0, 16)));
  }
}
