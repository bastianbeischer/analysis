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
  void processArguments(QStringList);
private slots:
  void setOrAddFileListActionTriggered();
  void saveCanvasActionTriggered();
  void saveAllCanvasActionTriggered();
  void saveForPostAnalysisActionTriggered();
  void listWidgetItemChanged(QListWidgetItem*);
  void listWidgetCurrentRowChanged(int);
  void toggleSelectionButtonClicked();
  void toggleGridButtonClicked();
  void showButtonsClicked();
  void analyzeButtonClicked();
  void firstOrLastEventChanged(int = 0);
  void numberOfEventsChanged(int);
  void changeControlWidgetsStatus(bool);
private:
  void setupPlots();
  void setupAnalysis(Track::Type& type, Corrections::Flags& flags);
  void startAnalysis();
  void removeListWidgetItem(int);
  void closeEvent(QCloseEvent*);

  Ui_mainWindow m_ui;
  QVector<unsigned int> m_activePlots;
  QVector<QCheckBox*> m_topicCheckBoxes;
  QVector<QWidget*> m_controlWidgets;
};

#endif
