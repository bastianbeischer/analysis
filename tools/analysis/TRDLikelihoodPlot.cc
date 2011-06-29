#include "TRDLikelihoodPlot.hh"

#include <TH1D.h>

#include "TRDElectronIdentifierLikelihood.hh"
#include "SimpleEvent.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SettingsManager.hh"

TRDLikelihoodPlot::TRDLikelihoodPlot(Topic topic)
  : AnalysisPlot(topic)
  , H1DPlot()
  , m_NonTRHisto(0)
  , m_TRHisto(0)
  , m_EIdentifierLH(new TRDElectronIdentifierLikelihood())
{
  setTitle("-log(L) distribution");

  m_NonTRHisto = new TH1D(qPrintable(title() + "_nonTR"), "", 100, 0, 2);
  m_TRHisto = new TH1D(qPrintable(title() + "_TR"), "", 100, 0, 2);
  m_TRHisto->SetLineColor(kRed);
  setAxisTitle("-ln(L)", "entries");

  addHistogram(m_NonTRHisto, H1DPlot::HIST);
  addHistogram(m_TRHisto, H1DPlot::HIST);
  setDrawOption(H1DPlot::HIST);
}

void TRDLikelihoodPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent* event)
{

  bool isTRParticle = truthMcIsElectron(event);
  double lhTR = 0.;
  bool validEvent = false;
  //bool identifiedAsTRParticle =
  m_EIdentifierLH->isElectronish(hits, particle, event, validEvent, lhTR);

  if (!validEvent)
    return;

  if (isTRParticle)
    m_TRHisto->Fill(lhTR);
  else
    m_NonTRHisto->Fill(lhTR);
}

void TRDLikelihoodPlot::finalize()
{
  m_NonTRHisto->Sumw2();
  m_NonTRHisto->Scale(1./m_NonTRHisto->GetEntries());
  m_TRHisto->Sumw2();
  m_TRHisto->Scale(1./m_TRHisto->GetEntries());
}

bool TRDLikelihoodPlot::truthMcIsElectron(SimpleEvent* event)
{
  if (event->contentType() == SimpleEvent::MonteCarlo)
    return qAbs(event->MCInformation()->primary()->pdgID) == 11;

  double c1Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV1);
  double c2Signal = event->sensorData(SensorTypes::BEAM_CHERENKOV2);

  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  bool eAboveCherenkov = settings->isAboveThreshold(Constants::electronMass);

  return (eAboveCherenkov && c1Signal > 200 && c2Signal > 200);
}
