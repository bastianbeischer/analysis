#include "TrackFindingEfficiency.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Hit.hh"
#include "Helpers.hh"
#include "ParticleProperties.hh"
#include "ParticleDB.hh"

#include <TH1D.h>
#include <TLatex.h>
#include <TVector3.h>

#include <QVector>
#include <QDebug>

#include <cmath>
#include <vector>

TrackFindingEfficiency::TrackFindingEfficiency(bool fineBinned)
  : AnalysisPlot(Enums::MiscellaneousTracker)
  , H1DPlot()
  , m_reconstructed(0)
  , m_total(0)
  , m_nTotal(0)
  , m_nReconstructed(0)
{
  QString title = "track finding efficiency";
  if (fineBinned)
    title+= " fine";
  setTitle(title);

  QVector<double> axis = Helpers::rigidityBinning(fineBinned);
  int axisSize = axis.size()*2;
  for (int i = 0; i < axisSize; i+=2) {
    double value = axis.at(i);
    axis.prepend(-value);
  }
  const int nBins = axis.size() - 1;

  TH1D* histogram = new TH1D(qPrintable(title + " reconstruction efficiency"), "", nBins, axis.constData());
  histogram->Sumw2();
  setAxisTitle("R / GV", "efficiency");
  addHistogram(histogram);

  m_reconstructed = new TH1D(qPrintable(title + " reconstructed"), "", nBins, axis.constData());
  m_reconstructed->Sumw2();
  m_total = new TH1D(qPrintable(title + " total"), "", nBins, axis.constData());
  m_total->Sumw2();

  addLatex(RootPlot::newLatex(.3, .85));
}


TrackFindingEfficiency::~TrackFindingEfficiency()
{
  delete m_reconstructed;
  delete m_total;
}

void TrackFindingEfficiency::processEvent(const AnalyzedEvent* event)
{
  bool fillHistogram = false;
  double rigidity = 0;

  const Settings* settings = event->settings();
  if (settings && settings->situation() == Enums::Testbeam11) {
    rigidity = settings->momentum();
    fillHistogram = true;
  } else if (event->simpleEvent()->contentType() == SimpleEvent::MonteCarlo) {
    if (!event->simpleEvent()->MCInformation()->primary()->isInsideMagnet())
      return;
    //TODO: albedo handling
    int mcPdgId = event->simpleEvent()->MCInformation()->primary()->pdgID;
    const ParticleProperties* mcParticle = ParticleDB::instance()->lookupPdgId(mcPdgId);
    rigidity = event->simpleEvent()->MCInformation()->primary()->initialMomentum.Mag() / mcParticle->charge();

    if (!isTriggerEvent(QVector<Hit*>::fromStdVector(event->simpleEvent()->hits())))
      return;
    fillHistogram = true;
  }

  ++m_nTotal;
  if (fillHistogram)
    m_total->Fill(rigidity);

  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good))
    return;

  ++m_nReconstructed;
  if (fillHistogram)
    m_reconstructed->Fill(rigidity);
}

void TrackFindingEfficiency::update()
{
  for (int i = 0; i < m_total->GetNbinsX(); ++i) {
    int reconstructed = m_reconstructed->GetBinContent(i+1);
    int total = m_total->GetBinContent(i+1);
    double efficiency = 0;
    double efficiencyError = 0;
    if (total != 0) {
      efficiency = double(reconstructed) / double(total);
      efficiencyError =  sqrt(efficiency * (1 - efficiency) / double(total));
    }
    histogram()->SetBinContent(i+1, efficiency);
    histogram()->SetBinError(i+1, efficiencyError);
  }
  double efficiency = double(m_nReconstructed) / double(m_nTotal);
  double efficiencyError = sqrt(efficiency * (1 - efficiency) / m_nTotal);
  latex()->SetTitle(qPrintable(QString("efficiency  = %1 #pm %2").arg(efficiency).arg(efficiencyError)));
}

bool TrackFindingEfficiency::isTriggerEvent(const QVector<Hit*>& clusters)
{
  const QVector<Hit*>::const_iterator endIt = clusters.end();
  bool hitUpperTof = false;
  bool hitLowerTof = false;
  for (QVector<Hit*>::const_iterator clusterIt = clusters.begin(); clusterIt != endIt; ++clusterIt) {
    Hit* hit = *clusterIt;
    if (hit->type() == Hit::tof) {
      TOFCluster* tofCluster = static_cast<TOFCluster*>(hit);
      if (tofCluster->position().z() > 0)
        hitUpperTof = true;
      if (tofCluster->position().z() < 0)
        hitLowerTof = true;
    }
  }
  return (hitUpperTof && hitLowerTof);
}
