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

#include <TH2D.h>
#include <TAxis.h>

#include <QString>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <math.h>

TRDSpectrumVsPressurePlot::TRDSpectrumVsPressurePlot(unsigned short id, TRDSpectrumPlot::TRDSpectrumType spectrumType, double lowerMom, double upperMom):
  AnalysisPlot(AnalysisPlot:: SignalHeightTRD),
  H2DPlot(),
  m_id(id),
  m_spectrumType(spectrumType),
  m_lowerMomentum(lowerMom),
  m_upperMomentum(upperMom)
{
  QString strType;
  switch(m_spectrumType){
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
  int nBins = TRDReconstruction::spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDReconstruction::spectrumUpperLimit();
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }

  TH2D* histogram = new TH2D(qPrintable(title()),"", nPressureBins, minPressure, maxPressure, nBins, p);
  setAxisTitle("pressure /  mBar", TRDReconstruction::xAxisTitle(), "");
  addHistogram(histogram);
}

TRDSpectrumVsPressurePlot::~TRDSpectrumVsPressurePlot()
{
}

void TRDSpectrumVsPressurePlot::processEvent(const QVector<Hit*>&, Particle* particle, SimpleEvent* event)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  // TODO: temp sensormap
  double pressure = event->sensorData(SensorTypes::TRD_PRESSURE_SMOOTHED);

  QList<double> valuesToFill;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    if (TRDReconstruction::calculateLengthInTube)
      for (int i = 0; i < 8; ++i)
        valuesToFill << trdReconst->getEnergyDepositionOnTrackPerLengthForLayer(i);
    else
      for (int i = 0; i < 8; ++i)
        valuesToFill << trdReconst->getEnergyDepositionOnTrackForLayer(i);
    break;
  case TRDSpectrumPlot::module:
    if (TRDReconstruction::calculateLengthInTube)
      valuesToFill << trdReconst->getEnergyDepositionOnTrackPerLengthForModule(m_id);
    else
      valuesToFill << trdReconst->getEnergyDepositionOnTrackForModule(m_id);
    break;
  case TRDSpectrumPlot::channel:
    if (TRDReconstruction::calculateLengthInTube)
      valuesToFill << trdReconst->getEnergyDepositionOnTrackPerLengthForChannel(m_id);
    else
      valuesToFill << trdReconst->getEnergyDepositionOnTrackForModule(m_id);
    break;
  case TRDSpectrumPlot::layer:
    if (TRDReconstruction::calculateLengthInTube)
        valuesToFill << trdReconst->getEnergyDepositionOnTrackPerLengthForLayer(m_id);
    else
        valuesToFill << trdReconst->getEnergyDepositionOnTrackForLayer(m_id);
    break;
  }

  for (QList<double>::const_iterator it = valuesToFill.constBegin(); it != valuesToFill.constEnd(); ++it) {
    if (!(*it > 0.))
      continue;
    int iGlobalBin = histogram()->FindBin(pressure, *it);
    int iXBin, iYBin, iZBin;
    histogram()->GetBinXYZ(iGlobalBin, iXBin, iYBin, iZBin);
    double width = histogram()->GetBinWidth(iYBin);
    double weight = 1./width;
    histogram()->Fill(pressure, *it, weight);
  }
}
