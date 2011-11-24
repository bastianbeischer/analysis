#include "Reconstruction.hh"
#include "Likelihood.hh"
#include "KineticVariable.hh"
#include "AnalyzedEvent.hh"
#include "Particle.hh"
#include "Setup.hh"
#include "Helpers.hh"

#include "SpectrometerReconstruction.hh"
#include "TOFReconstruction.hh"
#include "WeightedMeanReconstruction.hh"
#include "Chi2Reconstruction.hh"
#include "LikelihoodReconstruction.hh"

#include <Math/IFunction.h>
#include <Math/BrentMinimizer1D.h>
#include <Math/Functor.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>

#include <cfloat>
#include <cmath>

const double Reconstruction::s_minimumCurvature = -10.;
const double Reconstruction::s_maximumCurvature = +10.;

Reconstruction* Reconstruction::newReconstruction(Enums::ReconstructionMethod method, Enums::LikelihoodVariables variables, Enums::Particles particles)
{
  Q_ASSERT(method != Enums::UndefinedReconstructionMethod);
  switch (method) {
    case Enums::UndefinedReconstructionMethod: return 0;
    case Enums::Spectrometer: return new SpectrometerReconstruction(variables, particles);
    case Enums::TOF: return new TOFReconstruction(variables, particles);
    case Enums::WeightedMean: return new WeightedMeanReconstruction(variables, particles);
    case Enums::Chi2: return new Chi2Reconstruction(variables, particles);
    case Enums::Likelihood: return new LikelihoodReconstruction(variables, particles);
    case Enums::SpectrometerExternalInformation: return new SpectrometerReconstruction(variables, particles, true);
    case Enums::TOFExternalInformation: return new TOFReconstruction(variables, particles, true);
    case Enums::WeightedMeanExternalInformation: return new WeightedMeanReconstruction(variables, particles, true);
    case Enums::Chi2ExternalInformation: return new Chi2Reconstruction(variables, particles, true);
    case Enums::LikelihoodExternalInformation: return new LikelihoodReconstruction(variables, particles, true);
  }
  Q_ASSERT(false);
  return 0;
}

Reconstruction::Reconstruction(Enums::LikelihoodVariables likelihoods, Enums::Particles particles)
  : m_externalInformation(false)
  , m_method(Enums::UndefinedReconstructionMethod)
  , m_variables(likelihoods)
  , m_likelihoods()
  , m_particles()
  , m_minima()
  , m_indexOfGlobalMinimum(-1)
  , m_event(0)
  , m_legend(0)
  , m_graph(0)
{
  for (Enums::LikelihoodVariableIterator it = Enums::likelihoodVariableBegin(); it != Enums::likelihoodVariableEnd(); ++it)
    if ((it.key() & likelihoods) == it.key())
      m_likelihoods.append(Likelihood::newLikelihood(it.key(), particles));

  for (Enums::ParticleIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it)
    if (it.key() != Enums::NoParticle && (it.key() & particles) == it.key())
      m_particles.append(it.key());
}

Reconstruction::~Reconstruction()
{
  if (m_legend)
    delete m_legend;
  if (m_graph)
    delete m_graph;
  qDeleteAll(m_likelihoods);
}

bool Reconstruction::externalInformation() const
{
  return m_externalInformation;
}

Enums::ReconstructionMethod Reconstruction::method() const
{
  return m_method;
}

double Reconstruction::curvature() const
{
  Q_ASSERT(0 <= m_indexOfGlobalMinimum && m_indexOfGlobalMinimum <= m_minima.count());
  return m_minima[m_indexOfGlobalMinimum].x();
}

Enums::Particle Reconstruction::particle() const
{
  Q_ASSERT(0 <= m_indexOfGlobalMinimum && m_indexOfGlobalMinimum <= m_minima.count());
  return m_particles[m_indexOfGlobalMinimum];
}

int Reconstruction::indexOfGlobalMinimum() const
{
  return m_indexOfGlobalMinimum;
}

const QVector<QPointF>& Reconstruction::minima() const
{
  return m_minima;
}

const QVector<Enums::Particle>& Reconstruction::particles() const
{
  return m_particles;
}

void Reconstruction::setupDefaultLegend() const
{
  if (m_legend)
    delete m_legend;
  m_legend = new TLegend(0.80, 0.55, 0.88, 0.88);
  foreach (Enums::Particle particle, m_particles) {
    TLegendEntry* entry = m_legend->AddEntry(static_cast<TObject*>(0), qPrintable(" " + Enums::label(particle)), "L");
    ParticleProperties properties(particle);
    entry->SetLineColor(properties.color());
    entry->SetLineWidth(3);
  }
}

TLegend* Reconstruction::legend() const
{
  return m_legend;
}

TMultiGraph* Reconstruction::graph() const
{
  if (m_legend)
    delete m_legend;
  if (m_graph)
    delete m_graph;
  m_graph = new TMultiGraph;
  TGraph* g = new TGraph(5);
  g->SetPoint(0, -2, 0);
  g->SetPoint(1, +2, 90);
  g->SetPoint(2, +2, 0);
  g->SetPoint(3, -2, 90);
  g->SetPoint(4, -2, 0);
  m_graph->Add(g);
  m_graph->SetTitle(qPrintable(QString(Enums::label(m_variables)).remove(" likelihood")));
  return m_graph;
}
