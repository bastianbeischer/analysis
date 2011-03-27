#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "TimeOverThresholdProjection.hh"
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
  TCanvas* canvas = 0;
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();
  canvas = addCanvas(&file, "time over threshold canvas");
  for (int i = 0; i < 64; ++i)
    addPlot(new TimeOverThresholdProjection(canvas, i));
  file.Close();
}
