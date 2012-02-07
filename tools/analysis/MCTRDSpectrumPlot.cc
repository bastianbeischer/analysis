#include "MCTRDSpectrumPlot.hh"
#include "Helpers.hh"

#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>

#include "SimpleEvent.hh"
#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "ParticleDB.hh"
#include "ParticleProperties.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "TRDSpectrumPlot.hh"
#include "TRDReconstruction.hh"
#include "Corrections.hh"
#include "RootStyle.hh"

#include <math.h>

MCTRDSpectrumPlot::MCTRDSpectrumPlot(unsigned short id, TRDSpectrumPlot::TRDSpectrumType spectrumType)
  : AnalysisPlot(Enums::MonteCarloTRD)
  , H1DPlot()
  , m_colorCounter(0)
  , m_id(id)
  , m_spectrumType(spectrumType)
{
  QString strType;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    strType = "MC complete TRD";
    break;
  case TRDSpectrumPlot::module:
    strType = "MC Module";
    break;
  case TRDSpectrumPlot::channel:
    strType = "MC Channel";
    break;
  case TRDSpectrumPlot::layer:
    strType = "MC TRD layer";
    break;
  }

  setTitle(strType);

  setAxisTitle(TRDReconstruction::xAxisTitle(), "entries");

  TLegend* legend = new TLegend(.72, .72, .98, .98);
  legend->SetFillColor(kWhite);
  legend->SetMargin(.7);
  addLegend(legend);

  setDrawOption(H1DPlot::HIST);
}

MCTRDSpectrumPlot::~MCTRDSpectrumPlot()
{
}

void MCTRDSpectrumPlot::processEvent(const AnalyzedEvent* event)
{
  if (event->simpleEvent()->contentType() != SimpleEvent::MonteCarlo)
    return;

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

  //get histo:
  int pdgID = event->simpleEvent()->MCInformation()->primary()->pdgID;

  TH1D* spectrumHisto = 0;

  if (m_spectrumMap.contains(pdgID)) {
    spectrumHisto = m_spectrumMap.value(pdgID);
  } else {
    const ParticleProperties* properties = ParticleDB::instance()->lookupPdgId(pdgID);
    QString particleName = properties->name();
    int nBins = TRDReconstruction::s_spectrumDefaultBins;
    QVector<double> binning = Helpers::logBinning(nBins, 1e-3, TRDReconstruction::spectrumUpperLimit());
    spectrumHisto = new TH1D(qPrintable(particleName + " " + title()), "", nBins, binning.constData());
    spectrumHisto->Sumw2();
    spectrumHisto->SetLineColor(RootStyle::rootColor(m_colorCounter++));
    m_spectrumMap.insert(pdgID, spectrumHisto);
    legend()->AddEntry(spectrumHisto, qPrintable(particleName), "l");
    addHistogram(spectrumHisto, H1DPlot::HIST);
  }

  for (QVector<double>::const_iterator it = valuesToFill.constBegin(); it != valuesToFill.constEnd(); ++it) {
    int iBin = spectrumHisto->FindBin(*it);
    double width = spectrumHisto->GetBinWidth(iBin);
    double weight = 1./width;
    spectrumHisto->Fill(*it, weight);
  }
}


void MCTRDSpectrumPlot::finalize()
{
  for (int i = 0; i < numberOfHistograms(); i++)
  {
    if (histogram(i)->Integral("width") > 0)
      histogram(i)->Scale(1./histogram(i)->Integral());
  }
}

void MCTRDSpectrumPlot::draw(TCanvas* canvas)
{
  canvas->cd();
  for (int i = numberOfHistograms()-1; i >= 0; i--)
  {
    TH1D* h = histogram(i);
    if (h == histogram(numberOfHistograms()-1))
      h->Draw();
    else
      h->Draw("SAME");
  }
  RootPlot::draw(canvas);
}
