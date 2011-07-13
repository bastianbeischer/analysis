#include "TimeOfFlightHistogram.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>

#include <cmath>
#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

TH2D* TimeOfFlightHistogram::histgram(TCanvas* canvas)
{
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i)
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      return static_cast<TH2D*>(canvas->GetListOfPrimitives()->At(i));
  return 0;
}

TimeOfFlightHistogram::TimeOfFlightHistogram(PostAnalysisCanvas* canvas, int bin)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_y(NAN)
  , m_sigma(NAN)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  histogram->Draw("COLZ");
  m_y = histogram->GetXaxis()->GetBinCenter(bin);
  QString title = QString("%1 y=%2mm").arg(canvas->name().remove(" canvas")).arg(m_y);
  setTitle(title);
  TH1D* projection = histogram->ProjectionY("tmp", bin, bin);
  //projection->Smooth();
  projection->SetName(qPrintable(title));
  setAxisTitle("#Deltat / ns", "");
  TF1* function = new TF1(qPrintable(title + "Function"), "gaus", projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  projection->Fit(function, "QN0");
  function->SetParLimits(1, 2.2, 3.3);
  for (int i = 0; i < 5; ++i) {
    double mean = function->GetParameter(1);
    double sigma = qMax(0.2, function->GetParameter(2));
    function->SetRange(mean - 1.5 * sigma, mean + 1.5 * sigma);
    projection->Fit(function, "RQN0");
  }
  m_sigma = function->GetParameter(2);
  function->SetRange(projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  QStringList stringList = title.split(" ");
  addHistogram(projection);
  addFunction(function);
  TLatex* latex = 0;
  latex = RootPlot::newLatex(0.15, 0.85);
  latex->SetTitle(qPrintable(QString("n     = %1").arg(projection->GetEntries())));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.82);
  latex->SetTitle(qPrintable(QString("uflow = %1").arg(projection->GetBinContent(0))));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.79);
  latex->SetTitle(qPrintable(QString("oflow = %1").arg(projection->GetBinContent(projection->GetXaxis()->GetNbins()+1))));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.76);
  latex->SetTitle(qPrintable(QString("mean  = %1 ns").arg(function->GetParameter(1), 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.73);
  latex->SetTitle(qPrintable(QString("#sigma     = %1 ns").arg(function->GetParameter(2), 0, 'f', 3, ' ')));
  addLatex(latex);
}

TimeOfFlightHistogram::~TimeOfFlightHistogram()
{}

double TimeOfFlightHistogram::y()
{
  return m_y;
}

double TimeOfFlightHistogram::sigma()
{
  return m_sigma;
}
