#include "RigidityParticleSpectrum.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Helpers.hh"

#include <TH1D.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

RigidityParticleSpectrum::RigidityParticleSpectrum(Type type)
  : AnalysisPlot(AnalysisPlot::MomentumReconstruction)
  , H1DPlot()
  , m_type(type)
{
  QString title = "particle spectrum";
  if (m_type == All) {
    title += " - all";
  } else if (m_type == Negative) {
    title += " - negative";
  } else if (m_type == Positive) {
    title += " - positive";
  } else if (m_type == AlbedoPositive) {
    title += " - positive albedo";
  } else if (m_type == AlbedoNegative) {
    title += " - negative albedo";
  }

  const int nBins = 21;
  const double min = 0.1;
  const double max = 20;
  const QVector<double>& axis = Helpers::logBinning(nBins, min, max);

  m_particleHistogram = new TH1D(qPrintable(title), "", nBins, axis.constData());
  m_particleHistogram->Sumw2();
  m_particleHistogram->GetXaxis()->SetTitle("abs(rigidity / GV)");
  m_particleHistogram->GetYaxis()->SetTitle("particles");

  setTitle(title);

  TH1D* histogram = m_particleHistogram;

  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  addHistogram(histogram, H1DPlot::P);
}

RigidityParticleSpectrum::~RigidityParticleSpectrum()
{
}

void RigidityParticleSpectrum::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;
  if (!(flags & ParticleInformation::BetaGood))
    return;
  if (!(flags & ParticleInformation::InsideMagnet))
    return;
  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;
  if ((m_type == AlbedoPositive || m_type == AlbedoNegative ) && !(flags & ParticleInformation::Albedo))
    return;
  if ((m_type == Positive || m_type == Negative ) && (flags & ParticleInformation::Albedo))
    return;
  double rigidity = track->rigidity();

  if (flags & ParticleInformation::Albedo)
    rigidity *= -1;

  if (m_type == Negative && rigidity >= 0)
    return;
  if (m_type == Positive && rigidity <= 0)
    return;
  double absRigidity = qAbs(rigidity);
  m_particleHistogram->Fill(absRigidity);
}

void RigidityParticleSpectrum::update()
{
}

void RigidityParticleSpectrum::finalize()
{
  update();
}