#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "PostAnalysisCanvas.hh"
#include "MoPVScaling.hh"
#include "MoPVScalingTime.hh"
#include "MoPVScalingPressure.hh"
#include "MoPVScalingTemperature.hh"
#include "MoPVConstant.hh"
#include "LHFit.hh"

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
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();

  //add TRDSpectra and LHFit plots
  PostAnalysisCanvas* completeTRDSpectrumCanvas = addCanvas(&file, "complete TRD spectrum canvas");
  addPlot(new LHFit(completeTRDSpectrumCanvas));
  //add TRDSpectra for each layer
  for (int i = 0; i < 8; ++i) {
    PostAnalysisCanvas* trdLayerSpectrum = addCanvas(&file, "TRD layer spectrum " + QString::number(i) + " canvas");
    addPlot(new LHFit(trdLayerSpectrum, i));
  }


  //add info plots
  addCanvas(&file, "TRD_GAS_COLD_TEMP canvas");
  addCanvas(&file, "TRD_PRESSURE canvas");
  addCanvas(&file, "TRD_PRESSURE_SMOOTHED canvas");

  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  ElementIterator elementIt;

  //add module spectra:
  QList<PostAnalysisCanvas*> mopvModules;
  for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd) {
      QString cTitle = QString("Module spectrum 0x%1 canvas").arg(QString::number(element->id(),16));
      mopvModules << addCanvas(&file, qPrintable(cTitle));
    }
  }
  addPlot(new MoPVConstant(mopvModules, true));

  //add channel spectra:
  QList<PostAnalysisCanvas*> mopvChannels;
  for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd) {
      for(int i = 0; i < 16; i++) {
        QString cTitle = QString("Channel spectrum 0x%1 canvas").arg(QString::number(element->id() + i,16));
        mopvChannels << addCanvas(&file, qPrintable(cTitle));
      }
    }
  }
  addPlot(new MoPVConstant(mopvChannels, false));

  //vs time:
  QString name = "complete TRD vs time spectrum canvas";
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MoPVScalingTime(canvas));

  QList<PostAnalysisCanvas*> mopvVsTimeModules;
  for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd) {
      QString cTitle = QString("module vs time spectrum 0x%1 canvas").arg(QString::number(element->id(),16));
      mopvVsTimeModules << addCanvas(&file, qPrintable(cTitle));
    }
  }
  addPlot(new MoPVScaling(mopvVsTimeModules));

  /*
  //vs temperature:
  name = "complete TRD vs temperature spectrum canvas";
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MoPVScalingTemperature(canvas));

  QList<PostAnalysisCanvas*> mopvVsTemperatureModules;
  for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd) {
      QString cTitle = QString("module vs temperature spectrum 0x%1 canvas").arg(QString::number(element->id(),16));
      mopvVsTemperatureModules << addCanvas(&file, qPrintable(cTitle));
    }
  }
  addPlot(new MoPVScaling(mopvVsTemperatureModules));


  //vs pressure
  name = "complete TRD vs pressure spectrum canvas";
  canvas = addCanvas(&file, qPrintable(name));
  addPlot(new MoPVScalingPressure(canvas));

  QList<PostAnalysisCanvas*> mopvVsPressureModules;
  for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
    DetectorElement* element = *elementIt;
    if (element->type() == DetectorElement::trd) {
      QString cTitle = QString("module vs pressure spectrum 0x%1 canvas").arg(QString::number(element->id(),16));
      mopvVsPressureModules << addCanvas(&file, qPrintable(cTitle));
    }
  }
  addPlot(new MoPVScaling(mopvVsPressureModules));
  */

  file.Close();
}
