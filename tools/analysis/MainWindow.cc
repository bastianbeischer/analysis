#include "MainWindow.hh"

#include "Analysis.hh"
#include "Plotter.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "AnalysisPlot.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "Corrections.hh"
#include "ParticleFilter.hh"
#include "AnalysisProcessor.hh"
#include "EventReader.hh"
#include "Helpers.hh"
#include "RootPlot.hh"
#include "H1DPlot.hh"
#include "H2DPlot.hh"
#include "GraphPlot.hh"

#include "RootQtWidget.hh"
#include "TopicSelector.hh"
#include "CutSelector.hh"
#include "Caption.hh"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(Analysis* analysis, QWidget* parent)
  : QMainWindow(parent)
  , m_analysisSetting()
  , m_analysis(analysis)
  , m_analysisPath(Helpers::analysisPath())
  , m_activePlots()
  , m_drawOptions()
  , m_inhibitDraw(false)
  , m_time()
  , m_updateTimer()
  , m_dialogOptions(QFileDialog::DontUseNativeDialog)
{
  m_ui.setupUi(this);

  setupTopicSelectors();
  setupCorrectionsCheckBoxes();
  setupFilterCheckBoxes();
  setupCutSelectors();
  setupViewActions();

  for (QMap<Enums::TrackType, QString>::ConstIterator it = Enums::trackTypeBegin(); it != Enums::trackTypeEnd(); ++it)
    m_ui.trackComboBox->addItem(it.value());
  m_ui.trackComboBox->setCurrentIndex(m_ui.trackComboBox->findText(Enums::label(Enums::CenteredBrokenLine)));

  m_controlWidgets
    << m_ui.selectAllButton << m_ui.selectTrackerButton << m_ui.selectTrdButton << m_ui.selectTofButton
    << m_ui.trackComboBox << m_ui.firstEventSpinBox << m_ui.lastEventSpinBox << m_ui.numberOfThreadsSpinBox;

  connect(m_analysis, SIGNAL(started()), this, SLOT(toggleControlWidgetsStatus()));
  connect(m_analysis, SIGNAL(started()), &m_updateTimer, SLOT(start()));
  connect(m_analysis, SIGNAL(finished()), this, SLOT(toggleControlWidgetsStatus()));
  connect(m_analysis, SIGNAL(finished()), &m_updateTimer, SLOT(stop()));
  connect(m_analysis, SIGNAL(chainChanged(int)), this, SLOT(numberOfEventsChanged(int)));

  connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(update()));

  connect(m_ui.analyzeButton, SIGNAL(clicked()), this, SLOT(analyzeButtonClicked()));
  connect(m_ui.savePdfButton, SIGNAL(clicked()), this, SLOT(saveButtonsClicked()));
  connect(m_ui.saveEpsButton, SIGNAL(clicked()), this, SLOT(saveButtonsClicked()));
  connect(m_ui.savePngButton, SIGNAL(clicked()), this, SLOT(saveButtonsClicked()));
  connect(m_ui.saveRootButton, SIGNAL(clicked()), this, SLOT(saveButtonsClicked()));
  
  connect(m_ui.saveCanvasDialogAction, SIGNAL(triggered()), this, SLOT(saveCanvasDialogActionTriggered()));
  connect(m_ui.saveAllCanvasesDialogAction, SIGNAL(triggered()), this, SLOT(saveAllCanvasDialogActionTriggered()));
  connect(m_ui.saveForPostAnalysisAction, SIGNAL(triggered()), this, SLOT(saveForPostAnalysisActionTriggered()));
  connect(m_ui.saveAllCanvasesAction, SIGNAL(triggered()), this, SLOT(saveAllCanvasActionTriggered()));
  connect(m_ui.saveForPostAnalysisDialogAction, SIGNAL(triggered()), this, SLOT(saveForPostAnalysisDialogActionTriggered()));
  connect(m_ui.loadSettingAction, SIGNAL(triggered()), this, SLOT(loadSettingActionTriggered()));
  connect(m_ui.saveSettingAction, SIGNAL(triggered()), this, SLOT(saveSettingActionTriggered()));

  connect(m_ui.setFileListDialogAction, SIGNAL(triggered()), this, SLOT(setOrAddFileListDialogActionTriggered()));
  connect(m_ui.addFileListDialogAction, SIGNAL(triggered()), this, SLOT(setOrAddFileListDialogActionTriggered()));
  connect(m_ui.quitAction, SIGNAL(triggered()), this, SLOT(close()));

  connect(m_ui.plotter, SIGNAL(positionChanged(double, double)), this, SLOT(plotterPositionChanged(double, double)));
  connect(m_ui.firstEventSpinBox, SIGNAL(valueChanged(int)), this, SLOT(firstOrLastEventChanged(int)));
  connect(m_ui.lastEventSpinBox, SIGNAL(valueChanged(int)), this, SLOT(firstOrLastEventChanged(int)));

  connect(m_ui.selectAllButton, SIGNAL(clicked()), this, SLOT(changeTopicGroup()));
  connect(m_ui.selectTrackerButton, SIGNAL(clicked()), this, SLOT(changeTopicGroup()));
  connect(m_ui.selectTrdButton, SIGNAL(clicked()), this, SLOT(changeTopicGroup()));
  connect(m_ui.selectTofButton, SIGNAL(clicked()), this, SLOT(changeTopicGroup()));
  connect(m_ui.gridCheckBox, SIGNAL(stateChanged(int)), m_ui.plotter->rootWidget(), SLOT(setGrid(int)));
  connect(m_ui.logXCheckBox, SIGNAL(stateChanged(int)), m_ui.plotter->rootWidget(), SLOT(setLogX(int)));
  connect(m_ui.logYCheckBox, SIGNAL(stateChanged(int)), m_ui.plotter->rootWidget(), SLOT(setLogY(int)));
  connect(m_ui.logZCheckBox, SIGNAL(stateChanged(int)), m_ui.plotter->rootWidget(), SLOT(setLogZ(int)));
  connect(m_ui.listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listWidgetItemChanged(QListWidgetItem*)));
  connect(m_ui.listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(listWidgetCurrentRowChanged(int)));

  m_updateTimer.setInterval(50);
  m_ui.numberOfThreadsSpinBox->setValue(QThread::idealThreadCount());
  numberOfEventsChanged(m_analysis->numberOfEvents());
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupTopicSelectors()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(new Caption("tracker"));
  foreach (Enums::AnalysisTopic topic, AnalysisTopic::trackerTopics()) {
    if (!AnalysisTopic::otherTopics().contains(topic)) {
      TopicSelector* selector = new TopicSelector(topic);
      m_topicSelectors.append(selector);
      m_trackerSelectors.append(selector);
      layout->addWidget(selector);
    }
  }
  layout->addWidget(new Caption("TRD"));
  foreach (Enums::AnalysisTopic topic, AnalysisTopic::trdTopics()) {
    if (!AnalysisTopic::otherTopics().contains(topic)) {
      TopicSelector* selector = new TopicSelector(topic);
      m_topicSelectors.append(selector);
      m_trdSelectors.append(selector);
      layout->addWidget(selector);
    }
  }
  layout->addWidget(new Caption("TOF"));
  foreach (Enums::AnalysisTopic topic, AnalysisTopic::tofTopics()) {
    if (!AnalysisTopic::otherTopics().contains(topic)) {
      TopicSelector* selector = new TopicSelector(topic);
      m_topicSelectors.append(selector);
      m_tofSelectors.append(selector);
      layout->addWidget(selector);
    }
  }
  layout->addWidget(new Caption("other"));
  foreach (Enums::AnalysisTopic topic, AnalysisTopic::otherTopics()) {
    TopicSelector* selector = new TopicSelector(topic);
    m_topicSelectors.append(selector);
    layout->addWidget(selector);
  }
  foreach (TopicSelector* selector, m_topicSelectors) {
    connect(selector, SIGNAL(show(Enums::AnalysisTopic)), this, SLOT(showTopic(Enums::AnalysisTopic)));
    connect(selector, SIGNAL(hide(Enums::AnalysisTopic)), this, SLOT(hideTopic(Enums::AnalysisTopic)));
  }
  m_ui.scrollAreaWidgetContents->setLayout(layout);
}

void MainWindow::setupCorrectionsCheckBoxes()
{
  QVBoxLayout* layout = new QVBoxLayout;
  for (QMap<Enums::Correction, QString>::ConstIterator it = Enums::correctionBegin(); it != Enums::correctionEnd(); ++it) {
    if (it.key() == Enums::NoCorrection)
      continue;
    QCheckBox* checkBox = new QCheckBox(it.value());
    checkBox->setCheckState(Qt::Checked);
    layout->addWidget(checkBox);
    m_controlWidgets.append(checkBox);
    m_correctionCheckBoxes.append(checkBox);
  }
  m_ui.correctionsTab->setLayout(layout);
}

void MainWindow::setupFilterCheckBoxes()
{
  Enums::Particles particles = Enums::NoParticle;
  int row;
  
  QGridLayout* layout = new QGridLayout;
  particles = Enums::Proton | Enums::Helium | Enums::Electron | Enums::Positron | Enums::Muon | Enums::AntiMuon;
  layout->addWidget(new QLabel("data:"), 0, 0);
  row = 1;
  for (QMap<Enums::Particle, QString>::ConstIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it) {
    if (it.key() != Enums::NoParticle && (it.key() & particles) == it.key()) {
      QCheckBox* checkBox = new QCheckBox(it.value());
      checkBox->setCheckState(Qt::Checked);
      layout->addWidget(checkBox, row, 0);
      m_controlWidgets.append(checkBox);
      m_particleFilterCheckBoxes.append(checkBox);
      ++row;
    }
  }
  particles = Enums::Proton | Enums::AntiProton | Enums::Helium | Enums::Electron | Enums::Positron | Enums::Muon
    | Enums::AntiMuon | Enums::PiPlus | Enums::PiMinus | Enums::Photon;
  layout->addWidget(new QLabel("mc:"), 0, 1);
  row = 1;
  for (QMap<Enums::Particle, QString>::ConstIterator it = Enums::particleBegin(); it != Enums::particleEnd(); ++it) {
    if (it.key() != Enums::NoParticle && (it.key() & particles) == it.key()) {
      QCheckBox* checkBox = new QCheckBox(it.value());
      checkBox->setCheckState(Qt::Checked);
      layout->addWidget(checkBox, row, 1);
      m_controlWidgets.append(checkBox);
      m_mcFilterCheckBoxes.append(checkBox);
      ++row;
    }
  }
  m_ui.filterTab->setLayout(layout);
}

void MainWindow::setupCutSelectors()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  for (QMap<Enums::Cut, QString>::ConstIterator it = Enums::cutBegin(); it != Enums::cutEnd(); ++it) {
    CutSelector* selector = new CutSelector(it.key());
    layout->addWidget(selector);
    m_cutSelectors.append(selector);
    m_controlWidgets.append(selector);
    if (it.key() == Enums::CherenkovCut)
      selector->setMinimum(200);
  }
  m_ui.cutTab->setLayout(layout);
}

void MainWindow::setupViewActions()
{
  m_ui.viewAutoAction->setChecked(true);
  QVector<QAction*> actions = QVector<QAction*>() << m_ui.viewAutoAction << m_ui.view11Action << m_ui.view34Action
    << m_ui.view54Action << m_ui.view43Action << m_ui.view1610Action << m_ui.view169Action;
  QActionGroup* aspectRatioGroup = new QActionGroup(this);
  foreach (QAction* action, actions) {
    aspectRatioGroup->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(changeAspectRatioTriggered()));
  }
}

void MainWindow::showTopic(Enums::AnalysisTopic topic)
{
  QVector<unsigned int> plotIndices = m_analysis->plotIndices(topic);
  for (int i = 0; i < plotIndices.size(); ++i) {
    m_activePlots.append(plotIndices[i]);
    QListWidgetItem* item = new QListWidgetItem(m_analysis->plot(plotIndices[i])->title());
    item->setCheckState(Qt::Checked);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    m_ui.listWidget->addItem(item);
  }
}

void MainWindow::hideTopic(Enums::AnalysisTopic topic)
{
  QVector<int> matchingItems;
  for (int i = m_ui.listWidget->count() - 1; i >= 0; --i) {
    if (m_analysis->plot(m_activePlots[i])->topic() == topic)
      matchingItems << i;
  }
  if (matchingItems.size() > 0)
    m_inhibitDraw = true;
  foreach(int i, matchingItems) {
    removeListWidgetItem(i);
    if (i == matchingItems.last()) {
      m_inhibitDraw = false;
      listWidgetCurrentRowChanged(m_ui.listWidget->currentRow());
    }
  }
}

void MainWindow::removeListWidgetItem(int i)
{
  delete m_ui.listWidget->takeItem(i);
  m_activePlots.remove(i);
}

void MainWindow::listWidgetItemChanged(QListWidgetItem* item)
{
  if (item && item->checkState() == Qt::Unchecked)
    removeListWidgetItem(m_ui.listWidget->row(item));
}

void MainWindow::listWidgetCurrentRowChanged(int i)
{
  disconnect(m_ui.drawOptionComboBox, 0, this, 0);
  m_ui.drawOptionComboBox->setEnabled(false);
  m_ui.drawOptionComboBox->clear();
  m_drawOptions.clear();
  if (i < 0 || m_activePlots.size() == 0) {
    m_ui.plotter->setPlot(0);
    m_ui.titleLabel->setText("");
    return;
  }
  if (!m_inhibitDraw) {
    AnalysisPlot* plot = m_analysis->plot(m_activePlots[i]);
    m_ui.plotter->setPlot(plot);
    m_ui.titleLabel->setText(plot->title());
    switch (plot->type()) {
      case RootPlot::H1DPlot:
        foreach(const RootPlot::DrawOption& option, H1DPlot::drawOptions()) {
          m_drawOptions.append(option);
          m_ui.drawOptionComboBox->addItem(RootPlot::drawOption(option));
        }
        break;
      case RootPlot::H2DPlot:
        foreach(const RootPlot::DrawOption& option, H2DPlot::drawOptions()) {
          m_drawOptions.append(option);
          m_ui.drawOptionComboBox->addItem(RootPlot::drawOption(option));
        }
        break;
      case RootPlot::GraphPlot:
        foreach(const RootPlot::DrawOption& option, GraphPlot::drawOptions()) {
          m_drawOptions.append(option);
          m_ui.drawOptionComboBox->addItem(RootPlot::drawOption(option));
        }
        break;
      default: break;
    }
    setDrawOptionComboBox();
  }
}

void MainWindow::setDrawOptionComboBox()
{
  for (int i = 0; i < m_ui.drawOptionComboBox->count(); ++i) {
    if (m_drawOptions[i] == m_ui.plotter->plot()->drawOption()) {
      m_ui.drawOptionComboBox->setCurrentIndex(i);
      break;
    }
  }
  m_ui.drawOptionComboBox->setEnabled(true);
  connect(m_ui.drawOptionComboBox, SIGNAL(currentIndexChanged(int)),
    this, SLOT(drawOptionComboBoxCurrentIndexChanged(int)));
}

void MainWindow::updateAnalysisSetting()
{
  m_analysisSetting.clear();

  m_analysisSetting.firstEvent = m_ui.firstEventSpinBox->value();
  m_analysisSetting.lastEvent = m_ui.lastEventSpinBox->value();
  m_analysisSetting.numberOfThreads = m_ui.numberOfThreadsSpinBox->value();

  m_analysisSetting.analysisTopics = Enums::NoTopic;
  foreach (TopicSelector* selector, m_topicSelectors)
    if (selector->checked())
      m_analysisSetting.analysisTopics|= selector->topic();

  m_analysisSetting.trackType = Enums::trackType(m_ui.trackComboBox->currentText());

  m_analysisSetting.corrections = Enums::NoCorrection;
  foreach (QCheckBox* checkBox, m_correctionCheckBoxes)
    if (checkBox->isChecked())
      m_analysisSetting.corrections|= Enums::correction(checkBox->text());

  m_analysisSetting.particleFilter = Enums::NoParticle;
  foreach (QCheckBox* checkBox, m_particleFilterCheckBoxes)
    if (checkBox->isChecked())
      m_analysisSetting.particleFilter|= Enums::particle(checkBox->text());

  m_analysisSetting.mcParticleFilter = Enums::NoParticle;
  foreach (QCheckBox* checkBox, m_mcFilterCheckBoxes)
    if (checkBox->isChecked())
      m_analysisSetting.mcParticleFilter|= Enums::particle(checkBox->text());
  
  foreach (CutSelector* selector, m_cutSelectors)
    if (selector->checked())
      m_analysisSetting.cutFilter.addCut(selector->cut());
}

void MainWindow::analyzeButtonClicked()
{
  if (m_ui.analyzeButton->text() == "&start") {
    updateAnalysisSetting();
    m_ui.plotter->setPlot(0);
    m_activePlots.clear();
    m_ui.listWidget->clear();
    m_analysis->start(m_analysisSetting);
  } else {
    m_analysis->stop();
    m_ui.analyzeButton->setText("&stopping...");
  }
}

void MainWindow::setOrAddFileListDialogActionTriggered()
{
  QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more file lists to open", "", "*.txt;;*.*;;*", 0, m_dialogOptions);
  if (sender() == m_ui.setFileListDialogAction)
    m_analysis->clearFileList();
  foreach(QString file, files)
    m_analysis->addFileList(file);
}

void MainWindow::saveButtonsClicked()
{
  QString fileName = m_ui.titleLabel->text();
  fileName.remove(QChar('/'));
  fileName.remove(QChar(':'));
  fileName.prepend(m_analysisPath + "/plots/");
  QPushButton* b = static_cast<QPushButton*>(sender());
  if (b == m_ui.savePdfButton) {
    fileName+= ".pdf";
  } else if (b == m_ui.saveEpsButton) {
    fileName+= ".eps";
  } else if (b == m_ui.savePngButton) {
    fileName+= ".png";
  } else if (b == m_ui.saveRootButton) {
    fileName+= ".root";
  } else {
    return;
  }
  QDir dir(m_analysisPath);
  if (!dir.exists("plots"))
    dir.mkdir("plots");
  m_ui.plotter->saveCanvas(fileName);
}

void MainWindow::saveCanvasDialogActionTriggered()
{
  QStringList fileFormatEndings;
  fileFormatEndings << "svg" << "pdf" << "eps" << "root" << "png";
  QStringList filters;
  foreach(QString ending, fileFormatEndings) {
    QString description = ending.toUpper();
    filters.append( description + "(*." + ending + ")" );
  }
  QString selectedFilter;
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "All Files(*.*);;" + filters.join(";;"), &selectedFilter, m_dialogOptions);

  if (fileName.isEmpty())
    return;

  // if file name contains an ending, use that. Otherwise use selected filter
  QString fileEnding;
  if (fileName.contains('.')) {
    fileEnding = fileName.split('.').last().toLower();
  } else {
    fileEnding = selectedFilter.split("(").first().toLower();
  }

  // if filter == all, save all endings, otherwise use previously determined ending
  if(fileEnding == "all files"){
    foreach(QString fileFormatEnding, fileFormatEndings)
      m_ui.plotter->saveCanvas(fileName + "." + fileFormatEnding);
  } else {
    if (!fileEnding.startsWith('.'))
      fileEnding.prepend('.');
    if (!fileName.endsWith(fileEnding))
      fileName.append(fileEnding);
    m_ui.plotter->saveCanvas(fileName);
  }
}

void MainWindow::saveAllCanvasActionTriggered()
{
  QDir dir(m_analysisPath);
  if (!dir.exists("plots"))
    dir.mkdir("plots");
  QStringList fileFormatEndings;
  fileFormatEndings << "svg" << "pdf" << "eps" << "root" << "png";
  for (int i = 0; i < m_ui.listWidget->count(); ++i) {
    m_ui.listWidget->setCurrentRow(i);
    const QString& directoryName = m_analysisPath + "/plots/";
    foreach (QString fileFormatEnding, fileFormatEndings)
    m_ui.plotter->saveCanvas(directoryName + '/' + m_analysis->plot(m_activePlots[i])->title() + "." + fileFormatEnding);
  }
}

void MainWindow::saveAllCanvasDialogActionTriggered()
{
  QStringList fileFormatEndings;
  fileFormatEndings << "svg" << "pdf" << "eps" << "root" << "png";
  QFileDialog dialog(this, "save all canvases displayed", ".");
  dialog.setOptions(m_dialogOptions);
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  if (dialog.exec())
    for (int i = 0; i < m_ui.listWidget->count(); ++i) {
      m_ui.listWidget->setCurrentRow(i);
      QString directoryName = dialog.selectedFiles().first();
      foreach (QString fileFormatEnding, fileFormatEndings)
        m_ui.plotter->saveCanvas(directoryName + '/' + m_analysis->plot(m_activePlots[i])->title() + "." + fileFormatEnding);
    }
}

void MainWindow::saveForPostAnalysisActionTriggered()
{
  QDir dir(m_analysisPath);
  if (!dir.exists("plots"))
    dir.mkdir("plots");
  m_analysis->save(m_analysisPath + "/plots/postAnalysis.root", m_ui.plotter->rootWidget()->GetCanvas());
}

void MainWindow::saveForPostAnalysisDialogActionTriggered()
{
  QString fileEnding; 
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "*.root", &fileEnding, m_dialogOptions);
  if (fileName.isEmpty())
    return;
  fileEnding.remove(0, 1);
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  m_analysis->save(fileName, m_ui.plotter->rootWidget()->GetCanvas());
}

void MainWindow::checkBoxChanged()
{
  checkSelectAll();
}

void MainWindow::changeTopicGroup()
{
  QVector<TopicSelector*> group;
  QObject* button = sender();
  if (button == m_ui.selectAllButton) {
    group = m_topicSelectors;
  } else if (button == m_ui.selectTrackerButton) {
    group = m_trackerSelectors;
  } else if (button == m_ui.selectTrdButton) {
    group = m_trdSelectors;
  } else if (button == m_ui.selectTofButton) {
    group = m_tofSelectors;
  }

  bool groupAlreadyChecked = true;
  foreach(TopicSelector* selector, group)
    groupAlreadyChecked = groupAlreadyChecked && selector->checked();

  bool newState = groupAlreadyChecked ? false : true;
  foreach(TopicSelector* selector, group)
    selector->setChecked(newState);

  checkSelectAll();
}
 
void MainWindow::checkSelectAll()
{
  bool allTopicsChecked = true;
  foreach(TopicSelector* selector, m_topicSelectors)
    allTopicsChecked = allTopicsChecked && selector->checked();

  m_ui.selectAllButton->setText(allTopicsChecked ? "deselect &all" : "select &all");
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  m_ui.plotter->setPlot(0);
  m_analysis->stop();
  event->accept();
}

void MainWindow::firstOrLastEventChanged(int)
{
  double firstEvent = m_ui.firstEventSpinBox->value();
  double lastEvent = m_ui.lastEventSpinBox->value();
  m_ui.lastEventSpinBox->setMinimum(firstEvent);
  m_ui.firstEventSpinBox->setMaximum(lastEvent);
}

void MainWindow::numberOfEventsChanged(int nEvents)
{
  m_ui.firstEventSpinBox->setMinimum(0);
  m_ui.lastEventSpinBox->setMaximum(nEvents-1);
  m_ui.firstEventSpinBox->setValue(0);
  m_ui.lastEventSpinBox->setValue(nEvents-1);
  if (nEvents > 0)
    m_ui.analyzeButton->setEnabled(true);
  firstOrLastEventChanged();
}

void MainWindow::toggleControlWidgetsStatus()
{
  foreach(QWidget* widget, m_controlWidgets)
    widget->setEnabled(!widget->isEnabled());
  foreach(TopicSelector* selector, m_topicSelectors)
    selector->setActive(!selector->isActive());
  if (m_ui.analyzeButton->text() == "&start") {
    m_ui.analyzeButton->setText("&stop");
    m_time.restart();
  } else {
    m_ui.analyzeButton->setText("&start");
    update();
  }
  m_ui.plotter->toggleUpdateTimer();
}

void MainWindow::plotterPositionChanged(double x, double y)
{
  m_ui.positionLabel->setText(QString("%1%2  %3%4")
    .arg(x < 0 ? '-' : '+').arg(qAbs(x), 7, 'f', 3, '0')
    .arg(y < 0 ? '-' : '+').arg(qAbs(y), 7, 'f', 3, '0'));
}

void MainWindow::update()
{
  m_ui.dataChainProgressBar->setValue(m_analysis->progress());
  m_ui.eventQueueProgressBar->setValue(m_analysis->buffer());
  m_ui.timeLabel->setText(QString("%1s").arg(m_time.elapsed()/1000));
}

void MainWindow::drawOptionComboBoxCurrentIndexChanged(int i)
{
  m_ui.plotter->plot()->setDrawOption(m_drawOptions[i]);
}

void MainWindow::changeAspectRatioTriggered()
{
  QObject* s = sender();
  Q_ASSERT(s);
  if (s == m_ui.viewAutoAction) {
    m_ui.plotter->setAspectRatio(-1.);
  } else if (s == m_ui.view11Action) {
    m_ui.plotter->setAspectRatio(1.);
  } else if (s == m_ui.view34Action) {
    m_ui.plotter->setAspectRatio(3./4.);
  } else if (s == m_ui.view54Action) {
    m_ui.plotter->setAspectRatio(5./4.);
  } else if (s == m_ui.view43Action) {
    m_ui.plotter->setAspectRatio(4./3.);
  } else if (s == m_ui.view1610Action) {
    m_ui.plotter->setAspectRatio(16./10.);
  } else if (s == m_ui.view169Action) {
    m_ui.plotter->setAspectRatio(16./9.);
  }
}

void MainWindow::saveSettingActionTriggered()
{
  QString fileEnding;
  QString fileName = QFileDialog::getSaveFileName(this, "save setting", ".", "*.conf", &fileEnding, m_dialogOptions);
  if (fileName.isEmpty())
    return;
  fileEnding.remove(0, 1);
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  updateAnalysisSetting();
  m_analysisSetting.save(fileName);
}

void MainWindow::loadSettingActionTriggered()
{
  QString fileEnding;
  QString fileName = QFileDialog::getOpenFileName(this, "load setting", ".", "*.conf", &fileEnding, m_dialogOptions);
  if (fileName.isEmpty())
    return;
  fileEnding.remove(0, 1);
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  updateAnalysisSetting();
  m_analysisSetting.load(fileName);

  m_ui.firstEventSpinBox->setValue(m_analysisSetting.firstEvent);
  m_ui.lastEventSpinBox->setValue(m_analysisSetting.lastEvent);
  m_ui.numberOfThreadsSpinBox->setValue(m_analysisSetting.numberOfThreads);

  foreach (TopicSelector* selector, m_topicSelectors)
    selector->setChecked(m_analysisSetting.analysisTopics & selector->topic());

  m_ui.trackComboBox->setCurrentIndex(m_ui.trackComboBox->findText(Enums::label(m_analysisSetting.trackType)));

  foreach (QCheckBox* checkBox, m_correctionCheckBoxes)
    if (m_analysisSetting.corrections & Enums::correction(checkBox->text())) {
      checkBox->setCheckState(Qt::Checked);
    } else {
      checkBox->setCheckState(Qt::Unchecked);
    }

  foreach (QCheckBox* checkBox, m_particleFilterCheckBoxes)
    if (m_analysisSetting.particleFilter & Enums::particle(checkBox->text())) {
      checkBox->setCheckState(Qt::Checked);
    } else {
      checkBox->setCheckState(Qt::Unchecked);
    }

  foreach (QCheckBox* checkBox, m_mcFilterCheckBoxes)
    if (m_analysisSetting.mcParticleFilter & Enums::particle(checkBox->text())) {
      checkBox->setCheckState(Qt::Checked);
    } else {
      checkBox->setCheckState(Qt::Unchecked);
    }

  foreach (CutSelector* selector, m_cutSelectors) {
    foreach (Cut cut, m_analysisSetting.cutFilter.cuts()) {
      if (selector->cut().type() == cut.type()) {
        selector->setChecked(true);
        if (cut.minIsSet()) selector->setMinimum(cut.min());
        if (cut.maxIsSet()) selector->setMaximum(cut.max());
      }
    }
  }
}
