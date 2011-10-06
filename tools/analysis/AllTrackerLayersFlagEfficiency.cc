#include "AllTrackerLayersFlagEfficiency.hh"

#include "EfficiencyCorrectionSettings.hh"
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

#include <QSpinBox>

AllTrackerLayersFlagEfficiency::AllTrackerLayersFlagEfficiency(EfficiencyCorrectionSettings::FoldingType type)
  : AnalysisPlot(Enums::MiscellaneousTracker)
  , H1DPlot()
  , m_afterCutHisto(0)
  , m_normHisto(0)
{
  const QString& htitle = QString("All Tracker Layers Flag Efficiency - ") + EfficiencyCorrectionSettings::instance()->foldingTypeName(type);
  int nBinsData = EfficiencyCorrectionSettings::numberOfBins(type);
  setTitle(htitle);

  const double minData = 0.1;
  const double maxData = 20;
  QVector<double> axis = Helpers::logBinning(nBinsData, minData, maxData);
  int axisSize = 2 * axis.size();
  for (int i = 0; i < axisSize; i+= 2) {
    double value = axis.at(i);
    axis.prepend(-value);
  }
  const int nBins = axis.size() - 1;

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, axis.constData());
  histogram->Sumw2();
  setAxisTitle("R / GV", "efficiency");
  addHistogram(histogram, H1DPlot::P);

  m_afterCutHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, axis.constData());
  m_afterCutHisto->Sumw2();
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, axis.constData());
  m_normHisto->Sumw2();
}

AllTrackerLayersFlagEfficiency::~AllTrackerLayersFlagEfficiency()
{
  delete m_normHisto;
  delete m_afterCutHisto;
}

void AllTrackerLayersFlagEfficiency::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if (!(flags & ParticleInformation::Chi2Good))
    return;

  if (!(flags & ParticleInformation::InsideMagnet))
    return;
  //TODO: right albedo handling
  double rigidity = track->rigidity();

  m_normHisto->Fill(rigidity);

  if (!(flags & ParticleInformation::AllTrackerLayers))
    return;

  m_afterCutHisto->Fill(rigidity);
}

void AllTrackerLayersFlagEfficiency::update()
{
  histogram()->Divide(m_afterCutHisto, m_normHisto);
}
