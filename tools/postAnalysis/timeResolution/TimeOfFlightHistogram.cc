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

TimeOfFlightHistogram::TimeOfFlightHistogram(PostAnalysisCanvas* canvas, int bin)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_upperId(-1)
  , m_lowerId(-1)
  , m_i(-1)
  , m_j(-1)
  , m_k(-1)
  , m_l(-1)
  , m_upperY(NAN)
  , m_lowerY(NAN)
  , m_mean(NAN)
  , m_meanError(NAN)
  , m_sigma(NAN)
  , m_sigmaError(NAN)
  , m_v(NAN)
  , m_vError(NAN)
  , m_fitted(false)
  , m_fitGood(false)
{ 
  TH2D* histogram = canvas->histograms2D().at(0);
  histogram->Draw("COLZ");
  extractIndizes(canvas->name(), histogram, bin);
  TH1D* projection = histogram->ProjectionY("tmp", bin, bin);
  addHistogram(projection);
  //projection->Smooth();
  int nBins = projection->GetXaxis()->GetNbins();
  int entries = 0;
  for (int bin = 1; bin <= nBins; ++bin)
    entries+= projection->GetBinContent(bin);
  projection->SetName(qPrintable(title()));
  TF1* function = new TF1(qPrintable(title() + "Function"), "gaus", projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  addFunction(function);
  function->SetParameters(projection->GetMaximum(), 2.6, 0.4);
  if (entries > 10) {
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
    m_fitted = true;
  }

  extractParameters();

  setAxisTitle("#Deltat / ns", "");

  if (m_fitGood) {
    TF1* functionR = new TF1(qPrintable(title() + "FunctionR"), "gaus", function->GetXmin(), function->GetXmax());
    functionR->SetParameters(function->GetParameter(0), function->GetParameter(1), function->GetParameter(2));
    functionR->SetLineColor(kRed);
    functionR->SetNpx(1000);
    addFunction(functionR);
  }

  function->SetNpx(1000);
  function->SetRange(projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());

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

  latex = RootPlot::newLatex(0.50, 0.95);
  latex->SetTextAlign(23);
  latex->SetTitle(qPrintable(QString("(%1 %2 %3 %4)").arg(m_i).arg(m_j).arg(m_k).arg(m_l)));
  addLatex(latex);

  latex = RootPlot::newLatex(0.55, 0.85);
  latex->SetTitle(qPrintable(QString("chi2 / ndf = %1 / %2").arg(function->GetChisquare(), 0, 'f', 3, ' ').arg(function->GetNDF())));
  addLatex(latex);
  latex = RootPlot::newLatex(0.55, 0.82);
  latex->SetTitle(qPrintable(QString("mean       = %1 #pm %2 ns").arg(m_mean, 0, 'f', 3, ' ').arg(m_meanError, 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.55, 0.79);
  latex->SetTitle(qPrintable(QString("sigma      = %1 #pm %2 ns").arg(m_sigma, 0, 'f', 3, ' ').arg(m_sigmaError, 0, 'f', 3, ' ')));
  addLatex(latex);
}

TimeOfFlightHistogram::~TimeOfFlightHistogram()
{}

void TimeOfFlightHistogram::extractIndizes(const QString& canvasName, const TH2D* const histogram, int bin)
{
  QString axisLabel = histogram->GetXaxis()->GetBinLabel(bin);
  m_upperY = axisLabel.section(' ', 0, 0).remove(0, 1).toDouble();
  m_lowerY = axisLabel.section(' ', 1, 1).remove(0, 1).toDouble();
  QString title = canvasName;
  title.remove("time resolution ").remove(" canvas");
  m_upperId = title.mid(2, 4).toInt(0, 16);
  m_lowerId = title.mid(17, 4).toInt(0, 16);
  title.append(QString(" upper=%1mm lower=%2mm").arg(m_upperY).arg(m_lowerY));
  setTitle(title);

  if (m_upperId == 0x8000) m_i = 0;
  else if (m_upperId == 0x8004) m_i = 1;
  else if (m_upperId == 0x8008) m_i = 2;
  else if (m_upperId == 0x800c) m_i = 3;

  if (m_lowerId == 0x8020) m_j = 0;
  else if (m_lowerId == 0x8024) m_j = 1;
  else if (m_lowerId == 0x8028) m_j = 2;
  else if (m_lowerId == 0x802c) m_j = 3;

  int nBarPositions = sqrt(histogram->GetXaxis()->GetNbins());
  m_k = (bin - 1) / nBarPositions;
  m_l = (bin - 1) % nBarPositions;
}

void TimeOfFlightHistogram::extractParameters()
{
  m_fitGood = true;
  if (!m_fitted)
    m_fitGood = false;
  if (function()->GetChisquare() / function()->GetNDF() > 5.)
    m_fitGood = false;
  if (function()->GetParError(2) > 1.)
    m_fitGood = false;

  m_mean = m_fitGood ? function()->GetParameter(1) : 0.;
  m_meanError = m_fitGood ? function()->GetParError(1) : 1000.;
  m_sigma = m_fitGood ? function()->GetParameter(2) : 1000.;
  m_sigmaError = m_fitGood ? function()->GetParError(2) : 1000.;
  m_v = m_sigma * m_sigma;
  m_vError = 2 * m_sigma * m_sigmaError;
}

int TimeOfFlightHistogram::upperId() const
{
  return m_upperId;
}

int TimeOfFlightHistogram::lowerId() const
{
  return m_lowerId;
}

int TimeOfFlightHistogram::i() const
{
  return m_i;
}

int TimeOfFlightHistogram::j() const
{
  return m_j;
}

int TimeOfFlightHistogram::k() const
{
  return m_k;
}

int TimeOfFlightHistogram::l() const
{
  return m_l;
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

double TimeOfFlightHistogram::v() const
{
    return m_v;
}

double TimeOfFlightHistogram::vError() const
{
    return m_vError;
}

bool TimeOfFlightHistogram::fitted() const
{
  return m_fitted;
}
