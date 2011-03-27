#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "timeOverThresholdScaling.hh"
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
  for (int i = 0x8000; i <= 0x803f; i++) {
    QString name = QString("time over threshold temperature correlation 0x%1 canvas").arg(i , 0, 16);
    canvas = addCanvas(&file, qPrintable(name));
    addPlot(new TimeOverThresholdScaling(canvas, i));
  }
  file.Close();
}
