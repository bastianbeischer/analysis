#include "H2Plot.hh"

#include "Corrections.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH1D.h>
#include <TH2D.h>
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

H2Plot::H2Plot(TH2D* histogram)
  : PostAnalysisPlot()
  , H2DPlot()
{
  setTitle(histogram->GetTitle());
  addHistogram(new TH2D(*histogram));
  setAxisTitle(histogram->GetXaxis()->GetTitle(), histogram->GetYaxis()->GetTitle(), histogram->GetZaxis()->GetTitle());
}

H2Plot::~H2Plot()
{

}
