#include "OneHitAllLayersTrackingEfficiencyPlot.hh"

#include <cmath>

#include <TH1D.h>
#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"

#include <QSpinBox>

OneHitAllLayersTrackingEfficiencyPlot::OneHitAllLayersTrackingEfficiencyPlot(Type type) :
  AnalysisPlot(AnalysisPlot::MiscellaneousTracker),
  H1DPlot(),
  m_type(type),
  m_afterCutHisto(0),
  m_normHisto(0)
{
  QString htitle = "One hit in all layers";
  
  if (m_type == Positive)
    htitle += " positive";
  if (m_type == Negative)
    htitle += " negative";
  if (m_type == All)
    htitle += " all";
  setTitle(htitle);
  
  int nBins = 21;
  double lowerBound = 1e-1;
  double upperBound = 20.;
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }
  
  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, p);
  setAxisTitle("R / GV", "efficiency");
  addHistogram(histogram, H1DPlot::P);
  
  m_afterCutHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, p);
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, p);
}

OneHitAllLayersTrackingEfficiencyPlot::~OneHitAllLayersTrackingEfficiencyPlot()
{
  delete m_normHisto;
  delete m_afterCutHisto;
}

void OneHitAllLayersTrackingEfficiencyPlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::Chi2Good) )
    return;
  
  if ( !(flags & ParticleInformation::InsideMagnet) )
    return;
  
  double rigidity = track->rigidity();
  
  if (m_type == Positive && rigidity < 0) {
    return;
  }
  if (m_type == Negative && rigidity > 0) {
    return;
  }
  
  m_normHisto->Fill(qAbs(rigidity));
  
  if(!(flags & ParticleInformation::AllTrackerLayers))
    return;

  m_afterCutHisto->Fill(qAbs(rigidity));
}

void OneHitAllLayersTrackingEfficiencyPlot::update()
{
  histogram()->Divide(m_afterCutHisto, m_normHisto);
}

void OneHitAllLayersTrackingEfficiencyPlot::finalize()
{
  update();
}
