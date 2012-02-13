#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>

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
  typedef QPair<QString, TFile*> Key;
  QVector<Key> allKeys;
  foreach (QString fileName, m_analysisFiles) {
    TFile* file = new TFile(qPrintable(fileName));
    QStringList fileKeyList = PostAnalysisCanvas::savedCanvases(file);
    gROOT->cd();
    foreach (QString key, fileKeyList)
      allKeys.append(Key(key + " (" + fileName + ")", file));
    m_files.append(file);
    qDebug() << fileName;
  }
  qSort(allKeys);
  foreach (Key key, allKeys) {
    qDebug() << key.first;
    addCanvas(new PostAnalysisCanvas(key.second, key.first));
  }
}
