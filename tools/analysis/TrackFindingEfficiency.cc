#include "TrackFindingEfficiency.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "Cluster.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TF1.h>
#include <TVector3.h>

#include <QVector>
#include <QDebug>
#include <QWidget>
#include <QSpinBox>
#include <QLayout>
#include <QLabel>
#include <QPushButton>

#include <cmath>
#include <vector>

TrackFindingEfficiency::TrackFindingEfficiency(Type type) : 
AnalysisPlot(AnalysisPlot::MiscellaneousTracker), 
H1DPlot(),
m_type(type)
{
  m_nTotal = 0;
  m_nReconstructed = 0;
  
  TH1::SetDefaultSumw2(kTRUE);
  QString title = "Track finding efficiency";
  
  if (m_type == Negative) {
    title += " - negative";
  }
  if (m_type == Positive) {
    title += " - positive";
  }
  if (m_type == All) {
    title += " - all";
  }
  
  setTitle(title);
  
//  const double step = 0.1;
//  const double min = 0.05;
//  const double max = 10.05;
//  int nBins = (int)((max - min) / step);
//  
//  TH1D* histogram = new TH1D("reconstruction efficiency", "", nBins, min, max);
  
  const int nBins = 21;
  const double min = 0.1;
  const double max = 20;
  const QVector<double>& axis = logBinning(nBins, min, max);
  
  TH1D* histogram = new TH1D("reconstruction efficiency", "", nBins, axis.constData());
  
  setAxisTitle("abs(rigidity / GV)", "efficiency");
  addHistogram(histogram);
  
//  m_reconstructed = new TH1D("reconstructed", "", nBins, min, max);
//  m_total = new TH1D("total", "", nBins, min, max);
  
  m_reconstructed = new TH1D("reconstructed", "",  nBins, axis.constData());
  m_total = new TH1D("total", "",  nBins, axis.constData());
  
  addLatex(RootPlot::newLatex(.3, .85));
}


TrackFindingEfficiency::~TrackFindingEfficiency()
{
  delete m_reconstructed;
  m_reconstructed = 0;
  delete m_total;
  m_total = 0;
}

void TrackFindingEfficiency::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent* event)
{
  bool fillHistogram = false;
  double rigidity = 0;
  
  const Settings* settings = SettingsManager::instance()->settingsForEvent(event);
  if (settings && settings->situation() == Settings::Testbeam11) {
    if (m_type == Negative && settings->polarity() > 0)
      return;
    if (m_type == Positive && settings->polarity() < 0)
      return;
    rigidity = settings->momentum();
    
    fillHistogram = true;
  } else if (event->contentType() == SimpleEvent::MonteCarlo) {
    int mcPdgId = event->MCInformation()->primary()->pdgID;
    Particle mcParticle(mcPdgId);
    
    rigidity = event->MCInformation()->primary()->initialMomentum.Mag() / mcParticle.charge();
    
    if (m_type == Negative && rigidity > 0)
      return;
    if (m_type == Positive && rigidity < 0)
      return;
    
    fillHistogram = true;
  }
  
  ++m_nTotal;
  if (fillHistogram)
    m_total->Fill(qAbs(rigidity));
  
  const Track* track = particle->track();
  if (!track || !track->fitGood())
    return;
  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & (ParticleInformation::Chi2Good)))
    return;

  ++m_nReconstructed;
  if (fillHistogram)
    m_reconstructed->Fill(qAbs(rigidity));
}

void TrackFindingEfficiency::update()
{
  TH1::SetDefaultSumw2(kTRUE);
  histogram()->Divide(m_reconstructed, m_total);
  
  double efficiency = double(m_nReconstructed) / double(m_nTotal);
  double recError = sqrt(m_nReconstructed);
  double totError = sqrt(m_nTotal);
  
  double efficiencyError = sqrt(pow(recError/double(m_nTotal), 2) + pow((double(m_nReconstructed)*totError)/double(pow(m_nTotal, 2)), 2));
  
  latex(0)->SetTitle(qPrintable(QString("efficiency  = %1 #pm %2").arg(efficiency).arg(efficiencyError)));
}
  
void TrackFindingEfficiency::finalize()
{
  update();
}

QVector<double> TrackFindingEfficiency::logBinning(unsigned int nBins, double min, double max) {
  QVector<double> binning;
  const double delta = (log(max) / log(min) - 1) / nBins;
  for (unsigned int i = 0; i <= nBins; ++i) {
    binning.append(pow(min, 1 + delta * i));
  }
  return binning;
} 
