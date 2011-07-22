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
  , m_upperY(NAN)
  , m_lowerY(NAN)
  , m_mean(NAN)
  , m_meanError(NAN)
  , m_sigma(NAN)
  , m_sigmaError(NAN)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  histogram->Draw("COLZ");
  //  u-177.75 l19.75
  QString axisLabel(histogram->GetXaxis()->GetBinLabel(bin));
  m_upperY = axisLabel.section(' ', 0, 0).remove(0, 1).toDouble();
  m_lowerY = axisLabel.section(' ', 1, 1).remove(0, 1).toDouble();
  QString title = QString("%1 upper=%2mm lower=%3mm").arg(canvas->name().remove(" canvas")).arg(m_upperY).arg(m_lowerY);
  setTitle(title);
  TH1D* projection = histogram->ProjectionY("tmp", bin, bin);
  //projection->Smooth();
  int nBins = projection->GetXaxis()->GetNbins();
  int entries = 0;
  for (int bin = 1; bin <= nBins; ++bin)
    entries+= projection->GetBinContent(bin);
  projection->SetName(qPrintable(title));
  setAxisTitle("#Deltat / ns", "");
  TF1* function = new TF1(qPrintable(title + "Function"), "gaus", projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  if (entries < 10)
    return;
    function->SetParameters(projection->GetMaximum(), 2.6, 0.4);
    function->SetParLimits(1, 2.2, 3.3);
    function->SetParLimits(2, 0.2, 4.0);
    projection->Fit(function, "QN0");
    for (int i = 0; i < 5; ++i) {
      double mean = function->GetParameter(1);
      double sigma = function->GetParameter(2);
      function->SetRange(mean - 1.5 * sigma, mean + 1.5 * sigma);
      projection->Fit(function, "RQN0");
    }

  m_mean = function->GetParameter(1);
  m_meanError = function->GetParError(1);
  m_sigma = function->GetParameter(2);
  m_sigmaError = function->GetParError(2);
  function->SetRange(projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  QStringList stringList = title.split(" ");
  addHistogram(projection);
  addFunction(function);

  TLatex* latex = 0;

  latex = RootPlot::newLatex(0.15, 0.85);
  latex->SetTitle(qPrintable(QString("n     = %1").arg(entries)));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.82);
  latex->SetTitle(qPrintable(QString("uflow = %1").arg(projection->GetBinContent(0))));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.79);
  latex->SetTitle(qPrintable(QString("oflow = %1").arg(projection->GetBinContent(nBins + 1))));
  addLatex(latex);

  latex = RootPlot::newLatex(0.62, 0.85);
  latex->SetTitle(qPrintable(QString("chi2  = %1").arg(function->GetChisquare(), 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.62, 0.82);
  latex->SetTitle(qPrintable(QString("mean  = %1 #pm %2 ns").arg(m_mean, 0, 'f', 3, ' ').arg(m_meanError, 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.62, 0.79);
  latex->SetTitle(qPrintable(QString("sigma = %1 #pm %2 ns").arg(m_sigma, 0, 'f', 3, ' ').arg(m_sigmaError, 0, 'f', 3, ' ')));
  addLatex(latex);
}

TimeOfFlightHistogram::~TimeOfFlightHistogram()
{}

double TimeOfFlightHistogram::upperY()
{
  return m_upperY;
}

double TimeOfFlightHistogram::lowerY()
{
  return m_lowerY;
}

double TimeOfFlightHistogram::mean()
{
  return m_mean;
}

double TimeOfFlightHistogram::meanError()
{
  return m_meanError;
}

double TimeOfFlightHistogram::sigma()
{
  return m_sigma;
}

double TimeOfFlightHistogram::sigmaError()
{
  return m_sigmaError;
}
