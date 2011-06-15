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

#include <QDebug>
#include <QStringList>

BarShiftPlot::BarShiftPlot(PostAnalysisCanvas* canvas)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_lines()
{
  QString title = canvas->name().remove("canvas") + "plot";
  setTitle(title);

  TH1D* originalHistogram = new TH1D(*canvas->histograms1D().at(0));
  originalHistogram->GetXaxis()->SetRangeUser(-4, 4);
  originalHistogram->Draw();
  setAxisTitle("#Deltat / ns", "");
  addHistogram(originalHistogram);
  
  TH1D* modifiedHistogram = new TH1D(*originalHistogram);
  if (modifiedHistogram->GetEntries() > 100)
    modifiedHistogram->Smooth();
  modifiedHistogram->SetLineColor(kRed);

  TF1* function = new TF1(qPrintable(title + "Function"), "gaus", modifiedHistogram->GetXaxis()->GetXmin(), modifiedHistogram->GetXaxis()->GetXmax());
  function->SetNpx(1000);
  function->SetParameters(modifiedHistogram->GetMaximum(), modifiedHistogram->GetMean(), modifiedHistogram->GetRMS());
  function->FixParameter(1, modifiedHistogram->GetMean());
  if (modifiedHistogram->GetEntries() > 100) {
    modifiedHistogram->Fit(function, "QN0");
    function->ReleaseParameter(1);
    for (int i = 0; i < 5; ++i) {
      double mean = function->GetParameter(1);
      double sigma = qMax(0.5, function->GetParameter(2));
      function->SetRange(mean - sigma, mean + sigma);
      modifiedHistogram->Fit(function, "RQN0");
    }
  }
  function->SetRange(modifiedHistogram->GetXaxis()->GetXmin(), modifiedHistogram->GetXaxis()->GetXmax());

  //QStringList stringList = title.split(" ");
  //int id = (stringList[ch < 4 ? 2 : 3].remove(0, 2).toInt(0, 16)) | (ch - (ch < 4 ? 0 : 4));
  //TimeShiftContainer::instance()->setData(id, refCh, -function->GetParameter(1));

  addHistogram(modifiedHistogram);
  addFunction(function);

  
  TLatex* latex = 0;
  latex = RootPlot::newLatex(0.15, 0.85);
  latex->SetTextColor(kRed);
  latex->SetTitle(qPrintable(QString("mean  = %1%2 ns")
    .arg(modifiedHistogram->GetMean() < 0 ? '-' : ' ')
    .arg(qAbs(modifiedHistogram->GetMean()), 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.82);
  latex->SetTextColor(kRed);
  latex->SetTitle(qPrintable(QString("x_fit = %1%2 ns")
    .arg(function->GetParameter(1) < 0 ? '-' : ' ')
    .arg(qAbs(function->GetParameter(1)), 0, 'f', 3, ' ')));
  addLatex(latex);
  TLine* line = 0;
  line = new TLine(modifiedHistogram->GetMean(), 0, modifiedHistogram->GetMean(), 1.05 * qMax(modifiedHistogram->GetMaximum(), modifiedHistogram->GetMaximum()));
  line->SetLineColor(kRed);
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

void BarShiftPlot::dumpMatrix(const TMatrixT<double>& m)
{
  int width = 10;
  int space = 2;
  std::cout << " _";
  for (int i = 0; i < (width + space) * m.GetNcols() - 2; ++i) std::cout << ' ';
  std::cout << "_ " << std::endl;
  for (int r = 0; r < m.GetNrows(); ++r) {
    for (int c = 0; c < m.GetNcols(); ++c) {
      if (c) {
        for (int i = 0; i < space; ++i) std::cout << ' ';
      } else {
        std::cout << (r == m.GetNrows() - 1 ? "|_" : "| ");
      }
      std::cout << std::setw(width) << std::showpos << std::fixed << m[r][c];
    }
    std::cout << (r == m.GetNrows() - 1 ? "_|" : " |") << std::endl;
  }
}
