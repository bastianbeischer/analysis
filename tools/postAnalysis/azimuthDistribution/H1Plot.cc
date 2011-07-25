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

H1Plot::H1Plot(int nParams, ...)
: PostAnalysisPlot()
, H1DPlot()
{
  va_list params;
  TH1D* par;
  va_start(params, nParams);

  for (int i = 0; i < nParams; ++i)
  {
    par = va_arg(params, TH1D*);
    if (i == 0) {
      setTitle(par->GetTitle());
      setAxisTitle(par->GetXaxis()->GetTitle(), par->GetYaxis()->GetTitle());
    }
    addHistogram(new TH1D(*par));
  }
  va_end(params);
}

H1Plot::~H1Plot()
{

}
