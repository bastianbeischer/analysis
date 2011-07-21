#include "MCTRDSpectrumPlot.hh"

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

MCTRDSpectrumPlot::MCTRDSpectrumPlot(unsigned short id, TRDSpectrumType spectrumType)
  : AnalysisPlot(AnalysisPlot::MonteCarloTRD)
  , H1DPlot()
  , m_colorCounter(0)
  , m_id(id)
  , m_spectrumType(spectrumType)
{
  QString strType;
  switch(m_spectrumType){
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

  if(m_spectrumType == MCTRDSpectrumPlot::completeTRD)
    setTitle(QString("MC spectra"));
  else
    setTitle(QString("MC spectra 0x%1").arg(m_id,0,16));

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

void MCTRDSpectrumPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const event)
{
  if (event->contentType() != SimpleEvent::MonteCarlo)
    return;

  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

  QList<double> valuesToFill;
  switch (m_spectrumType) {
  case TRDSpectrumPlot::completeTRD:
    if (TRDReconstruction::calculateLengthInTube)
      for (int i = 0; i < 8; ++i)
        valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForLayer(i);
    else
      for (int i = 0; i < 8; ++i)
        valuesToFill << trdReconst->energyDepositionOnTrackForLayer(i);
    break;
  case TRDSpectrumPlot::module:
    if (TRDReconstruction::calculateLengthInTube)
      valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForModule(m_id);
    else
      valuesToFill << trdReconst->energyDepositionOnTrackForModule(m_id);
    break;
  case TRDSpectrumPlot::channel:
    if (TRDReconstruction::calculateLengthInTube)
      valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForChannel(m_id);
    else
      valuesToFill << trdReconst->energyDepositionOnTrackForModule(m_id);
    break;
  case TRDSpectrumPlot::layer:
    if (TRDReconstruction::calculateLengthInTube)
        valuesToFill << trdReconst->energyDepositionOnTrackPerLengthForLayer(m_id);
    else
        valuesToFill << trdReconst->energyDepositionOnTrackForLayer(m_id);
    break;
  }

  //get histo:
  int pdgID = event->MCInformation()->primary()->pdgID;

  TH1D* spectrumHisto = 0;

  if (m_spectrumMap.contains(pdgID))
    spectrumHisto = m_spectrumMap.value(pdgID);
  else
  {
    const ParticleProperties* properties = ParticleDB::instance()->lookupPdgId(pdgID);
    QString particleName = properties->name();
    int nBins = TRDReconstruction::spectrumDefaultBins;
    double lowerBound = 1e-3;
    double upperBound = TRDReconstruction::spectrumUpperLimit();
    double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
    double p[nBins+1];
    for (int i = 0; i < nBins+1; i++) {
      p[i] = pow(lowerBound, delta*i+1);
    }
    spectrumHisto = new TH1D(qPrintable(particleName + " " + title()), "", nBins, p);
    spectrumHisto->Sumw2();
    spectrumHisto->SetLineColor(RootStyle::rootColor(m_colorCounter++));
    m_spectrumMap.insert(pdgID, spectrumHisto);
    legend()->AddEntry(spectrumHisto, qPrintable(particleName), "l");
    addHistogram(spectrumHisto, H1DPlot::HIST);
  }

  for (QList<double>::const_iterator it = valuesToFill.constBegin(); it != valuesToFill.constEnd(); ++it) {
    if (!(*it > 0.))
      continue;
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
