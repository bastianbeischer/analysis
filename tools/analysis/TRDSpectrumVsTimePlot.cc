#include "TRDSpectrumVsTimePlot.hh"
#include "TRDSpectrumPlot.hh"

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "Helpers.hh"

#include "TRDReconstruction.hh"

#include <TH2D.h>
#include <TAxis.h>

#include <QDateTime>
#include <QString>
#include <math.h>

TRDSpectrumVsTimePlot::TRDSpectrumVsTimePlot(QDateTime first, QDateTime last, unsigned short id, TRDSpectrumPlot::TRDSpectrumType spectrumType)
  : AnalysisPlot(AnalysisPlot:: SignalHeightTRD)
  , H2DPlot()
  , m_id(id)
  , m_spectrumType(spectrumType)
{
  QString strType;
  switch(m_spectrumType){
  case TRDSpectrumPlot::completeTRD:
    strType = "complete TRD vs time";
    break;
  case TRDSpectrumPlot::module:
    strType = "module vs time";
    break;
  case TRDSpectrumPlot::channel:
    strType = "channel vs time";
    break;
  case TRDSpectrumPlot::layer:
    strType = "layer vs time";
    break;
  }

  if(m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + " spectrum");
  else
    setTitle(strType + QString(" spectrum 0x%1").arg(m_id,0,16));


  int secsPerBin = 3600*2;

  int t1 = first.toTime_t();
  int t2 = last.toTime_t();
  const unsigned int nTimeBins = qMin((t2 - t1) / secsPerBin, 500);
  int nBins = TRDReconstruction::s_spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDReconstruction::spectrumUpperLimit();
  const QVector<double>& axis = Helpers::logBinning(nBins, lowerBound, upperBound);

  TH2D* histogram = new TH2D(qPrintable(title()),"", nTimeBins,t1,t2, nBins, axis.constData());
  histogram->Sumw2();
  histogram->GetXaxis()->SetTimeDisplay(1);
  histogram->GetXaxis()->SetTimeFormat("%d-%H:%M");
  histogram->GetXaxis()->SetTimeOffset(3600, "gmt"); //dont understand this, but works at testbeam
  histogram->GetXaxis()->SetTimeDisplay(1);
  setAxisTitle("Time", TRDReconstruction::xAxisTitle(), "");
  addHistogram(histogram);
}

TRDSpectrumVsTimePlot::~TRDSpectrumVsTimePlot()
{
}

void TRDSpectrumVsTimePlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  double time = event->time();

  QVector<double> valuesToFill;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    if (TRDReconstruction::s_calculateLengthInTube) {
      for (int i = 0; i < 8; ++i)
        if (trdReconst->energyDepositionForLayer(i).isPierced)
          valuesToFill << trdReconst->energyDepositionForLayer(i).edepOnTrackPerLength;
    } else {
      for (int i = 0; i < 8; ++i)
        if (trdReconst->energyDepositionForLayer(i).isOnTRack)
          valuesToFill << trdReconst->energyDepositionForLayer(i).edepOnTrack;
    }
    break;
  case TRDSpectrumPlot::module:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForModule(m_id).isPierced)
        valuesToFill << trdReconst->energyDepositionForModule(m_id).edepOnTrackPerLength;
    } else {
      valuesToFill << trdReconst->energyDepositionForModule(m_id).edepOnTrack;
    }
    break;
  case TRDSpectrumPlot::channel:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForChannel(m_id).isPierced)
        valuesToFill << trdReconst->energyDepositionForChannel(m_id).edepOnTrackPerLength;
    } else {
      if (trdReconst->energyDepositionForChannel(m_id).isOnTRack)
        valuesToFill << trdReconst->energyDepositionForChannel(m_id).edepOnTrack;
    }
    break;
  case TRDSpectrumPlot::layer:
    if (TRDReconstruction::s_calculateLengthInTube) {
      if (trdReconst->energyDepositionForLayer(m_id).isPierced)
        valuesToFill << trdReconst->energyDepositionForLayer(m_id).edepOnTrackPerLength;
    } else {
      if (trdReconst->energyDepositionForLayer(m_id).isOnTRack)
        valuesToFill << trdReconst->energyDepositionForLayer(m_id).edepOnTrack;
    }
    break;
  }

  for (QVector<double>::const_iterator it = valuesToFill.constBegin(); it != valuesToFill.constEnd(); ++it) {
    int iGlobalBin = histogram()->FindBin(time, *it);
    int iXBin, iYBin, iZBin;
    histogram()->GetBinXYZ(iGlobalBin, iXBin, iYBin, iZBin);
    double width = histogram()->GetBinWidth(iYBin);
    double weight = 1./width;
    histogram()->Fill(time, *it, weight);
  }
}


