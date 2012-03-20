#include "AllTrackerLayersFlagEfficiency.hh"

#include "EfficiencyCollection.hh"

#include <cmath>

#include <TH1D.h>
#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Helpers.hh"
#include "Hypothesis.hh"

#include <QSpinBox>

AllTrackerLayersFlagEfficiency::AllTrackerLayersFlagEfficiency(Enums::Particles particles, bool fineBinned)
  : AnalysisPlot(Enums::MiscellaneousTracker)
  , H1DPlot()
  , m_particles(Enums::particleVector(particles))
  , m_passedCutHistograms(0)
  , m_totalHistograms(0)
{
  QString title = "all tracker layers flag efficiency";
  if (fineBinned)
    title+= " fine";
  setTitle(title);

  QVector<double> axis = Helpers::rigidityBinning(fineBinned);
  const int nBins = axis.size() - 1;

  foreach (Enums::Particle particle, m_particles) {
    QString histogramTitle = title + ' ' + Enums::label(particle);

    ParticleProperties properties(particle);

    TH1D* h = 0;

    h = new TH1D(qPrintable(histogramTitle), "", nBins, axis.constData());
    h->Sumw2();
    h->SetLineColor(properties.color());
    h->SetMarkerColor(properties.color());
    addHistogram(h, H1DPlot::P);

    h = new TH1D(qPrintable(histogramTitle + " passed"), "", nBins, axis.constData());
    h->Sumw2();
    m_passedCutHistograms.append(h);

    h = new TH1D(qPrintable(histogramTitle + " total"), "", nBins, axis.constData());
    h->Sumw2();
    m_totalHistograms.append(h);
  }

  setAxisTitle("|R| / GV", "efficiency");
}

AllTrackerLayersFlagEfficiency::~AllTrackerLayersFlagEfficiency()
{
  qDeleteAll(m_totalHistograms);
  qDeleteAll(m_passedCutHistograms);
}

void AllTrackerLayersFlagEfficiency::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(Enums::Chi2Good | Enums::InsideMagnet))
    return;

  //TODO: right albedo handling
  const Hypothesis* h = event->particle()->hypothesis();
  int i = m_particles.indexOf(h->particle());
  m_totalHistograms[i]->Fill(h->absoluteRigidity());
  if (!event->flagsSet(Enums::AllTrackerLayers))
    return;
  m_passedCutHistograms[i]->Fill(h->absoluteRigidity());
}

void AllTrackerLayersFlagEfficiency::update()
{
  foreach (Enums::Particle particle, m_particles) {
    int i = m_particles.indexOf(particle);
    for (int bin = 0; bin < m_totalHistograms[i]->GetNbinsX(); ++bin) {
      int reconstructed = m_passedCutHistograms[i]->GetBinContent(bin+1);
      int total = m_totalHistograms[i]->GetBinContent(bin+1);
      double efficiency = 0;
      double efficiencyError = 0;
      if (total != 0) {
        efficiency = double(reconstructed) / double(total);
        efficiencyError =  sqrt(efficiency * (1 - efficiency) / double(total));
      }
      histogram(i)->SetBinContent(bin+1, efficiency);
      histogram(i)->SetBinError(bin+1, efficiencyError);
    }
  }
}
