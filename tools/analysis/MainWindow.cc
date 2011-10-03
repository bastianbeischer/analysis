#include "MainWindow.hh"

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

#include "RootQtWidget.hh"
#include "TRDSpectrumPlotCollection.hh"
#include "TRDSpectrumCherenkovPlotCollection.hh"
#include "TRDSpectrumVsTimePlotCollection.hh"
#include "BendingPositionPlot.hh"
#include "BendingAnglePlot.hh"
#include "ResidualPlot.hh"
#include "GeometricOccupancyPlot.hh"
#include "BendingAnglePositionPlot.hh"
#include "Chi2Plot.hh"
#include "Chi2PerNdfPlot.hh"
#include "Chi2VsMomentumPlot.hh"
#include "AlbedosVsMomentumPlot.hh"
#include "TOFPositionCorrelationPlot.hh"
#include "MomentumSpectrumPlot.hh"
#include "SignalHeight2DPlot.hh"
#include "SignalHeightPlot.hh"
#include "ClusterLengthPlot.hh"
#include "ClusterShapePlot.hh"
#include "BetaPlot.hh"
#include "TOFTimeShiftPlotCollection.hh"
#include "BetaMomentumCorrelationPlot.hh"
#include "CutStatisticsPlot.hh"
#include "TrackerLayerStatisticsPlot.hh"
#include "TrackingEfficiencyVsMomentumPlot.hh"
#include "MultiLayerTrackingEfficiencyPlot.hh"
#include "SingleLayerTrackingEfficiencyPlot.hh"
#include "TRDClustersOnTrackPlot.hh"
#include "TRDDistanceWireToTrackPlot.hh"
#include "TRDDistanceInTube.hh"
#include "TRDTimeCorrectionPlot.hh"
#include "TRDEnergyDepositionOverMomentumPlot.hh"
#include "TRDSpectrumPlot.hh"
#include "TRDSpectrumVsTimePlot.hh"
#include "TRDSpectrumVsTemperaturePlot.hh"
#include "TRDSpectrumVsPressurePlot.hh"
#include "TRDOccupancyPlot.hh"
#include "TRDEfficiencyPlot.hh"
#include "TRDLikelihoodPlot.hh"
#include "TRDLikelihoodFunctionsPlot.hh"
#include "TotalEnergyDepositionPlot.hh"
#include "TotalEnergyDepositionTRDvsTrackerPlot.hh"
#include "TimeResolutionPlotCollection.hh"
#include "TOFTimeDifferencePlotCollection.hh"
#include "TotalSignalHeightPlot.hh"
#include "TOFEfficiencyPlot.hh"
#include "TOTMomentumCorrelation.hh"
#include "TOTBetaCorrelation.hh"
#include "TOTPlot.hh"
#include "TOTLayerPlot.hh"
#include "TOTLayerCollection.hh"
#include "TOTIonizationCorrelation.hh"
#include "TOTTemperatureCorrelationPlotCollection.hh"
#include "TOTTimeCorrelationPlotCollection.hh"
#include "TOTTemperatureCorrelationPlot.hh"
#include "TOTTimeCorrelationPlot.hh"
#include "TemperatureTimePlot.hh"
#include "PressureTimePlot.hh"
#include "SettingTimePlot.hh"
#include "ChannelTriggerProbabilityPlot.hh"
#include "TOFTimeShiftTriggerPlot.hh"
#include "TriggerRateTimePlot.hh"
#include "HeightTimePlot.hh"
#include "MCTotalEnergyDepositionTRDvsTrackerPlot.hh"
#include "MCTRDSpectrumPlot.hh"
#include "MCRigidityResolutionPlot.hh"
#include "MCTRDCalibrationPlot.hh"
#include "ResidualPlotMC.hh"
#include "TestbeamRigidityResolutionPlot.hh"
#include "PMTPlot.hh"
#include "PMTCorrelationPlot.hh"
#include "BeamProfilePlot.hh"
#include "ZSquareTRDPlot.hh"
#include "TOFBarShiftPlotCollection.hh"
#include "TimeReconstructionPlot.hh"
#include "EfficiencyCollection.hh"
#include "ZenithDistributionPlot.hh"
#include "ZenithAzimuthCorrelation.hh"
#include "AzimuthDistributionPlot.hh"
#include "AzimuthMigrationHistogram.hh"
#include "MeasurementTimePlot.hh"
#include "RigidityMigrationHistogram.hh"
#include "AzimuthPositionCorrelation.hh"
#include "AzimuthCutStatistics.hh"
#include "Helpers.hh"
#include "FluxCollection.hh"
#include "EventTimeDifferencePlot.hh"
#include "TopicSelector.hh"
#include "Caption.hh"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_processors()
  , m_reader(new EventReader(this))
  , m_topLevelPath(Helpers::analysisPath())
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
  setupViewActions();

  for (QMap<Enums::TrackType, QString>::ConstIterator it = Enums::trackTypeBegin(); it != Enums::trackTypeEnd(); ++it)
    m_ui.trackComboBox->addItem(it.value());
  m_ui.trackComboBox->setCurrentIndex(m_ui.trackComboBox->findText(Enums::label(Enums::CenteredBrokenLine)));

  m_controlWidgets
    << m_ui.selectAllButton << m_ui.selectTrackerButton << m_ui.selectTrdButton << m_ui.selectTofButton
    << m_ui.trackComboBox << m_ui.firstEventSpinBox << m_ui.lastEventSpinBox << m_ui.numberOfThreadsSpinBox
    << m_ui.rigidityCutCheckBox << m_ui.rigidityLineEditMax << m_ui.rigidityLineEditMin << m_ui.betaCutCheckBox
    << m_ui.betaLineEditMax << m_ui.betaLineEditMin << m_ui.tofTotCutCheckBox << m_ui.tofTotLineEditMax
    << m_ui.tofTotLineEditMin << m_ui.trdDepositionCutCheckBox << m_ui.trdDepositionLineEditMax
    << m_ui.trdDepositionLineEditMin << m_ui.cherenkovCutCheckBox << m_ui.cherenkovCutBelowRadioButton
    << m_ui.cherenkovCutAboveRadioButton << m_ui.cherenkovCutDoubleSpinBox;

  connect(m_reader, SIGNAL(started()), this, SLOT(toggleControlWidgetsStatus()));
  connect(m_reader, SIGNAL(finished()), this, SLOT(toggleControlWidgetsStatus()));
  connect(m_reader, SIGNAL(numberOfEventsChanged(int)), this, SLOT(numberOfEventsChanged(int)));
  connect(m_reader, SIGNAL(numberOfEventsChanged(int)), this, SLOT(numberOfEventsChanged(int)));
  connect(m_reader, SIGNAL(started()), &m_updateTimer, SLOT(start()));
  connect(m_reader, SIGNAL(finished()), &m_updateTimer, SLOT(stop()));
  connect(m_reader, SIGNAL(finished()), m_ui.plotter, SLOT(finalizeAnalysis()));

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
  connect(m_ui.saveForBatchJobAction, SIGNAL(triggered()), this, SLOT(saveForBatchJobActionTriggered()));

  connect(m_ui.setFileListDialogAction, SIGNAL(triggered()), this, SLOT(setOrAddFileListDialogActionTriggered()));
  connect(m_ui.addFileListDialogAction, SIGNAL(triggered()), this, SLOT(setOrAddFileListDialogActionTriggered()));
  connect(m_ui.quitAction, SIGNAL(triggered()), this, SLOT(close()));

  connect(m_ui.plotter, SIGNAL(titleChanged(const QString&)), this, SLOT(plotterTitleChanged(const QString&)));
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
}

MainWindow::~MainWindow()
{
  qDeleteAll(m_processors);
  m_processors.clear();
  delete m_ui.plotter;
  delete m_reader;
}

void MainWindow::setupTopicSelectors()
{
  QVBoxLayout* layout = new QVBoxLayout;
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

void MainWindow::processArguments(QStringList arguments)
{
  arguments.removeFirst();
  QRegExp onlyDigits("^\\d+$");
  foreach(QString argument, arguments) {
    if (!onlyDigits.exactMatch(argument)) {
      if (argument.endsWith(".root"))
        m_reader->addRootFile(argument);
      else
        m_reader->addFileList(argument);
    }
  }
  QStringList eventRange = arguments.filter(onlyDigits);
  if (eventRange.size() == 2) {
    int firstEvent = eventRange.at(0).toInt();
    int lastEvent = eventRange.at(1).toInt();
    m_ui.firstEventSpinBox->setValue(firstEvent);
    m_ui.lastEventSpinBox->setValue(lastEvent);
    firstOrLastEventChanged();
  }
}

void MainWindow::showTopic(Enums::AnalysisTopic topic)
{
  QVector<unsigned int> plotIndices = m_ui.plotter->plotIndices(topic);
  for (int i = 0; i < plotIndices.size(); ++i) {
    m_activePlots.append(plotIndices[i]);
    QListWidgetItem* item = new QListWidgetItem(m_ui.plotter->plotTitle(plotIndices[i]));
    item->setCheckState(Qt::Checked);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    m_ui.listWidget->addItem(item);
  }
}

void MainWindow::hideTopic(Enums::AnalysisTopic topic)
{
  QVector<int> matchingItems;
  for (int i = m_ui.listWidget->count() - 1; i >= 0; --i) {
    if (m_ui.plotter->plotTopic(m_activePlots[i]) == topic)
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
  if (item && item->checkState() == Qt::Unchecked) {
    removeListWidgetItem(m_ui.listWidget->row(item));
  }
}

void MainWindow::listWidgetCurrentRowChanged(int i)
{
  disconnect(m_ui.drawOptionComboBox, 0, this, 0);
  m_ui.drawOptionComboBox->setEnabled(false);
  m_ui.drawOptionComboBox->clear();
  m_drawOptions.clear();
  if (i < 0 || m_activePlots.size() == 0) {
    m_ui.plotter->selectPlot(-1);
    return;
  }
  if (!m_inhibitDraw) {
    m_ui.plotter->selectPlot(m_activePlots[i]);
    switch (m_ui.plotter->selectedPlotType()) {
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
    if (m_drawOptions[i] == m_ui.plotter->drawOption()) {
      m_ui.drawOptionComboBox->setCurrentIndex(i);
      break;
    }
  }
  m_ui.drawOptionComboBox->setEnabled(true);
  connect(m_ui.drawOptionComboBox, SIGNAL(currentIndexChanged(int)),
    this, SLOT(drawOptionComboBoxCurrentIndexChanged(int)));
}

void MainWindow::setupPlots()
{
  Setup* setup = Setup::instance();
  const ElementIterator& elementStartIt = setup->firstElement();
  const ElementIterator& elementEndIt = setup->lastElement();
  const LayerIterator& layerStartIt = setup->firstLayer();
  const LayerIterator& layerEndIt = setup->lastLayer();
  ElementIterator elementIt;
  LayerIterator layerIt;

  m_ui.plotter->clearPlots();
  m_activePlots.clear();
  m_ui.listWidget->clear();
    
  QDateTime first = m_reader->startTime();
  QDateTime last = m_reader->stopTime();

  QVector<Enums::AnalysisTopic> topics;
  foreach (TopicSelector* selector, m_topicSelectors)
    if (selector->checked())
      topics.append(selector->topic());

  if (topics.contains(Enums::SignalHeightTracker)) {
    m_ui.plotter->addPlot(new SignalHeight2DPlot);
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tracker)
        m_ui.plotter->addPlot(new SignalHeightPlot(Enums::SignalHeightTracker, element->id()));
    }
  }

  if (topics.contains(Enums::SignalHeightTRD)) {
    m_ui.plotter->addPlot(new ZSquareTRDPlot);
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::trd)
        m_ui.plotter->addPlot(new SignalHeightPlot(Enums::SignalHeightTRD, element->id()));
    }

    m_ui.plotter->addPlot(new TRDSpectrumPlot());
    m_ui.plotter->addPlot(new TRDSpectrumPlotCollection);
    for (int i = 0; i < 8; ++i)
      m_ui.plotter->addPlot(new TRDSpectrumPlot(i, TRDSpectrumPlot::layer));

    m_ui.plotter->addPlot(new TRDSpectrumVsTimePlot(first,last));
    m_ui.plotter->addPlot(new TRDSpectrumVsTimePlotCollection(first, last));

    m_ui.plotter->addPlot(new TRDSpectrumVsTemperaturePlot());
    m_ui.plotter->addPlot(new TRDSpectrumVsPressurePlot());
  }

  if (topics.contains(Enums::ClusterShapeTracker)) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tracker) {
        m_ui.plotter->addPlot(new ClusterLengthPlot(Enums::ClusterShapeTracker, element->id()));
        m_ui.plotter->addPlot(new ClusterShapePlot(element->id()));
      }
    }
  }
  if (topics.contains(Enums::ClusterShapeTRD)) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::trd)
        m_ui.plotter->addPlot(new ClusterLengthPlot(Enums::ClusterShapeTRD, element->id()));
    }
  }
  if (topics.contains(Enums::TimeOverThreshold)) {
    m_ui.plotter->addPlot(new TOTPlot);
    m_ui.plotter->addPlot(new TOTLayerCollection(new TOTLayerPlot()));
    m_ui.plotter->addPlot(new TOTLayerCollection(new TOTIonizationCorrelation(Hit::trd)));
    m_ui.plotter->addPlot(new TOTLayerCollection(new TOTIonizationCorrelation(Hit::tracker)));
    m_ui.plotter->addPlot(new TOTLayerCollection(new TOTMomentumCorrelation()));
    m_ui.plotter->addPlot(new TOTLayerCollection(new TOTBetaCorrelation()));
    m_ui.plotter->addPlot(new TOTTemperatureCorrelationPlotCollection);
    m_ui.plotter->addPlot(new TOTTimeCorrelationPlotCollection(first, last));
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof)
        for (int ch = 0; ch < 4; ++ch)
          m_ui.plotter->addPlot(new TOTTemperatureCorrelationPlot(element->id() | ch));
    }
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof)
        for (int ch = 0; ch < 4; ++ch)
          m_ui.plotter->addPlot(new TOTTimeCorrelationPlot(element->id() | ch, first, last));
    }
  }
  if (topics.contains(Enums::Tracking)) {
    m_ui.plotter->addPlot(new BendingPositionPlot);
    m_ui.plotter->addPlot(new BendingAnglePlot);
    for (double cut = .004; cut < .008; cut+=.001)
      m_ui.plotter->addPlot(new BendingAnglePositionPlot(cut));
    for (unsigned short ndf = 6; ndf <= 16; ndf++)
      m_ui.plotter->addPlot(new Chi2Plot(ndf));
    m_ui.plotter->addPlot(new Chi2PerNdfPlot);
    m_ui.plotter->addPlot(new Chi2VsMomentumPlot);
    m_ui.plotter->addPlot(new ZenithAzimuthCorrelation());
    m_ui.plotter->addPlot(new ZenithDistributionPlot());
    m_ui.plotter->addPlot(new AzimuthDistributionPlot());
    m_ui.plotter->addPlot(new AzimuthCutStatistics());
    m_ui.plotter->addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::X, Enums::Positive));
    m_ui.plotter->addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::X, Enums::Negative));
    m_ui.plotter->addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::X, Enums::Positive | Enums::Negative));
    m_ui.plotter->addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::Y, Enums::Positive));
    m_ui.plotter->addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::Y, Enums::Negative));
    m_ui.plotter->addPlot(new AzimuthPositionCorrelation(AzimuthPositionCorrelation::Y, Enums::Positive | Enums::Negative));
  }
  if (topics.contains(Enums::Occupancy)) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      m_ui.plotter->addPlot(new GeometricOccupancyPlot(layer->z()));
    }
  }
  if (topics.contains(Enums::ResidualsTracker)) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (layer->z() > -240 && layer->z() < 240)
        m_ui.plotter->addPlot(new ResidualPlot(Enums::ResidualsTracker, layer));
    }
  }
  if (topics.contains(Enums::ResidualsTRD)) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (layer->z() > -520 && layer->z() < -240)
        m_ui.plotter->addPlot(new ResidualPlot(Enums::ResidualsTRD, layer));
    }
  }
  if (topics.contains(Enums::MomentumReconstruction)) {
    m_ui.plotter->addPlot(new BetaMomentumCorrelationPlot());
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(Enums::Positive));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(Enums::Negative));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(Enums::Positive | Enums::Negative));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(Enums::Positive | Enums::Negative, true));
    m_ui.plotter->addPlot(new AlbedosVsMomentumPlot());
    m_ui.plotter->addPlot(new MeasurementTimePlot(first, last));
    m_ui.plotter->addPlot(new FluxCollection(first, last));
  }
  if (topics.contains(Enums::EfficiencyTOF)) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof) {
        for (int ch = 0; ch < 4; ++ch) {
          m_ui.plotter->addPlot(new TOFEfficiencyPlot(element->id() | ch));
        }
      }
    }
  }
  if (topics.contains(Enums::ResolutionTOF)) {
    m_ui.plotter->addPlot(new TimeResolutionPlotCollection);
  }
  if (topics.contains(Enums::CalibrationTOF)) {
    m_ui.plotter->addPlot(new ChannelTriggerProbabilityPlot);
    m_ui.plotter->addPlot(new TOFTimeShiftTriggerPlot);
    m_ui.plotter->addPlot(new TOFTimeShiftPlotCollection);
    m_ui.plotter->addPlot(new TOFTimeDifferencePlotCollection);
    m_ui.plotter->addPlot(new TOFBarShiftPlotCollection);
  }
  if (topics.contains(Enums::MiscellaneousTracker)) {
    m_ui.plotter->addPlot(new TotalSignalHeightPlot);
    m_ui.plotter->addPlot(new CutStatisticsPlot);
    m_ui.plotter->addPlot(new TrackerLayerStatisticsPlot);
    m_ui.plotter->addPlot(new TrackingEfficiencyVsMomentumPlot(Enums::Positive));
    m_ui.plotter->addPlot(new TrackingEfficiencyVsMomentumPlot(Enums::Negative));
    m_ui.plotter->addPlot(new TrackingEfficiencyVsMomentumPlot(Enums::Positive | Enums::Negative));
    m_ui.plotter->addPlot(new SingleLayerTrackingEfficiencyPlot(Enums::Positive));
    m_ui.plotter->addPlot(new SingleLayerTrackingEfficiencyPlot(Enums::Negative));
    m_ui.plotter->addPlot(new SingleLayerTrackingEfficiencyPlot(Enums::Positive | Enums::Negative));
    m_ui.plotter->addPlot(new MultiLayerTrackingEfficiencyPlot(MultiLayerTrackingEfficiencyPlot::Positive));
    m_ui.plotter->addPlot(new MultiLayerTrackingEfficiencyPlot(MultiLayerTrackingEfficiencyPlot::Negative));
    m_ui.plotter->addPlot(new MultiLayerTrackingEfficiencyPlot(MultiLayerTrackingEfficiencyPlot::All));
    m_ui.plotter->addPlot(new EfficiencyCollection());
  }
  if (topics.contains(Enums::MiscellaneousTRD)) {
    m_ui.plotter->addPlot(new TRDClustersOnTrackPlot(Enums::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TRDDistanceWireToTrackPlot(Enums::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TRDDistanceInTube(Enums::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TotalEnergyDepositionPlot());
    m_ui.plotter->addPlot(new TRDEnergyDepositionOverMomentumPlot(Enums::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TotalEnergyDepositionTRDvsTrackerPlot());
    m_ui.plotter->addPlot(new TRDEfficiencyPlot());
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::numberOfHits));
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::numberOfHits, true));
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::sumOfSignalHeights, true));
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::sumOfSignalHeightsNormalizedToHits, true));
    m_ui.plotter->addPlot(new TRDTimeCorrectionPlot(first, last));
    m_ui.plotter->addPlot(new TRDLikelihoodFunctionsPlot());
  }
  if (topics.contains(Enums::MiscellaneousTOF)) {
    m_ui.plotter->addPlot(new BetaPlot);
    m_ui.plotter->addPlot(new TimeReconstructionPlot(TimeReconstructionPlot::Mean));
    m_ui.plotter->addPlot(new TimeReconstructionPlot(TimeReconstructionPlot::Median));
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof)
        m_ui.plotter->addPlot(new TOFPositionCorrelationPlot(element->id()));
    }
    m_ui.plotter->addPlot(new EventTimeDifferencePlot(m_ui.numberOfThreadsSpinBox->value()));
  }
  if (topics.contains(Enums::SlowControl)) {
    QVector<SensorTypes::Type> temperatureSensors = QVector<SensorTypes::Type>::fromStdVector(SensorTypes::temperatureSensors());
    foreach(SensorTypes::Type sensor, temperatureSensors)
      m_ui.plotter->addPlot(new TemperatureTimePlot(sensor, first, last));
    m_ui.plotter->addPlot(new PressureTimePlot(SensorTypes::TRD_PRESSURE, first, last));
    m_ui.plotter->addPlot(new PressureTimePlot(SensorTypes::TRD_PRESSURE_SMOOTHED, first, last));
    m_ui.plotter->addPlot(new TriggerRateTimePlot(first, last));
    m_ui.plotter->addPlot(new HeightTimePlot(first, last));
  }
  if (topics.contains(Enums::MonteCarlo)) {
    m_ui.plotter->addPlot(new MCTotalEnergyDepositionTRDvsTrackerPlot());
  }
  if (topics.contains(Enums::MonteCarloTRD)) {
    m_ui.plotter->addPlot(new MCTRDSpectrumPlot());
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (layer->z() > -520 && layer->z() < -240)
        m_ui.plotter->addPlot(new ResidualPlotMC(Enums::MonteCarloTRD, layer));
    }
    m_ui.plotter->addPlot(new MCTRDCalibrationPlot());
    m_ui.plotter->addPlot(new TRDLikelihoodPlot(Enums::MonteCarloTRD));
  }
  if (topics.contains(Enums::MonteCarloTracker)) {
    m_ui.plotter->addPlot(new MCRigidityResolutionPlot(Enums::Positron));
    m_ui.plotter->addPlot(new MCRigidityResolutionPlot(Enums::Electron));
    m_ui.plotter->addPlot(new MCRigidityResolutionPlot(Enums::Proton));
    m_ui.plotter->addPlot(new MCRigidityResolutionPlot(Enums::PiMinus));
    m_ui.plotter->addPlot(new MCRigidityResolutionPlot(Enums::PiPlus));
    m_ui.plotter->addPlot(new MCRigidityResolutionPlot(Enums::Helium));
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (layer->z() > -240 && layer->z() < 240)
        m_ui.plotter->addPlot(new ResidualPlotMC(Enums::MonteCarloTracker, layer));
    }
    m_ui.plotter->addPlot(new AzimuthMigrationHistogram());
    m_ui.plotter->addPlot(new RigidityMigrationHistogram());
  }
  if (topics.contains(Enums::Testbeam)) {
    m_ui.plotter->addPlot(new SettingTimePlot(SettingTimePlot::MagnetInstalled, first, last));
    m_ui.plotter->addPlot(new SettingTimePlot(SettingTimePlot::Momentum, first, last));
    m_ui.plotter->addPlot(new SettingTimePlot(SettingTimePlot::Polarity, first, last));
    m_ui.plotter->addPlot(new SettingTimePlot(SettingTimePlot::AbsMomentum, first, last));
    m_ui.plotter->addPlot(new TRDSpectrumCherenkovPlotCollection());
    QVector<SensorTypes::Type> beamSensors = QVector<SensorTypes::Type>::fromStdVector(SensorTypes::beamSensors());
    foreach(SensorTypes::Type sensor, beamSensors)
      m_ui.plotter->addPlot(new PMTPlot(sensor));
    m_ui.plotter->addPlot(new PMTCorrelationPlot);
    m_ui.plotter->addPlot(new TRDLikelihoodPlot(Enums::Testbeam));
    m_ui.plotter->addPlot(new BeamProfilePlot(BeamProfilePlot::Horizontal));
    m_ui.plotter->addPlot(new BeamProfilePlot(BeamProfilePlot::Vertical));
    m_ui.plotter->addPlot(new TestbeamRigidityResolutionPlot(Enums::Positron));
    m_ui.plotter->addPlot(new TestbeamRigidityResolutionPlot(Enums::Electron));
    m_ui.plotter->addPlot(new TestbeamRigidityResolutionPlot(Enums::Proton));
    m_ui.plotter->addPlot(new TestbeamRigidityResolutionPlot(Enums::PiMinus));
    m_ui.plotter->addPlot(new TestbeamRigidityResolutionPlot(Enums::PiPlus));
  }
}

void MainWindow::setupAnalysis(Enums::TrackType& type, Enums::Corrections& flags, Enums::Particles& filterTypes, CutFilter& cutFilter, Enums::Particles& mcFilterTypes)
{
  type = Enums::trackType(m_ui.trackComboBox->currentText());

  flags = Enums::NoCorrection;
  foreach (QCheckBox* checkBox, m_correctionCheckBoxes)
    if (checkBox->isChecked())
      flags|= Enums::correction(checkBox->text());

  filterTypes = Enums::NoParticle;
  foreach (QCheckBox* checkBox, m_particleFilterCheckBoxes)
    if (checkBox->isChecked())
      filterTypes|= Enums::particle(checkBox->text());

  mcFilterTypes = Enums::NoParticle;
  foreach (QCheckBox* checkBox, m_mcFilterCheckBoxes)
    if (checkBox->isChecked())
      mcFilterTypes|= Enums::particle(checkBox->text());
  
  if (m_ui.rigidityCutCheckBox->isChecked()) {
    Cut cut(Cut::rigidity);
    QString minText = m_ui.rigidityLineEditMin->text();
    QString maxText = m_ui.rigidityLineEditMax->text();
    bool minIsNumber = false;
    double min = minText.toDouble(&minIsNumber);
    if (minIsNumber) {
      cut.setMin(min);
    }
    bool maxIsNumber = false;
    double max = maxText.toDouble(&maxIsNumber);
    if (maxIsNumber) {
      cut.setMax(max);
    }
    cutFilter.addCut(cut);
  }
  if (m_ui.betaCutCheckBox->isChecked()) {
    Cut cut(Cut::beta);
    QString minText = m_ui.betaLineEditMin->text();
    QString maxText = m_ui.betaLineEditMax->text();
    bool minIsNumber = false;
    double min = minText.toDouble(&minIsNumber);
    if (minIsNumber) {
      cut.setMin(min);
    }
    bool maxIsNumber = false;
    double max = maxText.toDouble(&maxIsNumber);
    if (maxIsNumber) {
      cut.setMax(max);
    }
    cutFilter.addCut(cut);
  }
  if (m_ui.trdDepositionCutCheckBox->isChecked()) {
    Cut cut(Cut::trdDeposition);
    QString minText = m_ui.trdDepositionLineEditMin->text();
    QString maxText = m_ui.trdDepositionLineEditMax->text();
    bool minIsNumber = false;
    double min = minText.toDouble(&minIsNumber);
    if (minIsNumber) {
      cut.setMin(min);
    }
    bool maxIsNumber = false;
    double max = maxText.toDouble(&maxIsNumber);
    if (maxIsNumber) {
      cut.setMax(max);
    }
    cutFilter.addCut(cut);
  }
  if (m_ui.tofTotCutCheckBox->isChecked()) {
    Cut cut(Cut::tofTimeOverThreshold);
    QString minText = m_ui.tofTotLineEditMin->text();
    QString maxText = m_ui.tofTotLineEditMax->text();
    bool minIsNumber = false;
    double min = minText.toDouble(&minIsNumber);
    if (minIsNumber) {
      cut.setMin(min);
    }
    bool maxIsNumber = false;
    double max = maxText.toDouble(&maxIsNumber);
    if (maxIsNumber) {
      cut.setMax(max);
    }
    cutFilter.addCut(cut);
  }
  if (m_ui.cherenkovCutCheckBox->isChecked()) {
    Cut cut(Cut::cherenkov);
    double cherenkovLimit = m_ui.cherenkovCutDoubleSpinBox->value();
    if (m_ui.cherenkovCutBelowRadioButton->isChecked())
      cut.setMax(cherenkovLimit);
    else
      cut.setMin(cherenkovLimit);
    cutFilter.addCut(cut);
  }
}

void MainWindow::analyzeButtonClicked()
{
  if (m_ui.analyzeButton->text() == "&start") {
    Enums::TrackType type = Enums::NoTrack;
    Enums::Corrections flags;
    Enums::Particles filterTypes;
    CutFilter cutFilter;
    Enums::Particles mcFilterTypes;
    setupAnalysis(type, flags, filterTypes, cutFilter, mcFilterTypes);
    setupPlots();

    qDeleteAll(m_processors);
    m_processors.clear();
    for (int i = 0; i < m_ui.numberOfThreadsSpinBox->value(); i++) {
      AnalysisProcessor* processor = new AnalysisProcessor;
      foreach(AnalysisPlot* plot, m_ui.plotter->plots())
        processor->addDestination(plot);
      processor->setTrackType(type);
      processor->setCorrectionFlags(flags);
      processor->setParticleFilter(filterTypes);
      processor->setCutFilter(cutFilter);
      processor->setMCFilter(mcFilterTypes);
      m_processors.append(processor);
    }
    m_reader->start(m_processors, m_ui.firstEventSpinBox->value(), m_ui.lastEventSpinBox->value());
  } else {
    m_reader->stop();
    m_ui.analyzeButton->setText("&stopping...");
  }
}

void MainWindow::setOrAddFileListDialogActionTriggered()
{
  QStringList files = QFileDialog::getOpenFileNames(this,
    "Select one or more file lists to open", "", "*.txt;;*.*;;*", 0, m_dialogOptions);
  if (sender() == m_ui.setFileListDialogAction) {
    foreach(QString file, files)
      m_reader->setFileList(file);
  } else if (sender() == m_ui.addFileListDialogAction) {
    foreach(QString file, files)
      m_reader->addFileList(file);
  }
}

void MainWindow::saveButtonsClicked()
{
  QString fileName = m_ui.titleLabel->text();
  fileName.remove(QChar('/'));
  fileName.remove(QChar(':'));
  fileName.prepend(m_topLevelPath + "/plots/");
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
  QDir dir(m_topLevelPath);
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
  QDir dir(m_topLevelPath);
  if (!dir.exists("plots"))
    dir.mkdir("plots");
  QStringList fileFormatEndings;
  fileFormatEndings << "svg" << "pdf" << "eps" << "root" << "png";
  for (int i = 0; i < m_ui.listWidget->count(); ++i) {
    m_ui.listWidget->setCurrentRow(i);
    const QString& directoryName = m_topLevelPath + "/plots/";
    foreach (QString fileFormatEnding, fileFormatEndings)
    m_ui.plotter->saveCanvas(directoryName + '/' + m_ui.plotter->plotTitle(m_activePlots[i]) + "." + fileFormatEnding);
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
        m_ui.plotter->saveCanvas(directoryName + '/' + m_ui.plotter->plotTitle(m_activePlots[i]) + "." + fileFormatEnding);
    }
}

void MainWindow::saveForPostAnalysisActionTriggered()
{
  QDir dir(m_topLevelPath);
  if (!dir.exists("plots"))
    dir.mkdir("plots");
  m_ui.plotter->saveForPostAnalysis(m_topLevelPath + "/plots/postAnalysis.root");
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
  m_ui.plotter->saveForPostAnalysis(fileName);
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
  m_reader->stop();
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

void MainWindow::plotterTitleChanged(const QString& title)
{
  m_ui.titleLabel->setText(title);
}

void MainWindow::plotterPositionChanged(double x, double y)
{
  m_ui.positionLabel->setText(QString("%1%2  %3%4")
    .arg(x < 0 ? '-' : '+').arg(qAbs(x), 7, 'f', 3, '0')
    .arg(y < 0 ? '-' : '+').arg(qAbs(y), 7, 'f', 3, '0'));
}

void MainWindow::update()
{
  m_ui.dataChainProgressBar->setValue(m_reader->progress());
  m_ui.eventQueueProgressBar->setValue(m_reader->buffer());
  m_ui.timeLabel->setText(QString("%1s").arg(m_time.elapsed()/1000));
}

void MainWindow::drawOptionComboBoxCurrentIndexChanged(int i)
{
  m_ui.plotter->setDrawOption(m_drawOptions[i]);
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

void MainWindow::saveForBatchJobActionTriggered()
{
  QString fileEnding;
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "*.root", &fileEnding, m_dialogOptions);
  if (fileName.isEmpty())
    return;
  fileEnding.remove(0, 1);
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
}
