#include "TRDSpectrumPlot.hh"

#include <TH1D.h>
#include <TMarker.h>
#include <TLatex.h>

#include "Particle.hh"
#include "Track.hh"
#include "ParticleInformation.hh"
#include "Cluster.hh"
#include "Hit.hh"
#include "SimpleEvent.hh"
#include "Settings.hh"
#include "SettingsManager.hh"
#include "TRDReconstruction.hh"
#include "Corrections.hh"

#include <math.h>

#include <QList>

TRDSpectrumPlot::TRDSpectrumPlot(unsigned short id, TRDSpectrumType spectrumType) :
  AnalysisPlot(AnalysisPlot::SignalHeightTRD),
  H1DPlot(),
  m_id(id),
  m_spectrumType(spectrumType),
  m_landauFitRange_lower(2./3./100. *TRDReconstruction::spectrumUpperLimit()),
  m_landauFitRange_upper(0.2 *TRDReconstruction::spectrumUpperLimit()),
  m_fitRangeMarker_lower(new TMarker(m_landauFitRange_lower, 0,2)),
  m_fitRangeMarker_upper(new TMarker(m_landauFitRange_upper, 0,2))
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

  if (m_spectrumType == TRDSpectrumPlot::completeTRD)
    setTitle(strType + QString(" spectrum"));
  else if (m_spectrumType == TRDSpectrumPlot::layer)
    setTitle(strType + QString(" spectrum %1").arg(m_id));
  else
    setTitle(strType + QString(" spectrum 0x%1").arg(m_id,0,16));

  //initialize fit function:
  m_landauFit = new TF1(qPrintable(title() + "LandauFit"),"landau", m_landauFitRange_lower, m_landauFitRange_upper);
  m_landauFit->SetLineColor(kRed);

  m_fitRangeMarker_lower->SetMarkerColor(kRed);
  m_fitRangeMarker_upper->SetMarkerColor(kRed);

  int nBins = TRDReconstruction::spectrumDefaultBins;
  double lowerBound = 1e-3;
  double upperBound = TRDReconstruction::spectrumUpperLimit();
  double delta = 1./nBins * (log(upperBound)/log(lowerBound) - 1);
  double p[nBins+1];
  for (int i = 0; i < nBins+1; i++) {
    p[i] = pow(lowerBound, delta*i+1);
  }

  TH1D* histogram = new TH1D(qPrintable(title()), "", nBins, p);
  histogram->Sumw2();
  setAxisTitle(TRDReconstruction::xAxisTitle(), "entries");
  addHistogram(histogram, H1DPlot::HIST);
  setDrawOption(H1DPlot::HIST);

  addLatex(RootPlot::newLatex(0.5, 0.8));
  addLatex(RootPlot::newLatex(0.55, 0.7));
}

TRDSpectrumPlot::~TRDSpectrumPlot()
{
  delete m_landauFit;
  delete m_fitRangeMarker_lower;
  delete m_fitRangeMarker_upper;
}

void TRDSpectrumPlot::processEvent(const QVector<Hit*>&, const Particle* const particle, const SimpleEvent* const)
{
  const TRDReconstruction* trdReconst = particle->trdReconstruction();
  if (!(trdReconst->flags() & TRDReconstruction::GoodTRDEvent))
    return;

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
    int iBin = histogram()->FindBin(*it);
    double width = histogram()->GetBinWidth(iBin);
    double weight = 1./width;
    histogram()->Fill(*it, weight);
  }
}

void TRDSpectrumPlot::finalize()
{
  if (histogram(0)->GetEntries() > 30) {
    histogram(0)->Fit(m_landauFit,"Q0","",m_landauFitRange_lower,m_landauFitRange_upper);

    m_fitRangeMarker_lower->SetX(m_landauFitRange_lower);
    m_fitRangeMarker_lower->SetY(m_landauFit->Eval(m_landauFitRange_lower));

    m_fitRangeMarker_upper->SetX(m_landauFitRange_upper);
    m_fitRangeMarker_upper->SetY(m_landauFit->Eval(m_landauFitRange_upper));
  }
}

void TRDSpectrumPlot::update()
{
  latex()->SetTitle(qPrintable(QString("entries = %1").arg(histogram()->GetEntries())));
  latex(1)->SetTitle(qPrintable(QString("uflow = %1, oflow = %2").arg(histogram()->GetBinContent(0)).arg(histogram()->GetBinContent(histogram()->GetNbinsX()))));
}

void TRDSpectrumPlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  m_landauFit->Draw("same");
  m_fitRangeMarker_lower->Draw("same");
  m_fitRangeMarker_upper->Draw("same");
}
