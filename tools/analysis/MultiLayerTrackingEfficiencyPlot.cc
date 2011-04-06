#include "MultiLayerTrackingEfficiencyPlot.hh"

#include <cmath>

#include <TH1D.h>
#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Track.hh"
#include "TrackInformation.hh"

MultiLayerTrackingEfficiencyPlot::MultiLayerTrackingEfficiencyPlot() :
  AnalysisPlot(AnalysisPlot::MiscellaneousTracker),
  H2DPlot(),
  m_normHisto(0),
  m_nLayers(8),
  m_layerZ(new double[m_nLayers])
{
  setTitle("Multi Layer Efficiency");
  
  int nBinsX = 10;
  double x0 = 0.;
  double x1 = 10.;
  int nBinsY = m_nLayers;
  double y0 = 0.5;
  double y1 = m_nLayers+0.5;
  
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBinsX, x0, x1, nBinsY, y0, y1);
  setAxisTitle("R / GV", "layer number", "");
  addHistogram(histogram);

  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nBinsX, x0, x1);

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

MultiLayerTrackingEfficiencyPlot::~MultiLayerTrackingEfficiencyPlot()
{
  delete m_normHisto;
  delete [] m_layerZ;
}

void MultiLayerTrackingEfficiencyPlot::processEvent(const QVector<Hit*>& hits, Track* track, SimpleEvent*)
{
  if (!track || !track->fitGood())
    return;

  TrackInformation::Flags flags = track->information()->flags();
  if ( !(flags & TrackInformation::InsideMagnet) )
    return;

  int nbOfLayers = 0;
  for (int i = 0; i < m_nLayers; i++) {
    // determine if the layer has a hit matching the track
    const QVector<Hit*>::const_iterator endIt = hits.end();
    for (QVector<Hit*>::const_iterator it = hits.begin(); it != endIt; ++it) {
      Hit* hit = *it;
      if (floor(hit->position().z()) == m_layerZ[i]) {
        if (TrackFinding::isInCorridor(track, hit, 10.)) {
          nbOfLayers++;
          break;
        }
      }
    }
  }
  histogram()->Fill(track->rigidity(), nbOfLayers);
  m_normHisto->Fill(track->rigidity());
}

void MultiLayerTrackingEfficiencyPlot::finalize()
{
  for (int i = 1; i <= histogram()->GetNbinsX(); i++) {
    for (int j = 1; j <= histogram()->GetNbinsY(); j++) {
      double bc = histogram()->GetBinContent(i,j);
      histogram()->SetBinContent(i,j,bc/m_normHisto->GetBinContent(i));
    }
  }
}
