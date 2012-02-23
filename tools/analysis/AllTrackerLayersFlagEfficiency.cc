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

AllTrackerLayersFlagEfficiency::AllTrackerLayersFlagEfficiency(bool fineBinned)
  : AnalysisPlot(Enums::MiscellaneousTracker)
  , H1DPlot()
  , m_passedCutHistogram(0)
  , m_totalHistogram(0)
{
  QString title = "all tracker layers flag efficiency";
  if (fineBinned)
    title+= " fine";
  setTitle(title);

  QVector<double> axis = Helpers::rigidityBinning(fineBinned);
  int axisSize = 2 * axis.size();
  for (int i = 0; i < axisSize; i+= 2) {
    double value = axis.at(i);
    axis.prepend(-value);
  }
  const int nBins = axis.size() - 1;

  TH1D* histogram = new TH1D(qPrintable(title), "", nBins, axis.constData());
  histogram->Sumw2();
  setAxisTitle("R / GV", "efficiency");
  addHistogram(histogram, H1DPlot::P);

  m_passedCutHistogram = new TH1D(qPrintable(title + " passed"), "", nBins, axis.constData());
  m_passedCutHistogram->Sumw2();
  m_totalHistogram = new TH1D(qPrintable(title + " total"), "", nBins, axis.constData());
  m_totalHistogram->Sumw2();
}

AllTrackerLayersFlagEfficiency::~AllTrackerLayersFlagEfficiency()
{
  delete m_totalHistogram;
  delete m_passedCutHistogram;
}

void AllTrackerLayersFlagEfficiency::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->goodTrack();
  if (!track)
    return;
  if (!event->flagsSet(ParticleInformation::Chi2Good | ParticleInformation::InsideMagnet))
    return;

  //TODO: right albedo handling
  double rigidity = event->particle()->hypothesis()->rigidity();
  m_totalHistogram->Fill(rigidity);
  if (!event->flagsSet(ParticleInformation::AllTrackerLayers))
    return;
  m_passedCutHistogram->Fill(rigidity);
}

void AllTrackerLayersFlagEfficiency::update()
{
  for (int i = 0; i < m_totalHistogram->GetNbinsX(); ++i) {
    int reconstructed = m_passedCutHistogram->GetBinContent(i+1);
    int total = m_totalHistogram->GetBinContent(i+1);
    double efficiency = 0;
    double efficiencyError = 0;
    if (total != 0) {
      efficiency = double(reconstructed) / double(total);
      efficiencyError =  sqrt(efficiency * (1 - efficiency) / double(total));
    }
    histogram()->SetBinContent(i+1, efficiency);
    histogram()->SetBinError(i+1, efficiencyError);
  }
}
