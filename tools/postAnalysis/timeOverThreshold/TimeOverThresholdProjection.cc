#include "TimeOverThresholdProjection.hh"
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

TimeOverThresholdProjection::TimeOverThresholdProjection(PostAnalysisCanvas* canvas, int ch)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH2D* histogram2D = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", QString("0x%1 histogram").arg(ch | 0x8000, 0, 16));
  TH1D* histogram = histogram2D->ProjectionY(qPrintable(title + "projection"), ch+1, ch+1);

  setTitle(title);
  setAxisTitle("time over threshold / ns", "");
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
