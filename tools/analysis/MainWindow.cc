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
#include "ParticleProperties.hh"

#include "RootQtWidget.hh"
#include "TopicSelector.hh"
#include "CutSelector.hh"
#include "Caption.hh"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(Analysis* analysis, bool batch, QWidget* parent)
  : QMainWindow(parent)
  , m_ui()
  , m_batch(batch)
  , m_analysis(analysis)
  , m_analysisSetting(analysis->analysisSetting())
  , m_analysisPath(Helpers::analysisPath())
  , m_activePlots()
  , m_topicSelectors()
  , m_trackerSelectors()
  , m_trdSelectors()
  , m_tofSelectors()
  , m_controlWidgets()
  , m_correctionSelector(0)
  , m_reconstructionSelector(0)
  , m_particleSelector(0)
  , m_likelihoodSelector(0)
  , m_particleFilterSelector(0)
  , m_mcParticleFilterSelector(0)
  , m_cutSelectors()
  , m_drawOptions()
  , m_inhibitDraw(false)
  , m_time()
  , m_updateTimer()
  , m_dialogOptions(QFileDialog::DontUseNativeDialog)
{
  m_ui.setupUi(this);

  setupTopicSelectors();
  setupSettingsTab();
  setupCutSelectors();
  setupViewActions();

  for (Enums::TrackTypeIterator it = Enums::trackTypeBegin(); it != Enums::trackTypeEnd(); ++it)
    m_ui.trackComboBox->addItem(it.value());

  for (Enums::ReconstructionMethodIterator it = Enums::reconstructionMethodBegin(); it != Enums::reconstructionMethodEnd(); ++it)
    m_ui.reconstructionMethodComboBox->addItem(it.value());

  m_controlWidgets
    << m_ui.selectAllButton << m_ui.selectTrackerButton << m_ui.selectTrdButton << m_ui.selectTofButton
    << m_ui.trackComboBox << m_ui.reconstructionMethodComboBox << m_ui.firstEventSpinBox
    << m_ui.lastEventSpinBox << m_ui.numberOfThreadsSpinBox;

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

  m_updateTimer.setInterval(m_batch ? 1000 : 50);

  analysisSettingToGui();

  if (batch) {
    analyzeButtonClicked();
  } else {
    showMaximized();
  }
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

void MainWindow::setupSettingsTab()
{
  QVBoxLayout* layout = new QVBoxLayout;

  m_correctionSelector = new CorrectionsSelector("corrections", Enums::correctionBegin() + 1, Enums::correctionEnd());
  m_correctionSelector->selectAll();
  m_controlWidgets.append(m_correctionSelector);
  layout->addWidget(m_correctionSelector);

  m_reconstructionSelector = new ReconstructionMethodSelector("reconstruction method", Enums::reconstructionMethodBegin(), Enums::reconstructionMethodEnd());
  m_reconstructionSelector->selectAll();
  m_controlWidgets.append(m_reconstructionSelector);
  layout->addWidget(m_reconstructionSelector);

  m_particleSelector = new ParticleSelector("likelihood particles", Enums::particleBegin() + 1, Enums::particleEnd());
  m_particleSelector->selectAll();
  m_controlWidgets.append(m_particleSelector);
  layout->addWidget(m_particleSelector);

  m_likelihoodSelector = new LikelihoodVariableSelector("likelihood variables", Enums::likelihoodVariableBegin(), Enums::likelihoodVariableEnd());
  m_controlWidgets.append(m_likelihoodSelector);
  layout->addWidget(m_likelihoodSelector);

  Enums::Particles particles = Enums::NoParticle;
  particles = Enums::Proton | Enums::Helium | Enums::Electron | Enums::Positron | Enums::Muon | Enums::AntiMuon;
  m_particleFilterSelector = new ParticleSelector("data particle filter",  Enums::particleBegin() + 1, Enums::particleEnd(), particles);
  m_particleFilterSelector->selectAll();
  m_controlWidgets.append(m_particleFilterSelector);
  layout->addWidget(m_particleFilterSelector);

  particles = Enums::Proton | Enums::AntiProton | Enums::Helium | Enums::Electron | Enums::Positron | Enums::Muon
  | Enums::AntiMuon | Enums::PiPlus | Enums::PiMinus | Enums::Pi0 | Enums::Photon;
  m_mcParticleFilterSelector = new ParticleSelector("mc particle filter",  Enums::particleBegin() + 1, Enums::particleEnd(), particles);
  m_mcParticleFilterSelector->selectAll();
  m_controlWidgets.append(m_mcParticleFilterSelector);
  layout->addWidget(m_mcParticleFilterSelector);

  m_ui.settingsTab->setLayout(layout);
}

void MainWindow::setupCutSelectors()
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  for (Enums::CutIterator it = Enums::cutBegin(); it != Enums::cutEnd(); ++it) {
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

void MainWindow::guiToAnalysisSetting()
{
  m_analysisSetting.clear();

  m_analysisSetting.setFirstEvent(m_ui.firstEventSpinBox->value());
  m_analysisSetting.setLastEvent(m_ui.lastEventSpinBox->value());
  m_analysisSetting.setNumberOfThreads(m_ui.numberOfThreadsSpinBox->value());

  Enums::AnalysisTopics topics = Enums::NoTopic;
  foreach (TopicSelector* selector, m_topicSelectors)
    if (selector->checked())
      topics|= selector->topic();
  m_analysisSetting.setAnalysisTopics(topics);

  m_analysisSetting.setTrackType(Enums::trackType(m_ui.trackComboBox->currentText()));

  m_analysisSetting.setReconstructionMethod(Enums::reconstructionMethod(m_ui.reconstructionMethodComboBox->currentText()));

  Enums::Corrections corrections = m_correctionSelector->selectedElements();
  m_analysisSetting.setCorrections(corrections);

  Enums::ReconstructionMethods methods = m_reconstructionSelector->selectedElements();
  m_analysisSetting.setReconstructionMethods(methods);

  Enums::Particles particles = m_particleSelector->selectedElements();
  m_analysisSetting.setParticles(particles);

  Enums::LikelihoodVariables likelihoodVariables = m_likelihoodSelector->selectedElements();
  m_analysisSetting.setLikelihoodVariables(likelihoodVariables);

  Enums::Particles particleFilter = m_particleFilterSelector->selectedElements();
  m_analysisSetting.setParticleFilter(particleFilter);

  Enums::Particles mcParticleFilter = m_mcParticleFilterSelector->selectedElements();
  m_analysisSetting.setMcParticleFilter(mcParticleFilter);

  foreach (CutSelector* selector, m_cutSelectors)
    if (selector->checked())
      m_analysisSetting.cutFilter().addCut(selector->cut());
}

void MainWindow::analysisSettingToGui()
{
  int first = m_analysisSetting.firstEvent() < 0 ? 0 : m_analysisSetting.firstEvent();
  int last = m_analysisSetting.lastEvent() < 0 ? m_analysis->numberOfEvents() - 1 : m_analysisSetting.lastEvent();
  numberOfEventsChanged(last - first + 1);

  if (m_analysisSetting.numberOfThreads() < 0) {
    m_ui.numberOfThreadsSpinBox->setValue(QThread::idealThreadCount());
    m_analysisSetting.setNumberOfThreads(QThread::idealThreadCount());
  } else {
    m_ui.numberOfThreadsSpinBox->setValue(m_analysisSetting.numberOfThreads());
  }

  foreach (TopicSelector* selector, m_topicSelectors)
    selector->setChecked(m_analysisSetting.analysisTopics() & selector->topic());

  m_ui.trackComboBox->setCurrentIndex(m_ui.trackComboBox->findText(Enums::label(m_analysisSetting.trackType())));

  m_ui.reconstructionMethodComboBox->setCurrentIndex(m_ui.reconstructionMethodComboBox->findText(Enums::label(m_analysisSetting.reconstructionMethod())));

  m_correctionSelector->setSelectedElements(m_analysisSetting.corrections());
  m_reconstructionSelector->setSelectedElements(m_analysisSetting.reconstructionMethods());
  m_particleSelector->setSelectedElements(m_analysisSetting.particles());
  m_likelihoodSelector->setSelectedElements(m_analysisSetting.likelihoods());
  m_particleFilterSelector->setSelectedElements(m_analysisSetting.particleFilter());
  m_mcParticleFilterSelector->setSelectedElements(m_analysisSetting.mcParticleFilter());

  foreach (CutSelector* selector, m_cutSelectors) {
    foreach (Cut cut, m_analysisSetting.cutFilter().cuts()) {
      if (selector->cut().type() == cut.type()) {
        selector->setChecked(true);
        if (cut.minIsSet()) selector->setMinimum(cut.min());
        if (cut.maxIsSet()) selector->setMaximum(cut.max());
      }
    }
  }

  checkSelectAll();
}

void MainWindow::analyzeButtonClicked()
{
  if (m_ui.analyzeButton->text() == "&start") {
    guiToAnalysisSetting();
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
  m_ui.lastEventSpinBox->setMaximum(nEvents - 1);
  m_ui.firstEventSpinBox->setValue(0);
  m_ui.lastEventSpinBox->setValue(nEvents - 1);
  m_ui.analyzeButton->setEnabled(nEvents > 0);
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
    if (m_batch) {
      m_analysis->save("batch.root", Plotter::rootWidget()->GetCanvas());
      qApp->quit();
    }
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
  int elapsed = m_time.elapsed() / 1000;
  QString time = QString("%1m%2s").arg(elapsed / 60, 2, 10, QLatin1Char('0')).arg(elapsed % 60, 2, 10, QLatin1Char('0'));
  m_ui.timeLabel->setText(time);
  if (m_batch)
    qDebug() << qPrintable(QString("%1 -> %2\%").arg(time).arg(m_analysis->progress(), 5, 'f', 2, '0'));
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
  guiToAnalysisSetting();
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
  m_analysisSetting.load(fileName);
  analysisSettingToGui();
}
