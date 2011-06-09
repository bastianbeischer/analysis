#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "ChannelTimeShiftHistogram.hh"
#include "BarTimeShiftHistogram.hh"
#include "RootStyle.hh"
#include "PostAnalysisCanvas.hh"
#include "TimeShiftContainer.hh"

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

  QVector<PostAnalysisCanvas*> canvases;
  canvases.append(addCanvas(&file, "bar shift 0x8000 0x8010 0x8020 0x8030 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8000 0x8010 0x8024 0x8034 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8000 0x8010 0x8028 0x8038 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8000 0x8010 0x802c 0x803c canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8004 0x8014 0x8020 0x8030 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8004 0x8014 0x8024 0x8034 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8004 0x8014 0x8028 0x8038 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8004 0x8014 0x802c 0x803c canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8008 0x8018 0x8020 0x8030 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8008 0x8018 0x8024 0x8034 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8008 0x8018 0x8028 0x8038 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x8008 0x8018 0x802c 0x803c canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x800c 0x801c 0x8020 0x8030 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x800c 0x801c 0x8024 0x8034 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x800c 0x801c 0x8028 0x8038 canvas"));
  canvases.append(addCanvas(&file, "bar shift 0x800c 0x801c 0x802c 0x803c canvas"));
  addPlot(new BarTimeShiftHistogram(canvases));
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
