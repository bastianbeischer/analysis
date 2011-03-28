#include "ChannelTimeShiftHistogram.hh"

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

ChannelTimeShiftHistogram::ChannelTimeShiftHistogram(TCanvas* canvas, int ch)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH2* histogram = 0;
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      histogram = static_cast<TH2*>(canvas->GetListOfPrimitives()->At(i));
  }
  histogram->Draw("COLZ");
  QString title = QString("%1 channel %2").arg(canvas->GetName()).arg(ch);
  setTitle(title);
  TH1D* projection = histogram->ProjectionY("tmp", ch+1, ch+1);
  if (ch > 0)
    projection->Smooth();
  projection->SetName(qPrintable(title));
  projection->GetXaxis()->SetTitle("#Deltat / ns");
  TF1* function = new TF1(qPrintable(title + "Function"), "gaus", projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  projection->Fit(function, "QN0");
  if (ch > 0) {
    for (int i = 0; i < 5; ++i) {
      double mean = function->GetParameter(1);
      double sigma = function->GetParameter(2);
      function->SetRange(mean - 1.5 * sigma, mean + 1.5 * sigma);
      projection->Fit(function, "RQN0");
    }
  }
  function->SetRange(projection->GetXaxis()->GetXmin(), projection->GetXaxis()->GetXmax());
  QStringList stringList = title.split(" ");
  int id = (stringList[ch < 4 ? 2 : 3].remove(0, 2).toInt(0, 16)) | (ch - (ch < 4 ? 0 : 4));
  std::cout << "0x" <<std::hex << id << "=" << -function->GetParameter(1) << std::endl;
  projection->GetXaxis()->SetRangeUser(-2, 2);
  addHistogram(projection);
  addFunction(function);
}

ChannelTimeShiftHistogram::~ChannelTimeShiftHistogram()
{}
