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
#include "ProjectionControlWidget.hh"
#include "Helpers.hh"

#include <QSpinBox>

MultiLayerTrackingEfficiencyPlot::MultiLayerTrackingEfficiencyPlot(Type type) :
  AnalysisPlot(Enums::MiscellaneousTracker),
  H2DProjectionPlot(),
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
  
  const int nBins = 21;
  const double min = 0.1;
  const double max = 20;
  const QVector<double>& axis = Helpers::logBinning(nBins, min, max);
  
  TH2D* histogram = new TH2D(qPrintable(title()), "", nBins, axis.constData(), nBinsY, y0, y1);
  setAxisTitle("R / GV", "number of layers", "");
  addHistogram(histogram);
  
  m_normHisto = new TH1D(qPrintable(title() + "_norm"), "", nBins, axis.constData());

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
  
  controlWidget()->spinBox()->setMaximum(histogram->GetNbinsY());
}

MultiLayerTrackingEfficiencyPlot::~MultiLayerTrackingEfficiencyPlot()
{
  delete m_normHisto;
  delete [] m_layerZ;
}

void MultiLayerTrackingEfficiencyPlot::processEvent(const AnalyzedEvent* event)
{
  const Track* track = event->particle()->track();

  if (!track || !track->fitGood())
    return;

  ParticleInformation::Flags flags = event->particle()->information()->flags();
  if ( !(flags & ParticleInformation::Chi2Good) )
    return;
  
  if ( !(flags & ParticleInformation::InsideMagnet) )
    return;

  int nbOfLayers = 0;
  for (int i = 0; i < m_nLayers; i++) {
    // determine if the layer has a hit matching the track
    const QVector<Hit*>::const_iterator endIt = event->clusters().end();
    for (QVector<Hit*>::const_iterator it = event->clusters().begin(); it != endIt; ++it) {
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
