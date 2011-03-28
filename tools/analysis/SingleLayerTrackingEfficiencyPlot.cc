#include "SingleLayerTrackingEfficiencyPlot.hh"

#include <cmath>

#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Track.hh"
#include "TrackInformation.hh"

SingleLayerTrackingEfficiencyPlot::SingleLayerTrackingEfficiencyPlot() :
  AnalysisPlot(AnalysisPlot::MiscellaneousTracker),
  H2DPlot(),
  m_normHisto(0),
  m_nLayers(8),
  m_layerZ(new double[m_nLayers])
{
  setTitle("Single Layer Efficiency");
  
  int nBinsX = 10;
  double x0 = 0.;
  double x1 = 10.;
  int nBinsY = m_nLayers;
  double y0 = 0.5;
  double y1 = m_nLayers+0.5;
  
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsX, x0, x1, nBinsY, y0, y1);
  histogram->GetXaxis()->SetTitle("rigidity / GV");
  histogram->GetYaxis()->SetTitle("layer number");
  setHistogram(histogram);

  m_normHisto = new TH2D(qPrintable(title() + "_norm"), "", nBinsX, x0, x1, nBinsY, y0, y1);

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
  setDrawOption("COL Z TEXT");
}

SingleLayerTrackingEfficiencyPlot::~SingleLayerTrackingEfficiencyPlot()
{
  delete m_normHisto;
  delete [] m_layerZ;
}

void SingleLayerTrackingEfficiencyPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if ( !(flags & TrackInformation::InsideMagnet) )
    return;

  for (int i = 0; i < m_nLayers; i++) {
    // determine if the layer has a hit matching the track
    bool beenHit = false;
    const QVector<Hit*>::const_iterator endIt = hits.end();
    for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
      Hit* hit = *it;
      if (floor(hit->position().z()) == m_layerZ[i]) {
        if (TrackFinding::isInCorridor(track, hit, 10.)) {
          beenHit = true;
          break;
        }
      }
    }

    // fill histograms
    if (beenHit)
      histogram()->Fill(track->p(), i+1);
    m_normHisto->Fill(track->p(), i+1);
  }
}

void SingleLayerTrackingEfficiencyPlot::finalize()
{
  histogram()->Divide(m_normHisto);
}
