#include "MultiLayerTrackingEfficiencyPlot.hh"

#include <cmath>

#include <TH1D.h>
#include <TH2D.h>

#include "Layer.hh"
#include "TrackFinding.hh"
#include "Setup.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"

MultiLayerTrackingEfficiencyPlot::MultiLayerTrackingEfficiencyPlot(Type type) :
  AnalysisPlot(AnalysisPlot::MiscellaneousTracker),
  H2DPlot(),
  m_type(type),
  m_normHisto(0),
  m_nLayers(8),
  m_layerZ(new double[m_nLayers])
{
  QString htitle = "Multi Layer Efficiency";
  
  if (m_type == Positive)
    htitle += " positive";
  if (m_type == Negative)
    htitle += " negative";
  if (m_type == All)
    htitle += " all";
  setTitle(htitle);
  
  int nBinsY = m_nLayers;
  double y0 = 0.5;
  double y1 = m_nLayers+0.5;
  
  int nBins = 21;
  double lowerBound = 1e-1;
  double upperBound = 20.;
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }
  
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBins, p, nBinsY, y0, y1);
  setAxisTitle("R / GV", "number of layers", "");
  addHistogram(histogram);
  
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, p);

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

void MultiLayerTrackingEfficiencyPlot::processEvent(const QVector<Hit*>& hits, Particle* particle, SimpleEvent*)
{
  const Track* track = particle->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = particle->information()->flags();
  if ( !(flags & ParticleInformation::Chi2Good) )
    return;
  
  if ( !(flags & ParticleInformation::InsideMagnet) )
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
  
  double rigidity = track->rigidity();
  
  if (m_type == Positive && rigidity < 0) {
    return;
  }
  if (m_type == Negative && rigidity > 0) {
    return;
  }
  
  double absRigidity = rigidity;
  if (absRigidity < 0) {
    absRigidity *= -1;
  }
  
  histogram()->Fill(absRigidity, nbOfLayers);
  m_normHisto->Fill(absRigidity);
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
