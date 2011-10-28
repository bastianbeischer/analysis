#include "TOFProbabilityDensityFunction.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SensorTypes.hh"
#include "Particle.hh"
#include "SimpleEvent.hh"
#include "Track.hh"
#include "KineticVariable.hh"

#include <TH1.h>
#include <TF1.h>
#include <TMath.h>
#include <TLegend.h>
#include <TLegendEntry.h>

TOFProbabilityDensityFunction::TOFProbabilityDensityFunction()
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
  TLegend* legend = new TLegend(.12, .70, .23, .88);
  TH1D* histogram = 0;
  histogram = new TH1D(qPrintable(title() + " below threshold"), "", m_nBins, m_min, m_max);
  histogram->SetLineColor(kBlack);
  legend->AddEntry(histogram, "below threshold", "L");
  addHistogram(histogram);
  histogram = new TH1D(qPrintable(title() + " above threshold"), "", m_nBins, m_min, m_max);
  histogram->SetLineColor(kRed);
  legend->AddEntry(histogram, "above threshold", "L");
  addHistogram(histogram);
  setAxisTitle("1 / #beta", "");
  yAxis()->SetRangeUser(1e-6, 3.);
  QVector<Enums::Particle> particles = QVector<Enums::Particle>()
    << Enums::Proton << Enums::PiPlus << Enums::Electron;
  foreach (Enums::Particle particleType, particles) {
    Particle particle(particleType);
    TLegendEntry* entry = legend->AddEntry(static_cast<TObject*>(0), qPrintable(particle.name()), "L");
    entry->SetLineColor(particle.color());
    entry->SetLineWidth(2);
  }
  addLegend(legend);
}

TOFProbabilityDensityFunction::~TOFProbabilityDensityFunction()
{
}

void TOFProbabilityDensityFunction::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good | ParticleInformation::BetaGood))
    return;

  const Settings* settings = event->settings();
  if (settings && settings->situation() != Settings::Testbeam11)
    return;

  addFunctions(settings->momentum());
  
  double signal1 = event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double signal2 = event->simpleEvent()->sensorData(SensorTypes::BEAM_CHERENKOV2);

  double threshold = 200.;
  if (signal1 < threshold && signal2 < threshold)
    m_belowThresholdHistogram->Fill(1./event->particle()->beta());
  if (signal1 > threshold && signal2 > threshold)
    m_aboveThresholdHistogram->Fill(1./event->particle()->beta());
}

double gauss(double* x, double* p)
{
  return TMath::Gaus(x[0], p[0], p[1], true);
}

void TOFProbabilityDensityFunction::addFunctions(double momentum)
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
  TLegendEntry* entry = legend()->AddEntry(static_cast<TObject*>(0), qPrintable(QString("%1 GeV").arg(momentum, 0, 'f', 2, ' ')), "L");
  entry->SetLineStyle(m_momenta.size());
  entry->SetLineWidth(2);
}

void TOFProbabilityDensityFunction::update()
{
  double binWidth = double(m_max - m_min) / double(m_nBins);
  for (int bin = 1; bin <= m_nBins; ++bin) {
    histogram(0)->SetBinContent(bin, 1./binWidth * m_belowThresholdHistogram->GetBinContent(bin) / m_belowThresholdHistogram->GetEntries());
    histogram(1)->SetBinContent(bin, 1./binWidth * m_aboveThresholdHistogram->GetBinContent(bin) / m_aboveThresholdHistogram->GetEntries());
  }
}

void TOFProbabilityDensityFunction::finalize()
{
  update();
}
