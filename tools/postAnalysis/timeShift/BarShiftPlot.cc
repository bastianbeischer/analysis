#include "BarShiftPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "TimeShiftContainer.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>
#include <TLine.h>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <QDebug>
#include <QStringList>

BarShiftPlot::BarShiftPlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_dt(NAN)
  , m_errDt(NAN)
  , m_lines()
{
  QString title = canvas->name().remove("canvas") + "plot";
  setTitle(title);

  TH1D* histogram = canvas->histograms1D().at(0);
  TH1D* originalHistogram = new TH1D(qPrintable(title + "original"), "", histogram->GetXaxis()->GetNbins(), histogram->GetXaxis()->GetXmin(), histogram->GetXaxis()->GetXmax());
  TH1D* modifiedHistogram = new TH1D(qPrintable(title + "modified"), "", histogram->GetXaxis()->GetNbins(), histogram->GetXaxis()->GetXmin(), histogram->GetXaxis()->GetXmax());
  originalHistogram->Add(histogram);
  modifiedHistogram->Add(histogram);
  setAxisTitle("#Deltat / ns", "");
  addHistogram(originalHistogram);
  addHistogram(modifiedHistogram);

  if (modifiedHistogram->GetEntries() > 200)
    modifiedHistogram->Smooth();
  modifiedHistogram->SetLineColor(kRed);

  TF1* function = new TF1(qPrintable(title + "Function"), "gaus", modifiedHistogram->GetXaxis()->GetXmin(), modifiedHistogram->GetXaxis()->GetXmax());
  function->SetNpx(1000);
  function->SetParameters(modifiedHistogram->GetMaximum(), modifiedHistogram->GetMean(), modifiedHistogram->GetRMS());
  function->FixParameter(1, modifiedHistogram->GetMean());
  if (modifiedHistogram->GetEntries() > 200) {
    modifiedHistogram->Fit(function, "QN0");
    function->ReleaseParameter(1);
    for (int i = 0; i < 5; ++i) {
      double mean = function->GetParameter(1);
      double sigma = qMax(0.2, function->GetParameter(2));
      function->SetRange(mean - sigma, mean + sigma);
      modifiedHistogram->Fit(function, "RQN0");
    }
    m_dt = function->GetParameter(1);
    m_errDt = function->GetParError(1);
  }
  function->SetRange(modifiedHistogram->GetXaxis()->GetXmin(), modifiedHistogram->GetXaxis()->GetXmax());
  addFunction(function);

  TLatex* latex = 0;
  latex = RootPlot::newLatex(0.15, 0.85);
  latex->SetTextColor(kRed);
  latex->SetTitle(qPrintable(QString("mean  = %1%2 ns")
    .arg(modifiedHistogram->GetMean() < 0 ? '-' : ' ')
    .arg(qAbs(modifiedHistogram->GetMean()), 0, 'f', 3, ' ')));
  addLatex(latex);
  if (!std::isnan(m_dt)) {
    latex = RootPlot::newLatex(0.15, 0.82);
    latex->SetTextColor(kRed);
    latex->SetTitle(qPrintable(QString("x_fit = %1%2 ns").arg(m_dt < 0 ? '-' : ' ').arg(qAbs(m_dt), 0, 'f', 3, ' ')));
    addLatex(latex);
  }
  TLine* line = 0;
  double x = 0;
  double max = 1.05 * originalHistogram->GetMaximum();

  x = modifiedHistogram->GetMean();
  line = new TLine(x, 0, x, max);
  line->SetLineColor(kRed);
  line->SetLineStyle(2);
  line->SetLineWidth(2);
  m_lines.append(line);

  x = (Constants::upperTofPosition - Constants::lowerTofPosition) / Constants::speedOfLight;
  line = new TLine(x, 0, x, max);
  line->SetLineColor(kGreen);
  line->SetLineStyle(2);
  line->SetLineWidth(2);
  m_lines.append(line);

}

BarShiftPlot::~BarShiftPlot()
{
  qDeleteAll(m_lines);
}

void BarShiftPlot::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  foreach (TLine* line, m_lines)
    line->Draw();
}
