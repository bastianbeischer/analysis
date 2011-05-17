#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "MoPVScaling.hh"

#include "Setup.hh"

#include "DetectorElement.hh"

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
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();

  //add info plots
  addCanvas(&file, "TRD_GAS_COLD_TEMP canvas");
  addCanvas(&file, "TRD_PRESSURE canvas");

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();

  //vs time:
  QString name = "complete TRD vs time spectrum (-100 GeV to 100 GeV) canvas";
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MoPVScaling(canvas));

  QList<PostAnalysisCanvas*> mopvVsTimeModules;
  ElementIterator elementIt;
  for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd) {
      QString cTitle = QString("module vs time spectrum 0x%1 (-100 GeV to 100 GeV) canvas").arg(QString::number(element->id(),16));
      mopvVsTimeModules << addCanvas(&file, qPrintable(cTitle));
    }
  }
  addPlot(new MoPVScaling(mopvVsTimeModules));


  //vs temperature:
  name = "complete TRD vs temperature spectrum (-100 GeV to 100 GeV) canvas";
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MoPVScaling(canvas));

  QList<PostAnalysisCanvas*> mopvVsTemperatureModules;
  for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd) {
      QString cTitle = QString("module vs temperature spectrum 0x%1 (-100 GeV to 100 GeV) canvas").arg(QString::number(element->id(),16));
      mopvVsTemperatureModules << addCanvas(&file, qPrintable(cTitle));
    }
  }
  addPlot(new MoPVScaling(mopvVsTemperatureModules));


  //vs pressure
  name = "complete TRD vs pressure spectrum (-100 GeV to 100 GeV) canvas";
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MoPVScaling(canvas));

  QList<PostAnalysisCanvas*> mopvVsPressureModules;
  for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd) {
      QString cTitle = QString("module vs pressure spectrum 0x%1 (-100 GeV to 100 GeV) canvas").arg(QString::number(element->id(),16));
      mopvVsPressureModules << addCanvas(&file, qPrintable(cTitle));
    }
  }
  addPlot(new MoPVScaling(mopvVsPressureModules));

  file.Close();
}
