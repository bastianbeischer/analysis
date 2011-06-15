#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "ChannelTimeShiftHistogram.hh"
#include "BarTimeShiftHistogram.hh"
#include "RootStyle.hh"
#include "PostAnalysisCanvas.hh"
#include "TimeShiftContainer.hh"
#include "BarShiftPlot.hh"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>
#include <QFileDialog>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
  : PostAnalysisWindow(parent)
{
  QPushButton* button = new QPushButton("save to config file...");
  connect(button, SIGNAL(clicked()), this, SLOT(saveToConfigFile()));
  addWidget(button);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupAnalysis()
{
  PostAnalysisCanvas* canvas = 0;
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();

  for (int refCh = 0; refCh < 8; ++refCh) {
    for (unsigned int bar = 0; bar < 8; ++bar) {
      for (int ch = 0; ch < 8; ++ch) {
        QString title = QString("time shift 0x%1 0x%2 ch %3 canvas")
          .arg(TimeShiftContainer::s_bars[bar][0], 0, 16).arg(TimeShiftContainer::s_bars[bar][1], 0, 16).arg(refCh);
        canvas = addCanvas(&file, qPrintable(title));
        addPlot(new ChannelTimeShiftHistogram(canvas, ch));
      }
    }
  }
  TimeShiftContainer::instance()->dump();
  TimeShiftContainer::instance()->shiftOnFirstChannel();
  TimeShiftContainer::instance()->dump();

  QVector<BarShiftPlot*> barShiftPlots;
  for (int upper = 0; upper < 4; ++upper) {
    for (int lower = 4; lower < 8; ++lower) {
      QString canvasTitle = QString("bar shift 0x%1 0x%2 0x%3 0x%4 canvas")
        .arg(TimeShiftContainer::s_bars[upper][0], 0, 16).arg(TimeShiftContainer::s_bars[upper][1], 0, 16)
        .arg(TimeShiftContainer::s_bars[lower][0], 0, 16).arg(TimeShiftContainer::s_bars[lower][1], 0, 16);
      canvas = addCanvas(&file, canvasTitle);
      BarShiftPlot* plot = new BarShiftPlot(canvas);
      addPlot(plot);
      barShiftPlots.append(plot);
    }
  }
  addPlot(new BarTimeShiftHistogram(barShiftPlots));
  file.Close();
  TimeShiftContainer::instance()->finalize();
  TimeShiftContainer::instance()->dump();
}

void MainWindow::saveToConfigFile()
{
  QFileDialog dialog(this, "save time shifts...", ".", "CONF files (*.conf)");
  dialog.setDefaultSuffix("conf");
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  QStringList fileNames;
  if (dialog.exec())
    TimeShiftContainer::instance()->saveToConfigfile(dialog.selectedFiles()[0]);
}
