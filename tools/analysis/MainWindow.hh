#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"

#include "Track.hh"
#include "Corrections.hh"
#include "ParticleFilter.hh"
#include "MCFilter.hh"
#include "EventReader.hh"
#include "RootPlot.hh"
#include "CutFilter.hh"

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QList>
#include <QTime>
#include <QTimer>

class QTabBar;
class DataChain;
class Plotter;

class MainWindow : public QMainWindow {
  Q_OBJECT
  public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void processArguments(QStringList);
private slots:
  void setOrAddFileListDialogActionTriggered();
  void saveCanvasDialogActionTriggered();
  void saveAllCanvasActionTriggered();
  void saveAllCanvasDialogActionTriggered();
  void saveForPostAnalysisActionTriggered();
  void saveForPostAnalysisDialogActionTriggered();
  void listWidgetItemChanged(QListWidgetItem*);
  void listWidgetCurrentRowChanged(int);
  void selectAllButtonClicked();
  void selectTrackerButtonClicked();
  void selectTrdButtonClicked();
  void selectTofButtonClicked();
  void showButtonsClicked();
  void analyzeButtonClicked();
  void saveButtonsClicked();
  void firstOrLastEventChanged(int = 0);
  void numberOfEventsChanged(int);
  void toggleControlWidgetsStatus();
  void checkBoxChanged();
  void checkSelectAll();
  void plotterTitleChanged(const QString&);
  void plotterPositionChanged(double, double);
  void drawOptionComboBoxCurrentIndexChanged(int);
  void update();
  void changeAspectRatioTriggered();
private:
  void setupPlots();
  void setupAnalysis(Track::Type&, Corrections::Flags&, ParticleFilter::Types&, CutFilter&, MCFilter::Types&);
  void startAnalysis();
  void changeTopicGroupStatus(QVector<QCheckBox*>&);
  void removeListWidgetItem(int);
  void closeEvent(QCloseEvent*);
  void setDrawOptionComboBox();

  Ui_mainWindow m_ui;
  QVector<EventProcessor*> m_processors;
  EventReader* m_reader;
  QString m_topLevelPath;
  QVector<unsigned int> m_activePlots;
  QVector<QCheckBox*> m_topicCheckBoxes;
  QVector<QCheckBox*> m_tofCheckBoxes;
  QVector<QCheckBox*> m_trdCheckBoxes;
  QVector<QCheckBox*> m_trackerCheckBoxes;
  QVector<QWidget*> m_controlWidgets;
  QVector<RootPlot::DrawOption> m_drawOptions;
  bool m_inhibitDraw;
  QTime m_time;
  QTimer m_updateTimer;
};

#endif
