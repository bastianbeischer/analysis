#include "ChannelTimeShiftHistogram.hh"
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

ChannelTimeShiftHistogram::ChannelTimeShiftHistogram(PostAnalysisCanvas* canvas, int ch)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_lines()
{
  TH2D* histogram = canvas->histograms2D().at(0);
  histogram->Draw("COLZ");
  QString title = QString("%1 channel %2").arg(histogram->GetName()).arg(ch);
  setTitle(title);
  TH1D* originalProjection = histogram->ProjectionY(qPrintable(title+"originalProjection"), ch+1, ch+1);
  originalProjection->GetXaxis()->SetRangeUser(-4, 4);
  addHistogram(originalProjection);
  TH1D* modifiedProjection = histogram->ProjectionY(qPrintable(title+"modifiedProjection"), ch+1, ch+1);
  if (modifiedProjection->GetEntries() > 0) modifiedProjection->Smooth(10);
  //if (modifiedProjection->GetEntries()) modifiedProjection->Rebin(2);
  modifiedProjection->SetName(qPrintable(title));
  modifiedProjection->SetLineColor(kRed);
  setAxisTitle("#Deltat / ns", "");
  TF1* function = new TF1(qPrintable(title + "Function"), "gaus", modifiedProjection->GetXaxis()->GetXmin(), modifiedProjection->GetXaxis()->GetXmax());
  function->SetParameters(modifiedProjection->GetMaximum(), modifiedProjection->GetMean(), modifiedProjection->GetRMS());
  function->FixParameter(1, modifiedProjection->GetMean());
  if (modifiedProjection->GetEntries() > 0) {
    modifiedProjection->Fit(function, "QN0");
    function->ReleaseParameter(1);
    for (int i = 0; i < 5; ++i) {
      double mean = function->GetParameter(1);
      double sigma = qMax(0.5, function->GetParameter(2));
      function->SetRange(mean - sigma, mean + sigma);
      modifiedProjection->Fit(function, "RQN0");
    }
  }
  function->SetRange(modifiedProjection->GetXaxis()->GetXmin(), modifiedProjection->GetXaxis()->GetXmax());
  function->SetNpx(1000);
  QStringList stringList = title.split(" ");
  int id = (stringList[ch < 4 ? 2 : 3].remove(0, 2).toInt(0, 16)) | (ch - (ch < 4 ? 0 : 4));
  int refCh = stringList[5].toInt();
  //qDebug() << qPrintable(QString("0x%1=%2").arg(id, 0, 16).arg(-function->GetParameter(1)));
  TimeShiftContainer::instance()->setData(id, refCh, -function->GetParameter(1));
  addHistogram(modifiedProjection);
  addFunction(function);
  TLatex* latex = 0;
  latex = RootPlot::newLatex(0.15, 0.85);
  latex->SetTextColor(kRed);
  latex->SetTitle(qPrintable(QString("mean  = %1%2 ns")
    .arg(modifiedProjection->GetMean() < 0 ? '-' : ' ')
    .arg(qAbs(modifiedProjection->GetMean()), 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.82);
  latex->SetTextColor(kRed);
  latex->SetTitle(qPrintable(QString("x_fit = %1%2 ns")
    .arg(function->GetParameter(1) < 0 ? '-' : ' ')
    .arg(qAbs(function->GetParameter(1)), 0, 'f', 3, ' ')));
  addLatex(latex);
  TLine* line = 0;
  line = new TLine(modifiedProjection->GetMean(), 0, modifiedProjection->GetMean(), 1.05 * qMax(modifiedProjection->GetMaximum(), modifiedProjection->GetMaximum()));
  line->SetLineColor(kRed);
  line->SetLineStyle(2);
  line->SetLineWidth(2);
  m_lines.append(line);

  for (int i = -4; i < 4; ++i) {
    line = new TLine(i+0.4, 0.1, i+0.4, 1.05 * qMax(modifiedProjection->GetMaximum(), originalProjection->GetMaximum()));
    line->SetLineColor(kGreen);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    m_lines.append(line);
  }
}

ChannelTimeShiftHistogram::~ChannelTimeShiftHistogram()
{
  qDeleteAll(m_lines);
}

void ChannelTimeShiftHistogram::draw(TCanvas* canvas)
{
  H1DPlot::draw(canvas);
  foreach (TLine* line, m_lines)
    line->Draw();
}
