#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "MergedHistogram1D.hh"
#include "MergedHistogram2D.hh"
#include "ProgressBar.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>
#include <QMultiMap>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_files()
{
}

MainWindow::~MainWindow()
{
  foreach (TFile* file, m_files) {
    file->Close();
    delete file;
  }
}

void MainWindow::setupAnalysis()
{
  QMultiMap<QString, TFile*> canvases;
  foreach (QString fileName, m_analysisFiles) {
    TFile* file = new TFile(qPrintable(fileName));
    QStringList canvasNames = PostAnalysisCanvas::savedCanvases(file);
    gROOT->cd();
    foreach (QString key, canvasNames)
      canvases.insert(key, file);
    m_files.append(file);
  }
  QList<QString> keys = canvases.uniqueKeys();
  ProgressBar bar(keys.count());
  foreach (QString canvasName, keys) {
    QVector<PostAnalysisCanvas*> canvasesForPlot;
    QMap<QString, TFile*>::ConstIterator upperBound = canvases.upperBound(canvasName);
    int histogramDimension = 0;
    for (QMap<QString, TFile*>::ConstIterator it = canvases.lowerBound(canvasName); it != upperBound; ++it) {
      QString fileName =  QString(it.value()->GetName());
      fileName.remove(0, fileName.lastIndexOf("/") + 1);
      fileName.chop(5);
      PostAnalysisCanvas* canvas = new PostAnalysisCanvas(it.value(), canvasName + " (" + fileName + ")");
      addCanvas(canvas);
      if (canvas->histograms1D().count() && canvas->histograms2D().count() == 0)
        histogramDimension = 1;
      if (canvas->histograms2D().count() && canvas->histograms1D().count() == 0)
        histogramDimension = 2;
      if (histogramDimension)
        canvasesForPlot.append(canvas);
    }
    if (canvasesForPlot.count()) {
      if (histogramDimension == 1)
        addPlot(new MergedHistogram1D(canvasesForPlot));
      if (histogramDimension == 2)
        addPlot(new MergedHistogram2D(canvasesForPlot));
    }
    bar.next();
  }
}
