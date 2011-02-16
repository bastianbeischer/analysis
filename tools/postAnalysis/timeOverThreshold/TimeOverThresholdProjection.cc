#include "TimeOverThresholdProjection.hh"

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

TimeOverThresholdProjection::TimeOverThresholdProjection(TCanvas* canvas, int ch)
  : PostAnalysisPlot()
  , H1DPlot()
{
  QString title;
  TH1D* histogram = 0;
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D")) {
      title = QString(canvas->GetName()).replace("canvas", QString("0x%1 histogram").arg(ch | 0x8000, 0, 16));
      TH2D* h = static_cast<TH2D*>(canvas->GetListOfPrimitives()->At(i));
      histogram = h->ProjectionY(qPrintable(title + "projection"), ch+1, ch+1);
    }
  }
  setTitle(title);
  histogram->GetXaxis()->SetTitle("time over threshold / ns");
  addHistogram(histogram);
  
  TLatex* latex = 0;
  
  latex = RootPlot::newLatex(.67, .85);
  latex->SetTitle(qPrintable(QString("id = 0x%1").arg(ch | 0x8000, 0, 16)));
  addLatex(latex);
  
  latex = RootPlot::newLatex(.67, .82);
  latex->SetTitle(qPrintable(QString("n  = %1").arg(histogram->GetEntries())));
  addLatex(latex);
}

TimeOverThresholdProjection::~TimeOverThresholdProjection()
{}
