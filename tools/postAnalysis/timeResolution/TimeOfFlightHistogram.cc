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

TimeOfFlightHistogram::TimeOfFlightHistogram(PostAnalysisCanvas* canvas, int bin)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_upperId(-1)
  , m_lowerId(-1)
  , m_upperY(NAN)
  , m_lowerY(NAN)
  , m_mean(NAN)
  , m_meanError(NAN)
  , m_sigma(NAN)
  , m_sigmaError(NAN)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  histogram->Draw("COLZ");
  QString axisLabel(histogram->GetXaxis()->GetBinLabel(bin));
  m_upperY = axisLabel.section(' ', 0, 0).remove(0, 1).toDouble();
  m_lowerY = axisLabel.section(' ', 1, 1).remove(0, 1).toDouble();
  QString title = canvas->name().remove("time resolution ").remove(" canvas");
  m_upperId = title.mid(2, 4).toInt(0, 16);
  m_lowerId = title.mid(17, 4).toInt(0, 16);
  title.append(QString(" upper=%1mm lower=%2mm").arg(m_upperY).arg(m_lowerY));
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
  function->SetParameters(projection->GetMaximum(), 2.6, 0.4);
  function->SetParError(1, 5.);
  function->SetParError(2, 5.);
  bool fitted = false;
  if (entries > 100) {
    function->SetParameters(projection->GetMaximum(), 2.6, 0.4);
    function->SetParError(1, .2);
    function->SetParError(2, .2);
    function->SetParLimits(1, 2.2, 3.3);
    function->SetParLimits(2, 0.2, 4.0);
    projection->Fit(function, "QN0");
    for (int i = 0; i < 5; ++i) {
      double mean = function->GetParameter(1);
      double sigma = function->GetParameter(2);
      function->SetRange(mean - 1.5 * sigma, mean + 1.5 * sigma);
      projection->Fit(function, "RQN0");
    }
    fitted = true;
  }

  m_mean = function->GetParameter(1);
  m_meanError = function->GetParError(1);
  m_sigma = function->GetParameter(2);
  m_sigmaError = function->GetParError(2);
  TF1* functionR = new TF1(qPrintable(title + "FunctionR"), "gaus", function->GetXmin(), function->GetXmax());
  function->SetRange(projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  QStringList stringList = title.split(" ");
  addHistogram(projection);
  function->SetNpx(1000);
  addFunction(function);
  functionR->SetParameters(function->GetParameter(0), function->GetParameter(1), function->GetParameter(2));
  functionR->SetLineColor(kRed);
  functionR->SetNpx(1000);
  if (fitted)
    addFunction(functionR);

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

  latex = RootPlot::newLatex(0.60, 0.85);
  latex->SetTitle(qPrintable(QString("chi2  = %1").arg(function->GetChisquare(), 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.60, 0.82);
  latex->SetTitle(qPrintable(QString("mean  = %1 #pm %2 ns").arg(m_mean, 0, 'f', 3, ' ').arg(m_meanError, 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.60, 0.79);
  latex->SetTitle(qPrintable(QString("sigma = %1 #pm %2 ns").arg(m_sigma, 0, 'f', 3, ' ').arg(m_sigmaError, 0, 'f', 3, ' ')));
  addLatex(latex);
}

TimeOfFlightHistogram::~TimeOfFlightHistogram()
{}

int TimeOfFlightHistogram::upperId() const
{
  return m_upperId;
}

int TimeOfFlightHistogram::lowerId() const
{
  return m_lowerId;
}

double TimeOfFlightHistogram::upperY() const
{
  return m_upperY;
}

double TimeOfFlightHistogram::lowerY() const
{
  return m_lowerY;
}

double TimeOfFlightHistogram::mean() const
{
  return m_mean;
}

double TimeOfFlightHistogram::meanError() const
{
  return m_meanError;
}

double TimeOfFlightHistogram::sigma() const
{
  return m_sigma;
}

double TimeOfFlightHistogram::sigmaError() const
{
  return m_sigmaError;
}
