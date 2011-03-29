#include "TimeDifferenceFunction.hh"
#include "PostAnalysisCanvas.hh"
#include "Constants.hh"
#include "Corrections.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF2.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>

double photonTime(double x[], double p[])
{
  return Corrections::photonTravelTimeDifference(x[1], x[0], p);
}

TimeDifferenceFunction::TimeDifferenceFunction(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H2DPlot()
  , m_function(0)
{
  TH2* histogram = canvas->histograms2D().at(0);
  QString title;
  title = QString("%1 function").arg(canvas->name());
  setTitle(title);
  double minX = histogram->GetXaxis()->GetXmin();
  double maxX = histogram->GetXaxis()->GetXmax();
  double minY = histogram->GetYaxis()->GetXmin();
  double maxY = histogram->GetYaxis()->GetXmax();
  m_function = new TF2(qPrintable(title), photonTime, minX, maxX, minY, maxY, Corrections::nPhotonTravelTimeDifferenceParameters);
  m_function->SetParameters(0, 1.5, 2.5, 2.5, 2.5, 2.5);
  histogram->Fit(m_function, "QN0 WW");
  for (int i = 0; i < Corrections::nPhotonTravelTimeDifferenceParameters; ++i)
    qDebug() << QString("%1_c%2=%3").arg(title).arg(i).arg(m_function->GetParameter(i));
  title = QString("%1 histogram").arg(canvas->name());
  setTitle(title);
  int nBinsX = histogram->GetXaxis()->GetNbins();
  int nBinsY = histogram->GetYaxis()->GetNbins();
  TH2D* h = new TH2D(qPrintable(title), "", nBinsX, minX, maxX, nBinsY, minY, maxY);
  for (int binX = 1; binX <= nBinsX; ++binX) {
    for (int binY = 1; binY <= nBinsY; ++binY) {
      double bending = h->GetYaxis()->GetBinCenter(binY);
      double nonBending = h->GetXaxis()->GetBinCenter(binX);
      h->SetBinContent(binX, binY, m_function->Eval(nonBending, bending));
    }
  }
  setHistogram(h);
}

TimeDifferenceFunction::~TimeDifferenceFunction()
{}

void TimeDifferenceFunction::draw(TCanvas* canvas)
{
  H2DPlot::draw(canvas);
  //histogram()->GetZaxis()->SetRangeUser(-5, 5);
}
TF2* TimeDifferenceFunction::function()
{
  return m_function;
}
