#include "TrackingEfficiencyVsMomentumPlot.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Helpers.hh"

#include <QMap>

#include <TH2D.h>

#include <cmath>

TrackingEfficiencyVsMomentumPlot::TrackingEfficiencyVsMomentumPlot(Type type)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTracker)
  , H2DPlot()
  , m_type(type)
{
  QString htitle = "Efficiency vs momentum";

  if (m_type == Positive)
    htitle += " positive";
  if (m_type == Negative)
    htitle += " negative";
  if (m_type == All)
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

void TrackingEfficiencyVsMomentumPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  const ParticleInformation* info = particle->information();

  ParticleInformation::Flags flags = info->flags();
  if ( !(flags & ParticleInformation::InsideMagnet) || !(flags & ParticleInformation::Chi2Good) )
    return;

  const QMap<double,int>& hitsInLayers = info->hitsInLayers();
  unsigned short nLayers = info->numberOfTrackerLayers();

  const QMap<double,int>::const_iterator endIt = hitsInLayers.end();
  for(QMap<double,int>::const_iterator it = hitsInLayers.begin(); it != endIt; ++it) {
    int count = it.value();
    if (count != 1)
      return;
  }

  double rigidity = track->rigidity();

  if (m_type == Positive && rigidity < 0) {
    return;
  }
  if (m_type == Negative && rigidity > 0) {
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
