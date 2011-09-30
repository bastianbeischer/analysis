#include "RigidityParticleSpectrum.hh"

#include "Track.hh"
#include "Particle.hh"
#include "SimpleEvent.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Helpers.hh"

#include <TH1D.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

RigidityParticleSpectrum::RigidityParticleSpectrum(Type type, TH1D* particleHistogram)
  : AnalysisPlot(AnalysisTopic::MomentumReconstruction)
  , H1DPlot()
  , m_type(type)
{
  QString title = "particle spectrum";
  if (m_type == NonAlbedo) {
    title += " - nonAlbedo";
  } else if (m_type == Albedo) {
    title += " - albedo";
  }
  setTitle(title);
  TH1D* histogram = particleHistogram;
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
  addHistogram(histogram, H1DPlot::P);
}

RigidityParticleSpectrum::~RigidityParticleSpectrum()
{
}

void RigidityParticleSpectrum::processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const)
{
}
