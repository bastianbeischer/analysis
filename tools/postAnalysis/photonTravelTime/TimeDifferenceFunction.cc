#include "TimeDifferenceFunction.hh"
#include "Constants.hh"

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

double photonTime(double* x, double* p) {
  double a = x[0] * 2. / Constants::tofBarLength;
  double b = x[1] * 2. / Constants::tofBarWidth;
  double s = (x[0] < 0) ? 1 : -1;
  return p[0] + p[1] * a + p[2] * s * pow(qAbs(a), 6) * (1 - cos(3.14*b));
}

TimeDifferenceFunction::TimeDifferenceFunction(TCanvas* canvas)
  : PostAnalysisPlot()
  , H2DPlot()
  , m_function(0)
{
  TH2* histogram = 0;
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      histogram = static_cast<TH2*>(canvas->GetListOfPrimitives()->At(i));
  }
  QString title;
  title = QString("%1 function").arg(canvas->GetName());
  setTitle(title);
  double minX = histogram->GetXaxis()->GetXmin();
  double maxX = histogram->GetXaxis()->GetXmax();
  double minY = histogram->GetYaxis()->GetXmin();
  double maxY = histogram->GetYaxis()->GetXmax();
  m_function = new TF2(qPrintable(title), photonTime, minX, maxX, minY, maxY, 3);
  m_function->SetParameters(0, 3, 2);
  histogram->Fit(m_function, "QN0");
  title = QString("%1 histogram").arg(canvas->GetName());
  setTitle(title);
  int nBinsX = histogram->GetXaxis()->GetNbins();
  int nBinsY = histogram->GetYaxis()->GetNbins();
  TH2D* h = new TH2D(qPrintable(title), "", nBinsX, minX, maxX, nBinsY, minY, maxY);
  for (int binX = 1; binX <= nBinsX; ++binX) {
    for (int binY = 1; binY <= nBinsY; ++binY) {
      double x = h->GetXaxis()->GetBinCenter(binX);
      double y = h->GetYaxis()->GetBinCenter(binY);
      h->SetBinContent(binX, binY, m_function->Eval(x, y));
    }
  }
  setHistogram(h);
}

TimeDifferenceFunction::~TimeDifferenceFunction()
{}

TF2* TimeDifferenceFunction::function()
{
  return m_function;
}
