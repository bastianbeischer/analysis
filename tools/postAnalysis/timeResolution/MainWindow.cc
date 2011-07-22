#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "TimeOfFlightHistogram.hh"
#include "RootStyle.hh"
#include "TimeResolutionPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH2.h>

#include <QFileDialog>

#include <QDebug>

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
  const char* upper[] = {"0x8000 0x8010", "0x8004 0x8014", "0x8008 0x8018", "0x800c 0x801c"};
  const char* lower[] = {"0x8020 0x8030", "0x8024 0x8034", "0x8028 0x8038", "0x802c 0x803c"};
  for (int u = 0; u < 4; ++u) {
    for (int l = 0; l < 4; ++l) {
      QString title = QString("time resolution %1, %2 canvas").arg(upper[u]).arg(lower[l]);
      QVector<TimeOfFlightHistogram*> timeOfFlightHistograms;
      qDebug() << title;
      canvas = addCanvas(&file, qPrintable(title));
      int nBins = canvas->histograms2D().at(0)->GetXaxis()->GetNbins();
      for (int bin = 1; bin <= nBins; ++bin) {
        TimeOfFlightHistogram* h = new TimeOfFlightHistogram(canvas, bin);
        addPlot(h);
        timeOfFlightHistograms.append(h);
      }
      addPlot(new TimeResolutionPlot(timeOfFlightHistograms));
    }
  }
  file.Close();
}
