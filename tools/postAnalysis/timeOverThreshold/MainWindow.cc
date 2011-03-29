#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "TimeOverThresholdProjection.hh"
#include "ResiduumPlot.hh"

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
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();
  PostAnalysisCanvas* canvas = addCanvas(&file, "time over threshold canvas");
  for (int i = 0; i < 64; ++i)
    addPlot(new TimeOverThresholdProjection(canvas, i));
  for (int ch = 0x8000; ch <= 0x803f; ch++) {
    const QString title = "time over threshold temperature correlation 0x"+QString::number(ch, 16);
    const QString canvasName = title + " canvas";
    canvas = addCanvas(&file, qPrintable(canvasName));
    addPlot(new ResiduumPlot(canvas, title, 30));
  }
  for (int ch = 0x8000; ch <= 0x803f; ch++) {
    const QString title = "time over threshold time correlation 0x"+QString::number(ch, 16);
    const QString canvasName = title + " canvas";
    canvas = addCanvas(&file, qPrintable(canvasName));
    addPlot(new ResiduumPlot(canvas, title, 30));
  }
  file.Close();
}
