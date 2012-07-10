#include "ExternalTrackerSignalHeight2DPlotNormalized.hh"

#include <TH2D.h>
#include <TAxis.h>
#include <TArrayD.h>

ExternalTrackerSignalHeight2DPlotNormalized::ExternalTrackerSignalHeight2DPlotNormalized(unsigned short moduleId)
  : ExternalTrackerSignalHeight2DPlot(moduleId)
{
  setTitle(QString("external tracker signal height 2d normalized 0x%1").arg(m_moduleId, 0, 16));
}

ExternalTrackerSignalHeight2DPlotNormalized::~ExternalTrackerSignalHeight2DPlotNormalized()
{

}

void ExternalTrackerSignalHeight2DPlotNormalized::update()
{
  TH2D* hist = m_histo;
  for (int xBin = 1; xBin <= hist->GetNbinsX(); xBin++) {
    for (int yBin = 1; yBin <= hist->GetNbinsY(); yBin++) {
      double bc = hist->GetBinContent(xBin, yBin);
      double norm = m_normHisto->GetBinContent(xBin);
      bc > 0 ? histogram()->SetBinContent(xBin, yBin, bc/norm) : histogram()->SetBinContent(xBin, yBin, 0.);
    }
  }
}
