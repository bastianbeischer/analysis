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
#include "Enums.hh"
#include "AnalysisSetting.hh"

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QVector>
#include <QList>
#include <QTime>
#include <QTimer>

class QTabBar;
class DataChain;
class Plotter;
class TopicSelector;
class CutSelector;

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
  void loadSettingActionTriggered();
  void saveSettingActionTriggered();
  void listWidgetItemChanged(QListWidgetItem*);
  void listWidgetCurrentRowChanged(int);
  void changeTopicGroup();
  void showTopic(Enums::AnalysisTopic);
  void hideTopic(Enums::AnalysisTopic);
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
  void setupTopicSelectors();
  void setupCorrectionsCheckBoxes();
  void setupFilterCheckBoxes();
  void setupCutSelectors();
  void setupViewActions();
  void setupPlots();
  //void setupAnalysis(Enums::TrackType&, Enums::Corrections&, Enums::Particles& particleFilter, CutFilter&, Enums::Particles& mcFilter);
  void updateAnalysisSetting();
  void startAnalysis();
  void removeListWidgetItem(int);
  void closeEvent(QCloseEvent*);
  void setDrawOptionComboBox();

  Ui_mainWindow m_ui;
  QVector<EventProcessor*> m_processors;
  EventReader* m_reader;
  AnalysisSetting m_analysisSetting;
  QString m_topLevelPath;
  QVector<unsigned int> m_activePlots;
  QVector<TopicSelector*> m_topicSelectors;
  QVector<TopicSelector*> m_trackerSelectors;
  QVector<TopicSelector*> m_trdSelectors;
  QVector<TopicSelector*> m_tofSelectors;
  
  QVector<QWidget*> m_controlWidgets;
  QVector<QCheckBox*> m_correctionCheckBoxes;
  QVector<QCheckBox*> m_particleFilterCheckBoxes;
  QVector<QCheckBox*> m_mcFilterCheckBoxes;
  QVector<CutSelector*> m_cutSelectors;
  QVector<RootPlot::DrawOption> m_drawOptions;
  bool m_inhibitDraw;
  QTime m_time;
  QTimer m_updateTimer;
  QFileDialog::Options m_dialogOptions;
};

#endif
