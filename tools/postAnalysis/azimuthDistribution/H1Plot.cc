#include "H1Plot.hh"

#include "Corrections.hh"

#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>
#include <QSettings>
#include <QVector>

H1Plot::H1Plot(TH1D* histogram)
: PostAnalysisPlot()
, H1DPlot()
{
  setTitle(histogram->GetTitle());
  addHistogram(new TH1D(*histogram));
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle());
}

H1Plot::H1Plot(TH1D* histogram1, TH1D* histogram2)
: PostAnalysisPlot()
, H1DPlot()
{
  setTitle(histogram1->GetTitle());
  addHistogram(new TH1D(*histogram1));
  setAxisTitle(histogram1->GetXaxis()->GetTitle(), histogram1->GetYaxis()->GetTitle());
  
  addHistogram(new TH1D(*histogram2));
}

H1Plot::~H1Plot()
{

}
