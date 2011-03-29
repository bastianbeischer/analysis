#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "ChannelTimeShiftHistogram.hh"
#include "BarTimeShiftHistogram.hh"
#include "RootStyle.hh"

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
  TCanvas* canvas = 0;
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();
  canvas = addCanvas(&file, "time shift 0x8000 0x8010 canvas");
  for (int ch = 0; ch < 8; ++ch)
    addPlot(new ChannelTimeShiftHistogram(canvas, ch));
  canvas = addCanvas(&file, "time shift 0x8004 0x8014 canvas");
  for (int ch = 0; ch < 8; ++ch)
    addPlot(new ChannelTimeShiftHistogram(canvas, ch));
  canvas = addCanvas(&file, "time shift 0x8008 0x8018 canvas");
  for (int ch = 0; ch < 8; ++ch)
    addPlot(new ChannelTimeShiftHistogram(canvas, ch));
  canvas = addCanvas(&file, "time shift 0x800c 0x801c canvas");
  for (int ch = 0; ch < 8; ++ch)
    addPlot(new ChannelTimeShiftHistogram(canvas, ch));
  canvas = addCanvas(&file, "time shift 0x8020 0x8030 canvas");
  for (int ch = 0; ch < 8; ++ch)
    addPlot(new ChannelTimeShiftHistogram(canvas, ch));
  canvas = addCanvas(&file, "time shift 0x8024 0x8034 canvas");
  for (int ch = 0; ch < 8; ++ch)
    addPlot(new ChannelTimeShiftHistogram(canvas, ch));
  canvas = addCanvas(&file, "time shift 0x8028 0x8038 canvas");
  for (int ch = 0; ch < 8; ++ch)
    addPlot(new ChannelTimeShiftHistogram(canvas, ch));
  canvas = addCanvas(&file, "time shift 0x802c 0x803c canvas");
  for (int ch = 0; ch < 8; ++ch)
    addPlot(new ChannelTimeShiftHistogram(canvas, ch));
  QVector<TCanvas*> canvases;
  canvases.append(addCanvas(&file, "time resolution 0x8000 0x8010 0x8020 0x8030 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8000 0x8010 0x8024 0x8034 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8000 0x8010 0x8028 0x8038 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8000 0x8010 0x802c 0x803c canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8004 0x8014 0x8020 0x8030 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8004 0x8014 0x8024 0x8034 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8004 0x8014 0x8028 0x8038 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8004 0x8014 0x802c 0x803c canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8008 0x8018 0x8020 0x8030 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8008 0x8018 0x8024 0x8034 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8008 0x8018 0x8028 0x8038 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x8008 0x8018 0x802c 0x803c canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x800c 0x801c 0x8020 0x8030 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x800c 0x801c 0x8024 0x8034 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x800c 0x801c 0x8028 0x8038 canvas"));
  canvases.append(addCanvas(&file, "time resolution 0x800c 0x801c 0x802c 0x803c canvas"));
  addPlot(new BarTimeShiftHistogram(canvases));
  file.Close();
}
