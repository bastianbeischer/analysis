#include "TrackingEfficiencyVsMomentumPlot.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"

#include <QMap>

#include <TH2D.h>

TrackingEfficiencyVsMomentumPlot::TrackingEfficiencyVsMomentumPlot()
  : AnalysisPlot(AnalysisPlot::MiscellaneousTracker)
  , H2DPlot()
{
  setTitle(QString("Efficiency vs momentum"));
  
  TH2D* histogram = new TH2D(qPrintable(title()), "", 10, 0., 10., 7, 1.5, 8.5);
  setAxisTitle("R / GV", "layers with exactly one hit", "");
  addHistogram(histogram);
  setDrawOption(COLZTEXT);
}

TrackingEfficiencyVsMomentumPlot::~TrackingEfficiencyVsMomentumPlot()
{
}

void TrackingEfficiencyVsMomentumPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
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

  histogram()->Fill(rigidity,nLayers);
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
