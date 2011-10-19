#include "SmearedRigidityComparissonGraph.hh"

#include "Helpers.hh"

#include <TH1D.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>

#include <QString>
#include <QDebug>

SmearedRigidityComparissonGraph::SmearedRigidityComparissonGraph(Enums::ChargeSign type, TH1D* generatedSpectrum, TH1D* reconstructedSpectrum, TH1D* reconstructedSpectrumLow, TH1D* reconstructedSpectrumUp)
: PostAnalysisPlot()
, H1DPlot()
, m_type(type)
, m_generatedSpectrum(generatedSpectrum)
, m_reconstructedSpectrum(reconstructedSpectrum)
, m_sysbelt(0)
{
  QString name = QString("SmearedRigidityComparissonGraph ") + Enums::label(type);
  setTitle(name);
  if (m_type == Enums::Negative) {
    m_generatedSpectrum = Helpers::createMirroredHistogram(generatedSpectrum);
    m_reconstructedSpectrum = Helpers::createMirroredHistogram(reconstructedSpectrum);
    m_reconstructedSpectrumLow = Helpers::createMirroredHistogram(reconstructedSpectrumLow);
    m_reconstructedSpectrumUp = Helpers::createMirroredHistogram(reconstructedSpectrumUp);
  } else {
    m_generatedSpectrum = new TH1D(*generatedSpectrum);
    m_reconstructedSpectrum = new TH1D(*reconstructedSpectrum);
    m_reconstructedSpectrumLow = new TH1D(*reconstructedSpectrumLow);
    m_reconstructedSpectrumUp = new TH1D(*reconstructedSpectrumUp);
  }
  Helpers::scaleToBinWidth(m_reconstructedSpectrum);
  Helpers::scaleToBinWidth(m_reconstructedSpectrumLow);
  Helpers::scaleToBinWidth(m_reconstructedSpectrumUp);
  m_sysbelt = Helpers::createBeltWithSystematicUncertainty(m_reconstructedSpectrum,m_reconstructedSpectrumLow,m_reconstructedSpectrumUp);
  m_sysbelt->SetTitle("rec. #pm 25% #sigma_{p}");
  m_sysbelt->SetLineColor(kBlue);
  m_sysbelt->SetFillColor(kBlue);
  m_sysbelt->SetFillStyle(3002);

  addHistogram(m_reconstructedSpectrum, H1DPlot::P);
  setAxisTitle(m_reconstructedSpectrum->GetXaxis()->GetTitle(), "entries / GV");
  histogram()->GetXaxis()->SetMoreLogLabels(true);
  const int nBins = histogram()->GetNbinsX();
  int nBinsNew = 0;
  int nBinsStart = 0;
  if (nBins%2 == 0)
    nBinsNew = nBins / 2;
  else
    nBinsNew = (nBins - 1) / 2;

  if (nBins%2 == 0)
    nBinsStart = nBinsNew + 1;
  else
    nBinsStart = nBinsNew + 2;
  histogram()->GetXaxis()->SetRange(nBinsStart, nBins);
  Helpers::scaleToBinWidth(m_generatedSpectrum);
  addHistogram(m_generatedSpectrum, H1DPlot::HIST);

  int low = histogram()->GetXaxis()->FindBin(0.2);
  int up= histogram()->GetXaxis()->FindBin(15);
  histogram()->GetXaxis()->SetRange(low, up);

  TLegend* legend = new TLegend(0.71625,0.7666667,0.91875,0.93);
  legend->SetFillColor(kWhite);
  legend->AddEntry(m_generatedSpectrum, "generated", "l");
  legend->AddEntry(m_reconstructedSpectrum, "reconstructed", "P");
  legend->AddEntry(m_sysbelt, "rec. #pm 25% #sigma_{p}", "F");
  addLegend(legend);
}

SmearedRigidityComparissonGraph::~SmearedRigidityComparissonGraph()
{
}

void SmearedRigidityComparissonGraph::draw(TCanvas* canvas) {
  H1DPlot::draw(canvas);
  m_sysbelt->Draw("3SAME");
}
