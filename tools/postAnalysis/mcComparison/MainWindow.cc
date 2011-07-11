#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "PostAnalysisWindow.hh"
#include "RootStyle.hh"
#include "HistCompare.hh"
#include "HistResiduals.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TKey.h>

#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>

#include <iostream>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis(QStringList args)
{
  args.removeFirst();

  QList<PostAnalysisCanvas*> allCanvases;

  foreach (QString filename, args) {
    TFile file(qPrintable(filename));
    gROOT->cd();

    QString analysisCanvasName = "complete TRD spectrum canvas";
    QString oldMCCanvasName = "old_mc_canvas";

    PostAnalysisCanvas* canvas = 0;
    //get canvas
    if (file.Get(qPrintable(analysisCanvasName)))
      canvas = addCanvas(&file, analysisCanvasName);
    else if (file.Get(qPrintable(oldMCCanvasName)))
      canvas = addCanvas(&file, oldMCCanvasName);
    else
      continue;

    qDebug() << "canvas " << canvas->name() << " got " << canvas->histograms1D().size() << " 1D histos";
    allCanvases << canvas;

    file.Close();
  }

  if (allCanvases.size() > 2)
    addPlot(new HistCompare(allCanvases));

  for (int i = 0; i < allCanvases.size()-1; ++i) {
    for (int j = i+1; j < allCanvases.size(); ++j) {
      QList<PostAnalysisCanvas*> twoCanvases;
      twoCanvases << allCanvases.at(i);
      twoCanvases << allCanvases.at(j);
      addPlot(new HistCompare(twoCanvases));
      addPlot(new HistResiduals(twoCanvases.at(0), twoCanvases.at(1), false));
      addPlot(new HistResiduals(twoCanvases.at(0), twoCanvases.at(1), true));
    }
  }
}

