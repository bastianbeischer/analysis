#include "ChannelTimeShiftHistogram.hh"
#include "PostAnalysisCanvas.hh"

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

ChannelTimeShiftHistogram::ChannelTimeShiftHistogram(PostAnalysisCanvas* canvas, int ch)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_line(0)
{
  TH2* histogram = canvas->histograms2D().at(0);
  histogram->Draw("COLZ");
  QString title = QString("%1 channel %2").arg(canvas->name()).arg(ch);
  setTitle(title);
  addHistogram(histogram->ProjectionY(qPrintable(title+"originalProjection"), ch+1, ch+1));
  TH1D* projection = histogram->ProjectionY(qPrintable(title+"modifiedProjection"), ch+1, ch+1);
  if (ch > 0) projection->Smooth(10);
  //if (ch > 0) projection->Rebin(2);
  projection->SetName(qPrintable(title));
  projection->SetLineColor(kRed);
  setAxisTitle("#Deltat / ns", "");
  TF1* function = new TF1(qPrintable(title + "Function"), "gaus", projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  projection->Fit(function, "QN0");
  if (ch > 0) {
    for (int i = 0; i < 5; ++i) {
      double mean = function->GetParameter(1);
      double sigma = qMax(0.5, function->GetParameter(2));
      function->SetRange(mean - sigma, mean + sigma);
      projection->Fit(function, "RQN0");
    }
  }
  function->SetRange(projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  function->SetNpx(1000);
  QStringList stringList = title.split(" ");
  int id = (stringList[ch < 4 ? 2 : 3].remove(0, 2).toInt(0, 16)) | (ch - (ch < 4 ? 0 : 4));
  std::cout << "0x" <<std::hex << id << "=" << -function->GetParameter(1) << std::endl;
  projection->GetXaxis()->SetRangeUser(-3, 3);
  addHistogram(projection);
  addFunction(function);
  TLatex* latex = 0;
  latex = RootPlot::newLatex(0.15, 0.85);
  latex->SetTextColor(kRed);
  latex->SetTitle(qPrintable(QString("mean  = %1%2 ns")
    .arg(projection->GetMean() < 0 ? '-' : ' ')
    .arg(qAbs(projection->GetMean()), 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.82);
  latex->SetTextColor(kRed);
  latex->SetTitle(qPrintable(QString("x_fit = %1%2 ns")
    .arg(function->GetParameter(1) < 0 ? '-' : ' ')
    .arg(qAbs(function->GetParameter(1)), 0, 'f', 3, ' ')));
  addLatex(latex);
  //m_line = new TLine(projection->GetMean(), 0, projection->GetMean(), 1.05 * projection->GetMaximum());
  m_line = new TLine(0.4, 0, 0.4, 1.05 * projection->GetMaximum());
  m_line->SetLineColor(kGreen);
  m_line->SetLineStyle(2);
  m_line->SetLineWidth(2);
}

ChannelTimeShiftHistogram::~ChannelTimeShiftHistogram()
{
  delete m_line;
}
  
void ChannelTimeShiftHistogram::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  m_line->Draw();
}
