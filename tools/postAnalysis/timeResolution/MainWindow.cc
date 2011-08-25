#include "MainWindow.hh"

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "TimeOfFlightHistogram.hh"
#include "RootStyle.hh"
#include "TimeResolutionPlot.hh"
#include "TimeResolutionAnalysis.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH2.h>

#include <QFileDialog>

#include <cmath>

#include <QComboBox>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
  , m_plotComboBox(new QComboBox)
  , m_plots()
  , m_evaluatedPlots()
  , m_discardedPlots()
  , m_bothPlots()
{
  m_plotComboBox->addItem("none");
  m_plotComboBox->addItem("evaluated");
  m_plotComboBox->addItem("discarded");
  m_plotComboBox->addItem("evaluated+discarded");
  connect(m_plotComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(plotSelectionChanged(int)));
  addWidget(m_plotComboBox);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  PostAnalysisCanvas* canvas = 0;
  TFile file(qPrintable(m_analysisFiles.at(0)));
  gROOT->cd();
  const char* upper[] = {"0x8000 0x8010", "0x8004 0x8014", "0x8008 0x8018", "0x800c 0x801c"};
  const char* lower[] = {"0x8020 0x8030", "0x8024 0x8034", "0x8028 0x8038", "0x802c 0x803c"};
  QVector<TimeOfFlightHistogram*> timeOfFlightHistograms;
  int nBins = -1;
  for (int u = 0; u < 4; ++u) {
    for (int l = 0; l < 4; ++l) {
      QString title = QString("time resolution %1, %2 canvas").arg(upper[u]).arg(lower[l]);
      qDebug() << title;
      canvas = addCanvas(&file, qPrintable(title));
      if (nBins < 0)
        nBins = canvas->histograms2D().at(0)->GetXaxis()->GetNbins();
      Q_ASSERT(nBins == canvas->histograms2D().at(0)->GetXaxis()->GetNbins());
      for (int bin = 1; bin <= nBins; ++bin) {
        TimeOfFlightHistogram* h = new TimeOfFlightHistogram(canvas, bin);
        m_bothPlots.append(h);
        if (h->fitGood())
          m_evaluatedPlots.append(h);
        else
          m_discardedPlots.append(h);
        timeOfFlightHistograms.append(h);
      }
    }
  }
  PostAnalysisCanvas* mcCanvas = addCanvas(&file, "timeResolutionToyMC canvas");
  TimeResolutionAnalysis* analysis = new TimeResolutionAnalysis(timeOfFlightHistograms, sqrt(nBins));
  m_plots.append(new TimeResolutionPlot(analysis, TimeResolutionPlot::Variance));
  m_plots.append(new TimeResolutionPlot(analysis, TimeResolutionPlot::StandardDeviation));
  m_plots.append(new TimeResolutionPlot(analysis, TimeResolutionPlot::StandardDeviation, mcCanvas));
  file.Close();
  plotSelectionChanged(0);
}

void MainWindow::plotSelectionChanged(int i)
{
  clearPlots();
  foreach (PostAnalysisPlot* plot, m_plots)
    addPlot(plot);
  if (i == 1) {
    foreach (PostAnalysisPlot* plot, m_evaluatedPlots)
      addPlot(plot);
  } else if (i == 2) {
    foreach (PostAnalysisPlot* plot, m_discardedPlots)
      addPlot(plot);
  } else if (i == 3) {
    foreach (PostAnalysisPlot* plot, m_bothPlots)
      addPlot(plot);
  }
}
