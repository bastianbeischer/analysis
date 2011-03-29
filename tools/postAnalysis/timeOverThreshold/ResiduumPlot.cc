#include "ResiduumPlot.hh"
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

ResiduumPlot::ResiduumPlot(PostAnalysisCanvas* canvas, QString title, double referenceValue)
  : PostAnalysisPlot()
  , H1DPlot()
{
  TH2D* h2 = canvas->histograms2D().at(0);
  title = canvas->name().replace("canvas", QString("histogram"));

  const unsigned int nBins = h2->GetNbinsY();
  const double xMin = h2->GetYaxis()->GetXmin() - referenceValue;
  const double xMax = h2->GetYaxis()->GetXmax() - referenceValue;
  
  TH1D* histogram = new TH1D(qPrintable(title), "", nBins, xMin, xMax);
  for (int binX = 0; binX < h2->GetNbinsX(); binX++) {
    TH1* projectionHistogram = h2->ProjectionY("_py", binX + 1, binX + 1);
    if (projectionHistogram->GetEntries() > 0) {
      const double value = projectionHistogram->GetMean();
      const double residuum = value - referenceValue;
      histogram->Fill(residuum);
    }
  }
  setTitle(title);
  const QString xTitle = h2->GetYaxis()->GetTitle();
  histogram->GetXaxis()->SetTitle(qPrintable(xTitle));
  addHistogram(histogram);
  
  TLatex* latex = 0;  
  latex = RootPlot::newLatex(.65, .85);
  latex->SetTitle(qPrintable(QString("n  = %1").arg(histogram->GetEntries())));
  addLatex(latex);
  
  latex = RootPlot::newLatex(.65, .82);
  latex->SetTitle(qPrintable(QString("mean  = %1").arg(histogram->GetMean())));
  addLatex(latex);
  
  latex = RootPlot::newLatex(.65, .79);
  latex->SetTitle(qPrintable(QString("rms  = %1").arg(histogram->GetRMS())));
  addLatex(latex);
}

ResiduumPlot::~ResiduumPlot()
{}
