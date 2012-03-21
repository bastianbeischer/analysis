#include "RigiditySpectrum.hh"

#include "Track.hh"
#include "Particle.hh"
#include "SimpleEvent.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Helpers.hh"
#include "ParticleProperties.hh"
#include "Hypothesis.hh"

#include <TH1D.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

RigiditySpectrum::RigiditySpectrum(Enums::Particles particles, bool albedo)
  : AnalysisPlot(Enums::FluxCalculation)
  , H1DPlot()
  , m_particles(Enums::particleVector(particles))
  , m_albedo(albedo)
{
  QString title = "rigidity spectrum";
  if (m_albedo)
    title += " albedo";
  setTitle(title);

  const int nBinsData = 31;
  const double minData = 0.1;
  const double maxData = 20;
  QVector<double> axisData = Helpers::logBinning(nBinsData, minData, maxData);
  foreach (Enums::Particle particle, m_particles) {
    TH1D* histogram = new TH1D(qPrintable(title + ' ' + Enums::label(particle)), "", nBinsData, axisData.constData());
    histogram->Sumw2();
    ParticleProperties properties(particle);
    histogram->SetMarkerColor(properties.color());
    histogram->SetLineColor(properties.color());
    addHistogram(histogram);
  }
  setAxisTitle("|R| / GV", "");

  addRequiredEventFlags(Enums::TrackGood | Enums::Chi2Good | Enums::AllTrackerLayers | Enums::InsideMagnet | Enums::BetaGood);
  if (m_albedo) addRequiredEventFlags(Enums::Albedo);
  else addRequiredEventFlagsAbsence(Enums::Albedo);
}

RigiditySpectrum::~RigiditySpectrum()
{
}

void RigiditySpectrum::processEvent(const AnalyzedEvent* event)
{
  const Hypothesis* h = event->particle()->hypothesis();
  int i = m_particles.indexOf(h->particle());
  histogram(i)->Fill(h->absoluteRigidity());
}
