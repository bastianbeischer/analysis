#include "SignalHeightTimeCorrelation2D.hh"

#include "Corrections.hh"
#include "RootQtWidget.hh"
#include "ProjectionControlWidget.hh"

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
#include <QDateTime>
#include <QComboBox>

SignalHeightTimeCorrelation2D::SignalHeightTimeCorrelation2D(PostAnalysisCanvas* canvas, unsigned short sipmId)
  : H2DProjectionPlot()
  , PostAnalysisPlot()
  , m_sipmId(sipmId)
{
  TH2D* histogram = canvas->histograms2D().at(0);
  QString title = QString(canvas->name()).replace("canvas", "histogram");
  setTitle(title);
  setAxisTitle("time", "signal height / adc counts", "");
  addHistogram(new TH2D(*histogram));
  TF1* function = new TF1(qPrintable(QString(histogram->GetTitle()) + "Function"), "landau", histogram->GetYaxis()->GetXmin(), histogram->GetYaxis()->GetXmax());
  function->SetLineColor(kRed);
  addProjectionFunction(function);
  addProjectionLatex(RootPlot::newLatex(.60, .85));
  addProjectionLatex(RootPlot::newLatex(.60, .80));
  addProjectionLatex(RootPlot::newLatex(.60, .75));
  addProjectionLatex(RootPlot::newLatex(.60, .70));
  const int defaultIndex = projectionControlWidget()->comboBox()->findText("projection on y");
  projectionControlWidget()->comboBox()->setCurrentIndex(defaultIndex);
}

SignalHeightTimeCorrelation2D::~SignalHeightTimeCorrelation2D()
{
}

void SignalHeightTimeCorrelation2D::updateProjection()
{
  if (projectionType() == ProjectionOnY) {
    const QDateTime& lowTime = QDateTime::fromTime_t(histogram()->GetXaxis()->GetBinLowEdge(projectionBin()));
    const QDateTime& upTime = QDateTime::fromTime_t(histogram()->GetXaxis()->GetBinLowEdge(projectionBin() + 1));
    const int nEntries = projection()->GetEntries();
    projection()->Fit(projectionFunction(), "EQN0");
    const double mpv = projectionFunction()->GetParameter(1);
    const double rms = projection()->GetRMS();
    projectionLatex(0)->SetTitle(qPrintable(QString("time: %1 to %2").arg(lowTime.toString("h:mm:ss")).arg(upTime.toString("h:mm:ss"))));
    projectionLatex(1)->SetTitle(qPrintable(QString("entries: %1").arg(nEntries)));
    projectionLatex(2)->SetTitle(qPrintable(QString("mpv: %1").arg(mpv)));
    projectionLatex(3)->SetTitle(qPrintable(QString("sigma: %1").arg(rms / sqrt(nEntries))));
  }
}
