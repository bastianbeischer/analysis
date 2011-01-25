#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

#include "Track.hh"
#include "Corrections.hh"

#include <QMainWindow>

class QTabBar;

class DataChain;

class MainWindow : public QMainWindow {
  Q_OBJECT
  public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void setFileList(const QString&);
  void addFileList(const QString&);
private slots:
  void setOrAddFileListButtonClicked();
  void analyzeButtonClicked();
  void saveCanvasButtonClicked();
  void saveAllCanvasButtonClicked();
  void saveForPostAnalysisButtonClicked();
  void chooseAllButtonClicked();
  void listWidgetItemChanged(QListWidgetItem*);
  void listWidgetCurrentRowChanged(int);
  void showButtonsClicked();
private:
  void setupPlots();
  void setupAnalysis(Track::Type& type, Corrections::Flags& flags);
  void startAnalysis();
  void removeListWidgetItem(int);
  void closeEvent(QCloseEvent*);

  Ui_mainWindow m_ui;
  QVector<unsigned int> m_activePlots;
};

#endif
