#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "MeasurementTimePostAnalysisPlot.hh"
#include "RigidityFluxPlot.hh"

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
  PostAnalysisCanvas* canvas = 0;
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();

  QString name = QString("measurement time canvas");
  canvas = addCanvas(&file, qPrintable(name));
  MeasurementTimePostAnalysisPlot* timePlot = new MeasurementTimePostAnalysisPlot(canvas);
  addPlot(timePlot);
  double measurementTime = timePlot->measurementTime();

  name = QString("particle spectrum - all canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new RigidityFluxPlot(canvas, measurementTime));

  name = QString("particle spectrum - positive canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new RigidityFluxPlot(canvas, measurementTime));

  name = QString("particle spectrum - negative canvas");
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new RigidityFluxPlot(canvas, measurementTime));

  file.Close();
}
