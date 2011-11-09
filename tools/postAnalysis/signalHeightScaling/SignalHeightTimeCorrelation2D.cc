#include "SignalHeightTimeCorrelation2D.hh"

#include "Corrections.hh"

#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TList.h>
#include <TF1.h>
#include <TLatex.h>
#include <TMultiGraph.h>

#include <iostream>
#include <iomanip>
#include <math.h>

#include <QDebug>
#include <QStringList>

SignalHeightTimeCorrelation2D::SignalHeightTimeCorrelation2D(PostAnalysisCanvas* canvas, unsigned short sipmId)
  : PostAnalysisPlot()
  , H2DProjectionPlot()
  , m_sipmId(sipmId)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);
  addHistogram(new TH2D(*histogram));
}

SignalHeightTimeCorrelation2D::~SignalHeightTimeCorrelation2D()
{
}
