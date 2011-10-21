#include "TrackingEfficiencyVsMomentumPlot.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Helpers.hh"

#include <QMap>

#include <TH2D.h>

#include <cmath>

TrackingEfficiencyVsMomentumPlot::TrackingEfficiencyVsMomentumPlot(Enums::ChargeSigns type)
  : AnalysisPlot(Enums::MiscellaneousTracker)
  , H2DPlot()
  , m_type(type)
{
  QString htitle = "Efficiency vs momentum";

  if (m_type == Enums::Positive)
    htitle += " positive";
  if (m_type == Enums::Negative)
    htitle += " negative";
  if (m_type == (Enums::Positive | Enums::Negative))
    htitle += " all";
  setTitle(htitle);

  const int nBins = 21;
  const double min = 0.1;
  const double max = 20;
  const QVector<double>& axis = Helpers::logBinning(nBins, min, max);

  TH2D* histogram = new TH2D(qPrintable(title()), "", nBins, axis.constData(), 7, 1.5, 8.5);
  setAxisTitle("R / GV", "layers with exactly one hit", "");
  addHistogram(histogram);
  setDrawOption(COLZTEXT);
}

TrackingEfficiencyVsMomentumPlot::~TrackingEfficiencyVsMomentumPlot()
{
}

void TrackingEfficiencyVsMomentumPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::InsideMagnet | ParticleInformation::Chi2Good))
    return;
  double rigidity = track->rigidity();
  if (m_type == Enums::Positive && rigidity < 0)
    return;
  if (m_type == Enums::Negative && rigidity > 0)
    return;

  const ParticleInformation* info = event->particle()->information();
  const QMap<double,int>& hitsInLayers = info->hitsInLayers();
  unsigned short nLayers = info->numberOfTrackerLayers();

  const QMap<double,int>::const_iterator endIt = hitsInLayers.end();
  for(QMap<double,int>::const_iterator it = hitsInLayers.begin(); it != endIt; ++it) {
    int count = it.value();
    if (count != 1)
      return;
  }
  histogram()->Fill(qAbs(rigidity), nLayers);
}

void TrackingEfficiencyVsMomentumPlot::finalize()
{
  int nBinsX = histogram()->GetNbinsX();
  int nBinsY = histogram()->GetNbinsY();
  for (int binX = 1; binX <= nBinsX; binX++) {
    double sum = 0.;
    for (int binY = 1; binY <= nBinsY; binY++) {
      sum += histogram()->GetBinContent(binX, binY);
    }
    for (int binY = 1; binY <= nBinsY; binY++) {
      double bc = histogram()->GetBinContent(binX, binY);
      histogram()->SetBinContent(binX, binY, bc/sum);
    }
  }
}
