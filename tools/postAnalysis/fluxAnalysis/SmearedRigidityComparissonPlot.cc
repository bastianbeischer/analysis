#include "SmearedRigidityComparissonPlot.hh"

#include "Helpers.hh"

#include <TH1D.h>

#include <QString>
#include <QDebug>

SmearedRigidityComparissonPlot::SmearedRigidityComparissonPlot(Enums::ChargeSign type, TH1D* generatedSpectrum, TH1D* reconstructedSpectrum, TH1D* unfoldedSpectrum)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_type(type)
  , m_generatedSpectrum(generatedSpectrum)
  , m_reconstructedSpectrum(reconstructedSpectrum)
  , m_unfoldedSpectrum(unfoldedSpectrum)
{
  QString name = QString("SmearedRigidityComparissonPlot ") + Enums::label(type);
  if (m_unfoldedSpectrum)
   name.append(" unfolded");
  setTitle(name);
  if (m_type == Enums::Negative) {
    m_generatedSpectrum = Helpers::createMirroredHistogram(generatedSpectrum);
    m_reconstructedSpectrum = Helpers::createMirroredHistogram(reconstructedSpectrum);
    if (m_unfoldedSpectrum)
      m_unfoldedSpectrum= Helpers::createMirroredHistogram(unfoldedSpectrum);
  } else {
    m_generatedSpectrum = new TH1D(*generatedSpectrum);
    m_reconstructedSpectrum = new TH1D(*reconstructedSpectrum);
    if (m_unfoldedSpectrum)
      m_unfoldedSpectrum = new TH1D(*unfoldedSpectrum);
  }
  Helpers::scaleToBinWidth(m_reconstructedSpectrum);
  addHistogram(m_reconstructedSpectrum, H1DPlot::P);
  setAxisTitle(m_reconstructedSpectrum->GetXaxis()->GetTitle(), "entries / GeV");
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
  if (m_unfoldedSpectrum) {
    m_unfoldedSpectrum->SetLineColor(kBlack);
    m_unfoldedSpectrum->SetMarkerColor(kBlack);
    Helpers::scaleToBinWidth(m_unfoldedSpectrum);
    addHistogram(m_unfoldedSpectrum, H1DPlot::P);
  }
}

SmearedRigidityComparissonPlot::~SmearedRigidityComparissonPlot()
{
}

