#include "TOFLikelihood.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SensorTypes.hh"
#include "SettingsManager.hh"
#include "Particle.hh"
#include "SimpleEvent.hh"
#include "Track.hh"
#include "KineticVariable.hh"

#include <TH1.h>
#include <TF1.h>
#include <TMath.h>

#include <QDebug>

TOFLikelihood::TOFLikelihood()
  : AnalysisPlot(Enums::Testbeam)
  , H1DPlot()
  , m_nBins(300)
  , m_min(-3.0)
  , m_max(+3.0)
  , m_belowThresholdHistogram(0)
  , m_aboveThresholdHistogram(0)
  , m_momenta()
{
  setTitle("TOF likelihood");
  m_belowThresholdHistogram = new TH1D(qPrintable(title() + " below threshold raw"), "", m_nBins, m_min, m_max);
  m_aboveThresholdHistogram = new TH1D(qPrintable(title() + " above threshold raw"), "", m_nBins, m_min, m_max);
  TH1D* histogram = 0;
  histogram = new TH1D(qPrintable(title() + " below threshold"), "", m_nBins, m_min, m_max);
  histogram->SetLineColor(kBlue);
  addHistogram(histogram);
  histogram = new TH1D(qPrintable(title() + " above threshold"), "", m_nBins, m_min, m_max);
  histogram->SetLineColor(kRed);
  addHistogram(histogram);
  setAxisTitle("1 / #beta", "");
  yAxis()->SetRangeUser(0.01, 3.);
}

TOFLikelihood::~TOFLikelihood()
{
}

void TOFLikelihood::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
  ParticleInformation::Flags required = ParticleInformation::Chi2Good | ParticleInformation::BetaGood;
  if ((flags & required) != required)
    return;

  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  if (settings && settings->situation() != Settings::Testbeam11)
    return;

  addFunctions(settings->momentum());
  
  double signal1 = event->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double signal2 = event->sensorData(SensorTypes::BEAM_CHERENKOV2);

  double threshold = 200.;
  if (signal1 < threshold && signal2 < threshold)
    m_belowThresholdHistogram->Fill(1./particle->beta());
  if (signal1 > threshold && signal2 > threshold)
    m_aboveThresholdHistogram->Fill(1./particle->beta());
}

double gauss(double* x, double* p)
{
  return TMath::Gaus(x[0], p[0], p[1], true);
}

void TOFLikelihood::addFunctions(double momentum)
{
  foreach (double existingMomentum, m_momenta)
    if (qAbs(existingMomentum - momentum) < 0.01)
      return;
  m_momenta.append(momentum);
  QVector<Enums::Particle> particles = QVector<Enums::Particle>()
    << Enums::Proton << Enums::PiPlus << Enums::Electron;
  foreach (Enums::Particle particleType, particles) {
    Particle particle(particleType);
    TF1* f = new TF1(qPrintable(title() + particle.name()), gauss, m_min, m_max, 2);
    KineticVariable variable(particle.type());
    variable.setMomentum(momentum);
    f->SetParameters(1./variable.beta(), 0.4 * Constants::speedOfLight / (Constants::upperTofPosition-Constants::lowerTofPosition));
    f->SetLineColor(particle.color());
    f->SetNpx(1000);
    addFunction(f);
  }
}

void TOFLikelihood::update()
{
  double binWidth = (m_max - m_min) / double(m_nBins);
  for (int bin = 1; bin <= m_nBins; ++bin) {
    histogram(0)->SetBinContent(bin, 1./binWidth * m_belowThresholdHistogram->GetBinContent(bin) / m_belowThresholdHistogram->GetEntries());
    histogram(1)->SetBinContent(bin, 1./binWidth * m_aboveThresholdHistogram->GetBinContent(bin) / m_aboveThresholdHistogram->GetEntries());
  }
}

void TOFLikelihood::finalize()
{
  update();
}
