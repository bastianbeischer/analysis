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
  if (m_type == NonAlbedo) {
    title += " - nonAlbedo";
  } else if (m_type == Albedo) {
    title += " - albedo";
  }
  const int nBinsData = 42;
  const double minData = 0.1;
  const double maxData = 20;
  QVector<double> axisData = Helpers::logBinning(nBinsData, minData, maxData);
  int axisSize = axisData.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axisData.at(i);
    axisData.prepend(-value);
  }
  m_particleHistogram = new TH1D(qPrintable(title), "", axisData.size()-1, axisData.constData());
  m_particleHistogram->Sumw2();
  m_particleHistogram->GetXaxis()->SetTitle("rigidity / GV");
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
  if ((m_type == Albedo ) && !(flags & ParticleInformation::Albedo))
    return;
  if ((m_type == NonAlbedo ) && (flags & ParticleInformation::Albedo))
    return;
  double rigidity = track->rigidity();
  if (flags & ParticleInformation::Albedo)
    rigidity *= -1;
  m_particleHistogram->Fill(rigidity);
}

void RigidityParticleSpectrum::update()
{
}

void RigidityParticleSpectrum::finalize()
{
  update();
}