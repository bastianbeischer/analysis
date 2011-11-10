#include "SignalHeightTimeCorrelation2D.hh"

#include "Corrections.hh"
#include "RootQtWidget.hh"

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
  : H2DProjectionPlot()
  , PostAnalysisPlot()
  , m_sipmId(sipmId)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);
  addHistogram(new TH2D(*histogram));
  TF1* function = new TF1(qPrintable(QString(histogram->GetTitle()) + "Function"), "landau", histogram->GetYaxis()->GetXmin(), histogram->GetYaxis()->GetXmax());
  function->SetLineColor(kRed);
  addProjectionFunction(function);
}

SignalHeightTimeCorrelation2D::~SignalHeightTimeCorrelation2D()
{
}

void SignalHeightTimeCorrelation2D::updateProjection()
{
  projection()->Fit(projectionFunction(), "EQN0");
}
