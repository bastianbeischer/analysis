#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "TimeOverThresholdProjection.hh"
#include "ResiduePlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();
  PostAnalysisCanvas* canvas = addCanvas(&file, "time over threshold canvas");
  for (int i = 0; i < 64; ++i)
    addPlot(new TimeOverThresholdProjection(canvas, i));
  for (int ch = 0x8000; ch <= 0x803f; ch++) {
    QString name = QString("time over threshold temperature correlation 0x%1 canvas").arg(ch, 0, 16);
    canvas = addCanvas(&file, qPrintable(name));
    addPlot(new ResiduePlot(canvas));
  }
  for (int ch = 0x8000; ch <= 0x803f; ch++) {
    QString name = QString("time over threshold time correlation 0x%1 canvas").arg(ch, 0, 16);
    canvas = addCanvas(&file, qPrintable(name));
    addPlot(new ResiduePlot(canvas));
  }
  file.Close();
}
