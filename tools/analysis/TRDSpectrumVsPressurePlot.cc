#include "TRDSpectrumVsPressurePlot.hh"

#include "Particle.hh"
#include "ParticleInformation.hh"
#include "Track.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "TRDReconstruction.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "Helpers.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <math.h>

TRDSpectrumVsPressurePlot::TRDSpectrumVsPressurePlot(unsigned short id, TRDSpectrumPlot::TRDSpectrumType spectrumType, double lowerMom, double upperMom):
  AnalysisPlot(Enums:: SignalHeightTRD),
  H2DPlot(),
  m_id(id),
  m_spectrumType(spectrumType),
  m_lowerMomentum(lowerMom),
  m_upperMomentum(upperMom)
{
  QString strType;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    strType = "complete TRD vs pressure";
    break;
  case TRDSpectrumPlot::module:
    strType = "module vs pressure";
    break;
  case TRDSpectrumPlot::channel:
    strType = "channel vs pressure";
    break;
  case TRDSpectrumPlot::layer:
    strType = "layer vs pressure";
    break;
  }

  if (m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum (%1 GeV to %2 GeV)").arg(m_lowerMomentum).arg(m_upperMomentum));
  else
    setTitle(strType + QString(" spectrum 0x%1 (%2 GeV to %3 GeV)").arg(m_id,0,16).arg(m_lowerMomentum).arg(m_upperMomentum));


  const unsigned int nPressureBins = 200;
  const double minPressure = 1070;
  const double maxPressure = 1115;
  int nBins = TRDReconstruction::s_spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDReconstruction::spectrumUpperLimit();
  const QVector<double>& axis = Helpers::logBinning(nBins, lowerBound, upperBound);

  TH2D* histogram = new TH2D(qPrintable(title()),"", nPressureBins, minPressure, maxPressure, nBins, axis.constData());
  setAxisTitle("pressure /  mBar", TRDReconstruction::xAxisTitle(), "");

  addHistogram(histogram);
}

TRDSpectrumVsPressurePlot::~TRDSpectrumVsPressurePlot()
{
}

void TRDSpectrumVsPressurePlot::processEvent(const AnalyzedEvent* event)
{
  const TRDReconstruction* trdReconst = event->particle()->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  // TODO: temp sensormap
  double pressure = event->simpleEvent()->sensorData(SensorTypes::TRD_PRESSURE_SMOOTHED);

  QVector<double> valuesToFill;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    if (TRDReconstruction::s_calculateLengthInTube) {
      for (int i = 0; i < 8; ++i)
        if (trdReconst->energyDepositionForLayer(i).isPierced) {
          valuesToFill << trdReconst->energyDepositionForLayer(i).edepOnTrackPerLength;
        }
    } else {
      for (int i = 0; i < 8; ++i)
        if (trdReconst->energyDepositionForLayer(i).isOnTRack) {
          valuesToFill << trdReconst->energyDepositionForLayer(i).edepOnTrack;
        }
    }
    break;
  case TRDSpectrumPlot::module:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForModule(m_id).isPierced) {
        valuesToFill << trdReconst->energyDepositionForModule(m_id).edepOnTrackPerLength;
      }
    } else {
      if (trdReconst->energyDepositionForModule(m_id).isOnTRack) {
        valuesToFill << trdReconst->energyDepositionForModule(m_id).edepOnTrack;
      }
    }
    break;
  case TRDSpectrumPlot::channel:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForChannel(m_id).isPierced) {
        valuesToFill << trdReconst->energyDepositionForChannel(m_id).edepOnTrackPerLength;
      }
    } else {
      if (trdReconst->energyDepositionForChannel(m_id).isOnTRack) {
        valuesToFill << trdReconst->energyDepositionForChannel(m_id).edepOnTrack;
      }
    }
    break;
  case TRDSpectrumPlot::layer:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForLayer(m_id).isPierced) {
        valuesToFill << trdReconst->energyDepositionForLayer(m_id).edepOnTrackPerLength;
      }
    } else {
      if (trdReconst->energyDepositionForLayer(m_id).isOnTRack) {
        valuesToFill << trdReconst->energyDepositionForLayer(m_id).edepOnTrack;
      }
    }
    break;
  }

  for (QVector<double>::const_iterator it = valuesToFill.constBegin(); it != valuesToFill.constEnd(); ++it) {
    int iGlobalBin = histogram()->FindBin(pressure, *it);
    int iXBin, iYBin, iZBin;
    histogram()->GetBinXYZ(iGlobalBin, iXBin, iYBin, iZBin);
    double width = histogram()->GetBinWidth(iYBin);
    double weight = 1./width;
    histogram()->Fill(pressure, *it, weight);
  }
}
