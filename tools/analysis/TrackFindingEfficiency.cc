#include "TrackFindingEfficiency.hh"

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Constants.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "Cluster.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Hit.hh"
#include "Helpers.hh"

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

TrackFindingEfficiency::TrackFindingEfficiency(Type type)
  : AnalysisPlot(AnalysisPlot::MiscellaneousTracker)
  , H1DPlot()
  , m_type(type)
{
  m_nTotal = 0;
  m_nReconstructed = 0;

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

  const int nBins = 21;
  const double min = 0.1;
  const double max = 20;
  const QVector<double>& axis = Helpers::logBinning(nBins, min, max);

  TH1D* histogram = new TH1D("reconstruction efficiency", "", nBins, axis.constData());
  histogram->Sumw2();

  setAxisTitle("abs(rigidity / GV)", "efficiency");
  addHistogram(histogram);

  m_reconstructed = new TH1D("reconstructed", "", nBins, axis.constData());
  m_reconstructed->Sumw2();
  m_total = new TH1D("total", "", nBins, axis.constData());
  m_total->Sumw2();

  addLatex(RootPlot::newLatex(.3, .85));
}


TrackFindingEfficiency::~TrackFindingEfficiency()
{
  delete m_reconstructed;
  delete m_total;
}

void TrackFindingEfficiency::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
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
    if (!event->MCInformation()->primary()->isInsideMagnet())
      return;

    int mcPdgId = event->MCInformation()->primary()->pdgID;
    Particle mcParticle(mcPdgId);
    rigidity = event->MCInformation()->primary()->initialMomentum.Mag() / mcParticle.charge();

    if (!isTriggerEvent(QVector<Hit*>::fromStdVector(event->hits())))
      return;

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
  histogram()->Divide(m_reconstructed, m_total);
  double efficiency = double(m_nReconstructed) / double(m_nTotal);
  double recError = sqrt(m_nReconstructed);
  double totError = sqrt(m_nTotal);
  double efficiencyError = efficiency * Helpers::addQuad(recError/m_nReconstructed, totError/m_nTotal);

  latex()->SetTitle(qPrintable(QString("efficiency  = %1 #pm %2").arg(efficiency).arg(efficiencyError)));
}

void TrackFindingEfficiency::finalize()
{
  update();
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