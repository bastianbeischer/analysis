#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

#include <QMainWindow>

class TCanvas;
class TFile;
class PostAnalysisPlot;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void setAnalysisFile(const QString&);
private slots:
  void savePlotsButtonClicked();
  void saveAllPlotsButtonClicked();
  void canvasListWidgetCurrentRowChanged(int);
  void plotListWidgetCurrentRowChanged(int);
private:
  void setupAnalysis();
  TCanvas* addCanvas(TFile*, const char* name);
  void addPlot(PostAnalysisPlot*);
  QString m_analysisFile;
  QVector<TCanvas*> m_canvases;
  QVector<PostAnalysisPlot*> m_plots;
  Ui_mainWindow m_ui;
};

#endif
