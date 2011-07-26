#include "AllTrackerLayersFlagEfficiency.hh"

#include <cmath>

#include <TH1D.h>
#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Helpers.hh"

#include <QSpinBox>

AllTrackerLayersFlagEfficiency::AllTrackerLayersFlagEfficiency(Type type) :
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

  const int nBins = 21;
  const double min = 0.1;
  const double max = 20;
  const QVector<double>& axis = Helpers::logBinning(nBins, min, max);

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, axis.constData());
  histogram->Sumw2();
  setAxisTitle("R / GV", "efficiency");
  addHistogram(histogram, H1DPlot::P);

  m_afterCutHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, axis.constData());
  m_afterCutHisto->Sumw2();
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, axis.constData());
  m_normHisto->Sumw2();
}

AllTrackerLayersFlagEfficiency::~AllTrackerLayersFlagEfficiency()
{
  delete m_normHisto;
  delete m_afterCutHisto;
}

void AllTrackerLayersFlagEfficiency::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;

  if (!(flags & ParticleInformation::InsideMagnet))
    return;

  double rigidity = track->rigidity();

  if (m_type == Positive && rigidity < 0)
    return;
  if (m_type == Negative && rigidity > 0)
    return;

  m_normHisto->Fill(qAbs(rigidity));

  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  m_afterCutHisto->Fill(qAbs(rigidity));
}

void AllTrackerLayersFlagEfficiency::update()
{
  histogram()->Divide(m_afterCutHisto, m_normHisto);
}

void AllTrackerLayersFlagEfficiency::finalize()
{
  update();
}
