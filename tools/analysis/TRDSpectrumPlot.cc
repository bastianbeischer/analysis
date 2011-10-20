#include "TRDSpectrumPlot.hh"

#include <TH1D.h>
#include <TLatex.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Settings.hh"
#include "TRDReconstruction.hh"
#include "Corrections.hh"
#include "Helpers.hh"

#include <math.h>
#include <QList>

TRDSpectrumPlot::TRDSpectrumPlot(unsigned short id, TRDSpectrumType spectrumType) :
  AnalysisPlot(Enums::SignalHeightTRD),
  H1DPlot(),
  m_id(id),
  m_spectrumType(spectrumType)
{
  QString strType;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    strType = "complete TRD";
    break;
  case TRDSpectrumPlot::module:
    strType = "Module";
    break;
  case TRDSpectrumPlot::channel:
    strType = "Channel";
    break;
  case TRDSpectrumPlot::layer:
    strType = "TRD layer";
    break;
  }

  if (m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum"));
  else if (m_spectrumType == TRDSpectrumPlot::layer)
    setTitle(strType + QString(" spectrum %1").arg(m_id));
  else
    setTitle(strType + QString(" spectrum 0x%1").arg(m_id,0,16));

  int nBins = TRDReconstruction::s_spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDReconstruction::spectrumUpperLimit();
  const QVector<double>& axis = Helpers::logBinning(nBins, lowerBound, upperBound);

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, axis.constData());
  histogram->Sumw2();
  setAxisTitle(TRDReconstruction::xAxisTitle(), "entries");
  addHistogram(histogram, H1DPlot::HIST);
  setDrawOption(H1DPlot::HIST);

  addLatex(RootPlot::newLatex(0.6, 0.85));
  addLatex(RootPlot::newLatex(0.65, 0.8));
  addLatex(RootPlot::newLatex(0.7, 0.75));
  addLatex(RootPlot::newLatex(0.7, 0.7));
}

TRDSpectrumPlot::~TRDSpectrumPlot()
{
}

void TRDSpectrumPlot::processEvent(const AnalyzedEvent* event)
{
  const TRDReconstruction* trdReconst = event->particle()->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

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
    int iBin = histogram()->FindBin(*it);
    double width = histogram()->GetBinWidth(iBin);
    double weight = 1./width;
    if (iBin == 0 || iBin == histogram()->GetNbinsX()+1)
      weight = 1.;
    histogram()->Fill(*it, weight);
  }
}

void TRDSpectrumPlot::update()
{
  latex(0)->SetTitle(qPrintable(title()));
  latex(1)->SetTitle(qPrintable(QString("entries = %1").arg(histogram()->GetEntries())));
  latex(2)->SetTitle(qPrintable(QString("uflow = %1").arg(histogram()->GetBinContent(0))));
  latex(3)->SetTitle(qPrintable(QString("oflow = %1").arg(histogram()->GetBinContent(histogram()->GetNbinsX()+1))));
}
