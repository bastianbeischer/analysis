#include "SignalHeightProjection.hh"
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

SignalHeightProjection::SignalHeightProjection(PostAnalysisCanvas* canvas, unsigned short id)
  : PostAnalysisPlot()
  , H1DPlot()
  , m_id(id)
{
  QString channelString = QString("0x%1").arg(m_id, 0, 16);
  TH2D* histogram2D = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", QString("%1 histogram").arg(channelString));
  unsigned int bin = histogram2D->GetXaxis()->FindBin(qPrintable(channelString));
  TH1D* histogram = histogram2D->ProjectionY(qPrintable(title + "projection"), bin, bin);

  setTitle(title);
  setAxisTitle("signal height / adc counts", "");
  addHistogram(histogram);

  TLatex* latext = 0;
  latext = RootPlot::newLatex(.67, .85);
  latext->SetTitle(qPrintable(QString("id = 0x%1").arg(m_id, 0, 16)));
  addLatex(latext);
  latext = RootPlot::newLatex(.67, .82);
  latext->SetTitle(qPrintable(QString("n  = %1").arg(histogram->GetEntries())));
  addLatex(latext);

  TF1* function = new TF1(qPrintable(QString(histogram->GetTitle()) + "_function"), "landau", histogram->GetXaxis()->GetXmin(), histogram->GetXaxis()->GetXmax());
  function->SetLineColor(kRed);
  histogram->Fit(function, "EQN0");
  addFunction(function);
  addLatex(RootPlot::newLatex(.60, .75));
  addLatex(RootPlot::newLatex(.60, .70));
  const double mpv = function->GetParameter(1);
  const double mpvError = function->GetParError(1);
  latex(2)->SetTitle(qPrintable(QString("mpv: %1 #pm %2").arg(mpv).arg(mpvError)));
  latex(3)->SetTitle(qPrintable(QString("mean: %1").arg(histogram->GetMean())));
}

SignalHeightProjection::~SignalHeightProjection()
{}
