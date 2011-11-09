#include "SignalHeight2DNormalizedPlot.hh"

#include <TH2D.h>
#include <TAxis.h>
#include <TArrayD.h>

SignalHeight2DNormalizedPlot::SignalHeight2DNormalizedPlot()
  :  SignalHeight2DPlot()
{
  setTitle("signal height 2d normalized");
}

SignalHeight2DNormalizedPlot::~SignalHeight2DNormalizedPlot()
{
}

void SignalHeight2DNormalizedPlot::update()
{
  TH2D* hist = m_histo;
  for (int xBin = 1; xBin <= hist->GetNbinsX(); xBin++) {
    for (int yBin = 1; yBin <= hist->GetNbinsY(); yBin++) {
      double bc = hist->GetBinContent(xBin, yBin);
      double norm = m_normHisto->GetBinContent(xBin);
      bc > 10 ? histogram()->SetBinContent(xBin, yBin, bc/norm) : histogram()->SetBinContent(xBin, yBin, 0.);
    }
  }
}
