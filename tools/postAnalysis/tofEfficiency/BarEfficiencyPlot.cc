#include "BarEfficiencyPlot.hh"
#include "PostAnalysisCanvas.hh"

#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TLatex.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

BarEfficiencyPlot::BarEfficiencyPlot(PostAnalysisCanvas* c1, PostAnalysisCanvas* c2, PostAnalysisCanvas* c3, PostAnalysisCanvas* c4)
  : PostAnalysisPlot()
  , H2DPlot()
{
  TH2D* h1 = c1->histograms2D().at(0);
  TH2D* h2 = c2->histograms2D().at(0);
  TH2D* h3 = c3->histograms2D().at(0);
  TH2D* h4 = c4->histograms2D().at(0);
  QString title;
  title = QString("%1 bar efficiency").arg(c1->name());
  setTitle(title);

  double sum1 = sumEntries(h1);
  double mean1 = sum1 / (h1->GetXaxis()->GetNbins() * h1->GetYaxis()->GetNbins());
  double sum2 = sumEntries(h2);
  double mean2 = sum2 / (h2->GetXaxis()->GetNbins() * h2->GetYaxis()->GetNbins());
  double sum3 = sumEntries(h3);
  double mean3 = sum3 / (h3->GetXaxis()->GetNbins() * h3->GetYaxis()->GetNbins());
  double sum4 = sumEntries(h4);
  double mean4 = sum4 / (h4->GetXaxis()->GetNbins() * h4->GetYaxis()->GetNbins());

  qDebug() << title << ((1.-mean1)+(1.-mean2)+(1.-mean3)+(1.-mean4))/4.;

  int nBinsX = h1->GetXaxis()->GetNbins();
  int nBinsY = h1->GetYaxis()->GetNbins();
  double minX = h1->GetXaxis()->GetXmin();
  double maxX = h1->GetXaxis()->GetXmax();
  double minY = h1->GetYaxis()->GetXmin();
  double maxY = h1->GetYaxis()->GetXmax();
  TH2D* h = new TH2D(qPrintable(title), "", nBinsX, minX, maxX, nBinsY, minY, maxY);
  setAxisTitle("y_{tracker} / mm", "x_{tracker} / mm", "efficiency");
  h->Add(h1, 0.25);
  h->Add(h2, 0.25);
  h->Add(h3, 0.25);
  h->Add(h4, 0.25);

  double sum = sumEntries(h);
  double mean = sum / (h->GetXaxis()->GetNbins() * h->GetYaxis()->GetNbins());

  addHistogram(h);

  TLatex* latex = 0;
  latex = RootPlot::newLatex(.3, .82);
  latex->SetTitle(qPrintable(QString("mean efficiency = %1").arg(mean)));
  addLatex(latex);
}

BarEfficiencyPlot::~BarEfficiencyPlot()
{}

double BarEfficiencyPlot::sumEntries(TH2D* h)
{
  double sum = 0;
  for (int binX = 1; binX <= h->GetXaxis()->GetNbins(); ++binX) {
    for (int binY = 1; binY <= h->GetYaxis()->GetNbins(); ++binY) {
      sum+= h->GetBinContent(binX, binY);
    }
  }
  return sum;
}
