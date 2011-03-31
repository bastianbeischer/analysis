#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "TimeDifferenceFunction.hh"
#include "TimeDifferencePlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH2.h>
#include <TF2.h>

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
  PostAnalysisCanvas* canvas = 0;
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();
  int ids[] = {
    0x8000, 0x8004, 0x8008, 0x800c,
    0x8010, 0x8014, 0x8018, 0x801c,
    0x8020, 0x8024, 0x8028, 0x802c,
    0x8030, 0x8034, 0x8038, 0x803c
  };
  for (int i = 0; i < 16; ++i) {
    QString title = QString("tof time difference position 0x%1 canvas").arg(ids[i], 0, 16);
    canvas = addCanvas(&file, qPrintable(title));
    TimeDifferenceFunction* f = new TimeDifferenceFunction(canvas);
    addPlot(f);
    addPlot(new TimeDifferencePlot(canvas, f));
  }
  file.Close();
}
