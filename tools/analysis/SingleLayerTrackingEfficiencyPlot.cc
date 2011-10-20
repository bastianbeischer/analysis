#include "SingleLayerTrackingEfficiencyPlot.hh"

#include <cmath>

#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Helpers.hh"

SingleLayerTrackingEfficiencyPlot::SingleLayerTrackingEfficiencyPlot(Enums::ChargeSigns type)
  : AnalysisPlot(Enums::MiscellaneousTracker)
  , H2DPlot()
  , m_type(type)
  , m_normHisto(0)
  , m_nLayers(8)
  , m_layerZ(new double[m_nLayers])
{
  QString htitle = "Single Layer Efficiency";

  if (m_type == Enums::Positive)
    htitle += " positive";
  if (m_type == Enums::Negative)
    htitle += " negative";
  if (m_type == (Enums::Positive | Enums::Negative))
    htitle += " all";
  setTitle(htitle);

  const int nBins = 21;
  const double min = 0.1;
  const double max = 20;
  const QVector<double>& axis = Helpers::logBinning(nBins, min, max);

  int nBinsY = m_nLayers;
  double y0 = 0.5;
  double y1 = m_nLayers+0.5;

  TH2D* histogram = new TH2D(qPrintable(title()), "", nBins, axis.constData(), nBinsY, y0, y1);
  setAxisTitle("rigidity / GV", "layer number", "");
  addHistogram(histogram);

  m_normHisto = new TH2D(qPrintable(title() + "_norm"), "", nBins, axis.constData(), nBinsY, y0, y1);

  int i = 0;
  Setup* setup = Setup::instance();
  const LayerIterator endIt = setup->lastLayer();
  for (LayerIterator it = setup->firstLayer(); it != endIt; ++it) {
    Layer* layer = *it;
    double layerZ = floor(layer->z());
    if (layerZ > -240 && layerZ < 240) {
      m_layerZ[i] = layerZ;
      i++;
    }
  }
  setDrawOption(COLZTEXT);
}

SingleLayerTrackingEfficiencyPlot::~SingleLayerTrackingEfficiencyPlot()
{
  delete m_normHisto;
  delete[] m_layerZ;
}

void SingleLayerTrackingEfficiencyPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = event->particle()->information()->flags();
  if ( !(flags & ParticleInformation::InsideMagnet) )
    return;

  for (int i = 0; i < m_nLayers; i++) {
    // determine if the layer has a hit matching the track
    bool beenHit = false;
    const QVector<Hit*>::const_iterator endIt = event->clusters().end();
    for (QVector<Hit*>::const_iterator it = event->clusters().begin(); it != endIt; ++it) {
      Hit* hit = *it;
      if (floor(hit->position().z()) == m_layerZ[i]) {
        if (TrackFinding::isInCorridor(track, hit, 10.)) {
          beenHit = true;
          break;
        }
      }
    }

    double rigidity = track->rigidity();

    if (m_type == Enums::Positive && rigidity < 0)
      return;
    if (m_type == Enums::Negative && rigidity > 0)
      return;

    // fill histograms
    if (beenHit)
      histogram()->Fill(qAbs(rigidity), i+1);
    m_normHisto->Fill(qAbs(rigidity), i+1);
  }
}

void SingleLayerTrackingEfficiencyPlot::finalize()
{
  histogram()->Divide(m_normHisto);
}
