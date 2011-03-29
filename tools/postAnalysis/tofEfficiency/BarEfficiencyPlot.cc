#include "BarEfficiencyPlot.hh"

#include <TH2.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>

#include <iostream>
#include <iomanip>

#include <QDebug>
#include <QStringList>

BarEfficiencyPlot::BarEfficiencyPlot(TCanvas* c1, TCanvas* c2, TCanvas* c3, TCanvas* c4)
  : PostAnalysisPlot()
  , H2DPlot()
{
  TH2D* h1 = findHistogram(c1);
  TH2D* h2 = findHistogram(c2);
  TH2D* h3 = findHistogram(c3);
  TH2D* h4 = findHistogram(c4);
  QString title;
  title = QString("%1 bar efficiency").arg(c1->GetName());
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
  setHistogram(h);
}

BarEfficiencyPlot::BarEfficiencyPlot(QVector<TCanvas*> canvases)
: PostAnalysisPlot()
, H2DPlot()
{
  TH2* h0 = findHistogram(canvases[0]);
  int nBinsX = h0->GetXaxis()->GetNbins();
  int nBinsY = h0->GetYaxis()->GetNbins();
  double minX = h0->GetXaxis()->GetXmin();
  double maxX = h0->GetXaxis()->GetXmax();
  double minY = h0->GetYaxis()->GetXmin();
  double maxY = h0->GetYaxis()->GetXmax();
  QString title;
  title = QString("tof efficiency");
  setTitle(title);
  TH2D* h = new TH2D(qPrintable(title), "", nBinsX, minX, maxX, nBinsY, minY, maxY);
  h->GetXaxis()->SetTitle("y_{tracker} / mm");
  h->GetYaxis()->SetTitle("x_{tracker} / mm");
  h->GetZaxis()->SetTitle("efficiency");
  int n = 0;
  for (int i = 0; i < 64; ++i) {
    QString name = canvases[i]->GetName();
    if (name.contains("0x8034")) {
      continue;
    }
    TH2D* hi = findHistogram(canvases[i]);
    h->Add(hi, 1);
    n++;
  }
  h->Scale(1/double(n));
  setHistogram(h);
  histogram()->GetZaxis()->SetRangeUser(0.97, 1.01);
}

BarEfficiencyPlot::~BarEfficiencyPlot()
{}

TH2D* BarEfficiencyPlot::findHistogram(TCanvas* canvas)
{
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      return static_cast<TH2D*>(canvas->GetListOfPrimitives()->At(i));
  }
  return 0;
}

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
