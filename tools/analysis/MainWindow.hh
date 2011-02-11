#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

#include "Track.hh"
#include "Corrections.hh"

#include <QMainWindow>
#include <QString>

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
  void selectAllButtonClicked();
  void selectTrackerButtonClicked();
  void selectTrdButtonClicked();
  void selectTofButtonClicked();
  void gridCheckBoxChanged(int);
  void logXCheckBoxChanged(int);
  void logYCheckBoxChanged(int);
  void showButtonsClicked();
  void analyzeButtonClicked();
  void saveButtonsClicked();
  void firstOrLastEventChanged(int = 0);
  void numberOfEventsChanged(int);
  void toggleControlWidgetsStatus();
  void checkBoxChanged();
  void checkSelectAll();
private:
  void setupPlots();
  void setupAnalysis(Track::Type& type, Corrections::Flags& flags);
  void startAnalysis();
  void changeTopicGroupStatus(QVector<QCheckBox*>&);
  void removeListWidgetItem(int);
  void closeEvent(QCloseEvent*);

  Ui_mainWindow m_ui;
  QString m_topLevelPath;
  QVector<unsigned int> m_activePlots;
  QVector<QCheckBox*> m_topicCheckBoxes;
  QVector<QCheckBox*> m_tofCheckBoxes;
  QVector<QCheckBox*> m_trdCheckBoxes;
  QVector<QCheckBox*> m_trackerCheckBoxes;
  QVector<QWidget*> m_controlWidgets;
  bool m_inhibitDraw;
};

#endif
