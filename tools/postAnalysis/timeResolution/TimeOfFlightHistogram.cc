#include "TimeOfFlightHistogram.hh"
#include "PostAnalysisCanvas.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>

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
{
  TH2D* histogram = canvas->histograms2D().at(0);
  histogram->Draw("COLZ");
  QString title = QString("%1 channel %2").arg(canvas->name()).arg(bin);
  setTitle(title);
  TH1D* projection = histogram->ProjectionY("tmp", bin+1, bin+1);
  if (bin > 0)
    projection->Smooth();
  projection->SetName(qPrintable(title));
  setAxisTitle("#Deltat / ns", "");
  TF1* function = new TF1(qPrintable(title + "Function"), "gaus", projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  projection->Fit(function, "QN0");
  if (bin > 0) {
    for (int i = 0; i < 5; ++i) {
      double mean = function->GetParameter(1);
      double sigma = function->GetParameter(2);
      function->SetRange(mean - 1.5 * sigma, mean + 1.5 * sigma);
      projection->Fit(function, "RQN0");
    }
  }
  function->SetRange(projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  QStringList stringList = title.split(" ");
  //int id = (stringList[ch < 4 ? 2 : 3].remove(0, 2).toInt(0, 16)) | (ch - (ch < 4 ? 0 : 4));
  //std::cout << "0x" <<std::hex << id << "=" << -m_fitFunction->GetParameter(1) << std::endl;
  addHistogram(projection);
  addFunction(function);
  TLatex* latex = 0;
  latex = RootPlot::newLatex(0.15, 0.85);
  latex->SetTitle(qPrintable(QString("mean   = %1 ns").arg(function->GetParameter(1), 0, 'f', 3, ' ')));
  addLatex(latex);
  latex = RootPlot::newLatex(0.15, 0.82);
  latex->SetTitle(qPrintable(QString("#sigma = %1 ns").arg(function->GetParameter(2), 0, 'f', 3, ' ')));
  addLatex(latex);
}

TimeOfFlightHistogram::~TimeOfFlightHistogram()
{}
