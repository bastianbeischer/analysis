#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "TrackerTemperaturePlot.hh"
#include "TriggerRateHeightCorrelationPlot.hh"
#include "TriggerRateHeightTimePlot.hh"
#include "SensorTypes.hh"

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

  PostAnalysisCanvas* triggerRateCanvas = addCanvas(&file, "trigger rate canvas");
  PostAnalysisCanvas* heightCanvas = addCanvas(&file, "EBASS_HEIGHT canvas");
  addPlot(new TriggerRateHeightTimePlot(triggerRateCanvas, heightCanvas));
  addPlot(new TriggerRateHeightCorrelationPlot(triggerRateCanvas, heightCanvas));

  QVector<SensorTypes::Type> temperatureSensors = QVector<SensorTypes::Type>::fromStdVector(SensorTypes::temperatureSensors());

  QVector<PostAnalysisCanvas*> trackerTemperatures;
  foreach(SensorTypes::Type type, temperatureSensors) {
    QString title = QString::fromStdString(convertToString(type)) + " canvas";
    if (title.contains("HPE"))
      trackerTemperatures.append(addCanvas(&file, title));
  }
  addPlot(new TrackerTemperaturePlot(trackerTemperatures));

  file.Close();
}
