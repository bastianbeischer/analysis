#include "FluxCollection.hh"

#include "Helpers.hh"
#include "Constants.hh"
#include "RigidityFlux.hh"
#include "RigiditySpectrumRatio.hh"
#include "RigidityParticleSpectrum.hh"
#include "ParticleInformation.hh"
#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"

#include <TH1D.h>

#include <cmath>
#include <vector>

#include <QLayout>
#include <QComboBox>
#include <QWidget>
#include <QVector>
#include <QDebug>

FluxCollection::FluxCollection(const QDateTime& first, const QDateTime& last) 
  : PlotCollection(AnalysisPlot::MomentumReconstruction)
  , m_particleHistogram(0)
  , m_particleHistogramAlbedo(0)
{
  QWidget* widget = new QWidget;
  QHBoxLayout* layout = new QHBoxLayout(widget);
  layout->setContentsMargins(0, 0, 0, 0);
  QComboBox* comboBox = new QComboBox;
  layout->addWidget(comboBox);
  layout->addStretch();
  secondaryWidget()->layout()->addWidget(widget);
  setTitle("flux collection");

  const int nBinsData = 42;
  const double minData = 0.1;
  const double maxData = 20;
  QVector<double> axisData = Helpers::logBinning(nBinsData, minData, maxData);
  int axisSize = axisData.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axisData.at(i);
    axisData.prepend(-value);
  }

  m_particleHistogram = new TH1D("particle spectrum - nonAlbedo", "", axisData.size()-1, axisData.constData());
  m_particleHistogram->Sumw2();
  m_particleHistogram->GetXaxis()->SetTitle("rigidity / GV");
  m_particleHistogram->GetYaxis()->SetTitle("particles");

  addPlot(new RigidityParticleSpectrum(RigidityParticleSpectrum::NonAlbedo, m_particleHistogram));
  comboBox->addItem("RigidityParticleSpectrum");
  addPlot(new RigidityFlux(RigidityFlux::Positive, first, last, m_particleHistogram));
  comboBox->addItem("RigidityFlux Positive");
  addPlot(new RigidityFlux(RigidityFlux::Negative, first, last, m_particleHistogram));
  comboBox->addItem("RigidityFlux Negative");

  m_particleHistogramAlbedo = new TH1D("particle spectrum - albedo", "", axisData.size()-1, axisData.constData());
  m_particleHistogramAlbedo->Sumw2();
  m_particleHistogramAlbedo->GetXaxis()->SetTitle("rigidity / GV");
  m_particleHistogramAlbedo->GetYaxis()->SetTitle("particles");

  addPlot(new RigidityParticleSpectrum(RigidityParticleSpectrum::Albedo, m_particleHistogramAlbedo));
  comboBox->addItem("RigidityParticleSpectrum Albedo");
  addPlot(new RigidityFlux(RigidityFlux::Positive, first, last, m_particleHistogramAlbedo));
  comboBox->addItem("RigidityFlux Positive Albedo");
  addPlot(new RigidityFlux(RigidityFlux::Negative, first, last, m_particleHistogramAlbedo));
  comboBox->addItem("RigidityFlux Negative Albedo");
  addPlot(new RigiditySpectrumRatio());
  comboBox->addItem("RigiditySpectrumRatio");

  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectPlot(int)));
}

FluxCollection::~FluxCollection()
{
}

void FluxCollection::processEvent(const QVector<Hit*>& hits, const Particle* const particle, const SimpleEvent* const event)
{
  PlotCollection::processEvent(hits, particle, event);
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
  double rigidity = track->rigidity();
  if (flags & ParticleInformation::Albedo)
    rigidity *= -1;
  if (!(flags & ParticleInformation::Albedo))
    m_particleHistogram->Fill(rigidity);
  if ((flags & ParticleInformation::Albedo))
    m_particleHistogramAlbedo->Fill(rigidity);
}
