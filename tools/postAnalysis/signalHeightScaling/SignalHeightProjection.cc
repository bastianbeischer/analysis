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
  
  TLatex* latex = 0;
  
  latex = RootPlot::newLatex(.67, .85);
  latex->SetTitle(qPrintable(QString("id = 0x%1").arg(m_id, 0, 16)));
  addLatex(latex);
  
  latex = RootPlot::newLatex(.67, .82);
  latex->SetTitle(qPrintable(QString("n  = %1").arg(histogram->GetEntries())));
  addLatex(latex);
}

SignalHeightProjection::~SignalHeightProjection()
{}
