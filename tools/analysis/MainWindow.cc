#include "MainWindow.hh"

#include "Plotter.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "AnalysisPlot.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "Corrections.hh"
#include "AnalysisProcessor.hh"
#include "EventReader.hh"

#include "BendingPositionPlot.hh"
#include "BendingAnglePlot.hh"
#include "ResidualPlot.hh"
#include "GeometricOccupancyPlot.hh"
#include "GeometricOccupancyProjectionPlot.hh"
#include "BendingAnglePositionPlot.hh"
#include "Chi2Plot.hh"
#include "Chi2PerNdfPlot.hh"
#include "Chi2VsMomentumPlot.hh"
#include "AlbedosVsMomentumPlot.hh"
#include "TOFPositionCorrelationPlot.hh"
#include "MomentumSpectrumPlot.hh"
#include "SignalHeightPlot.hh"
#include "ClusterLengthPlot.hh"
#include "ClusterShapePlot.hh"
#include "BetaPlot.hh"
#include "TOFTimeShiftPlot.hh"
#include "BetaMomentumCorrelationPlot.hh"
#include "CutStatisticsPlot.hh"
#include "TrackerLayerStatisticsPlot.hh"
#include "TrackingEfficiencyVsMomentumPlot.hh"
#include "MultiLayerTrackingEfficiencyPlot.hh"
#include "SingleLayerTrackingEfficiencyPlot.hh"
#include "TRDClustersOnTrackPlot.hh"
#include "TRDDistanceWireToTrackPlot.hh"
#include "TRDDistanceInTube.hh"
#include "TRDMoPVTimeEvolutionPlot.hh"
#include "TRDEnergyDepositionOverMomentumPlot.hh"
#include "TRDSpectrumPlot.hh"
#include "TRDFitPlot.hh"
#include "TRDOccupancyPlot.hh"
#include "TRDEfficiencyPlot.hh"
#include "TotalEnergyDepositionPlot.hh"
#include "TotalEnergyDepositionTRDvsTrackerPlot.hh"
#include "TimeResolutionPlot.hh"
#include "TOFTimeDifferencePlot.hh"
#include "TotalSignalHeightPlot.hh"
#include "TOFEfficiencyPlot.hh"
#include "TOTMomentumCorrelation.hh"
#include "TOTPlot.hh"
#include "TOTTemperatureCorrelationPlot.hh"
#include "TOFAlignment.hh"
#include "TOTTimeCorrelationPlot.hh"
#include "TemperatureTimePlot.hh"
#include "ChannelTriggerProbabilityPlot.hh"
#include "TOFTimeShiftTriggerPlot.hh"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_processors()
  , m_reader(new EventReader(this))
  , m_activePlots()
  , m_inhibitDraw(false)
  , m_time()
  , m_updateTimer()
{
  m_ui.setupUi(this);
 
  char* env = getenv("PERDAIXANA_PATH");
  if (env == 0) {
    qFatal("ERROR: You need to set PERDAIXANA_PATH environment variable to the toplevel location!");
  }
  m_topLevelPath = QString(env);

  m_topicCheckBoxes.append(m_ui.signalHeightTrackerCheckBox);
  m_topicCheckBoxes.append(m_ui.signalHeightTRDCheckBox);
  m_topicCheckBoxes.append(m_ui.clusterShapeTrackerCheckBox);
  m_topicCheckBoxes.append(m_ui.clusterShapeTRDCheckBox);
  m_topicCheckBoxes.append(m_ui.timeOverThresholdCheckBox);
  m_topicCheckBoxes.append(m_ui.trackingCheckBox);
  m_topicCheckBoxes.append(m_ui.occupancyCheckBox);
  m_topicCheckBoxes.append(m_ui.residualsTrackerCheckBox);
  m_topicCheckBoxes.append(m_ui.residualsTRDCheckBox);
  m_topicCheckBoxes.append(m_ui.momentumReconstructionCheckBox);
  m_topicCheckBoxes.append(m_ui.efficiencyTofCheckBox);
  m_topicCheckBoxes.append(m_ui.resolutionTofCheckBox);
  m_topicCheckBoxes.append(m_ui.calibrationTofCheckBox);
  m_topicCheckBoxes.append(m_ui.miscellaneousTrackerCheckBox);
  m_topicCheckBoxes.append(m_ui.miscellaneousTRDCheckBox);
  m_topicCheckBoxes.append(m_ui.miscellaneousTOFCheckBox);
  m_topicCheckBoxes.append(m_ui.slowControlCheckBox);

  m_trackerCheckBoxes.append(m_ui.signalHeightTrackerCheckBox);
  m_trackerCheckBoxes.append(m_ui.clusterShapeTrackerCheckBox);
  m_trackerCheckBoxes.append(m_ui.trackingCheckBox);
  m_trackerCheckBoxes.append(m_ui.residualsTrackerCheckBox);
  m_trackerCheckBoxes.append(m_ui.momentumReconstructionCheckBox);
  m_trackerCheckBoxes.append(m_ui.miscellaneousTrackerCheckBox);

  m_trdCheckBoxes.append(m_ui.signalHeightTRDCheckBox);
  m_trdCheckBoxes.append(m_ui.clusterShapeTRDCheckBox);
  m_trdCheckBoxes.append(m_ui.residualsTRDCheckBox);
  m_trdCheckBoxes.append(m_ui.miscellaneousTRDCheckBox);

  m_tofCheckBoxes.append(m_ui.timeOverThresholdCheckBox);
  m_tofCheckBoxes.append(m_ui.momentumReconstructionCheckBox);
  m_tofCheckBoxes.append(m_ui.efficiencyTofCheckBox);
  m_tofCheckBoxes.append(m_ui.resolutionTofCheckBox);
  m_tofCheckBoxes.append(m_ui.calibrationTofCheckBox);
  m_tofCheckBoxes.append(m_ui.miscellaneousTOFCheckBox);
  
  foreach(QCheckBox* checkBox, m_topicCheckBoxes)
    m_controlWidgets.append(checkBox);
  m_controlWidgets.append(m_ui.selectAllButton);
  m_controlWidgets.append(m_ui.selectTrackerButton);
  m_controlWidgets.append(m_ui.selectTrdButton);
  m_controlWidgets.append(m_ui.selectTofButton);
  m_controlWidgets.append(m_ui.trackComboBox);
  m_controlWidgets.append(m_ui.firstEventSpinBox);
  m_controlWidgets.append(m_ui.lastEventSpinBox);
  m_controlWidgets.append(m_ui.numberOfThreadsSpinBox);
  m_controlWidgets.append(m_ui.alignmentCorrectionCheckBox);
  m_controlWidgets.append(m_ui.timeShiftCorrectionCheckBox);
  m_controlWidgets.append(m_ui.trdMopValueCorrectionCheckBox);
  m_controlWidgets.append(m_ui.timeOverThresholdCorrectionCheckBox);
  m_controlWidgets.append(m_ui.photonTravelTimeCorrectionCheckBox);

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
  connect(m_ui.savePngButton, SIGNAL(clicked()), this, SLOT(saveButtonsClicked()));
  connect(m_ui.saveRootButton, SIGNAL(clicked()), this, SLOT(saveButtonsClicked()));
  
  connect(m_ui.saveCanvasDialogAction, SIGNAL(triggered()), this, SLOT(saveCanvasDialogActionTriggered()));
  connect(m_ui.saveAllCanvasesDialogAction, SIGNAL(triggered()), this, SLOT(saveAllCanvasDialogActionTriggered()));
  connect(m_ui.saveForPostAnalysisAction, SIGNAL(triggered()), this, SLOT(saveForPostAnalysisActionTriggered()));
  connect(m_ui.saveForPostAnalysisDialogAction, SIGNAL(triggered()), this, SLOT(saveForPostAnalysisDialogActionTriggered()));
  connect(m_ui.setFileListDialogAction, SIGNAL(triggered()), this, SLOT(setOrAddFileListDialogActionTriggered()));
  connect(m_ui.addFileListDialogAction, SIGNAL(triggered()), this, SLOT(setOrAddFileListDialogActionTriggered()));
  connect(m_ui.quitAction, SIGNAL(triggered()), this, SLOT(close()));
  
  connect(m_ui.plotter, SIGNAL(titleChanged(const QString&)), this, SLOT(plotterTitleChanged(const QString&)));
  connect(m_ui.plotter, SIGNAL(positionChanged(double, double)), this, SLOT(plotterPositionChanged(double, double)));
  connect(m_ui.firstEventSpinBox, SIGNAL(valueChanged(int)), this, SLOT(firstOrLastEventChanged(int)));
  connect(m_ui.lastEventSpinBox, SIGNAL(valueChanged(int)), this, SLOT(firstOrLastEventChanged(int)));

  connect(m_ui.selectAllButton, SIGNAL(clicked()), this, SLOT(selectAllButtonClicked()));
  connect(m_ui.selectTrackerButton, SIGNAL(clicked()), this, SLOT(selectTrackerButtonClicked()));
  connect(m_ui.selectTrdButton, SIGNAL(clicked()), this, SLOT(selectTrdButtonClicked()));
  connect(m_ui.selectTofButton, SIGNAL(clicked()), this, SLOT(selectTofButtonClicked()));
  connect(m_ui.gridCheckBox, SIGNAL(stateChanged(int)), this, SLOT(gridCheckBoxChanged(int)));
  connect(m_ui.logXCheckBox, SIGNAL(stateChanged(int)), this, SLOT(logXCheckBoxChanged(int)));
  connect(m_ui.logYCheckBox, SIGNAL(stateChanged(int)), this, SLOT(logYCheckBoxChanged(int)));
  connect(m_ui.logZCheckBox, SIGNAL(stateChanged(int)), this, SLOT(logZCheckBoxChanged(int)));
  connect(m_ui.listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listWidgetItemChanged(QListWidgetItem*)));
  connect(m_ui.listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(listWidgetCurrentRowChanged(int)));

  connect(m_ui.signalHeightTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.signalHeightTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.clusterShapeTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.clusterShapeTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.timeOverThresholdButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.trackingButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.occupancyButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.residualsTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.residualsTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.momentumReconstructionButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.efficiencyTofButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.resolutionTofButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.calibrationTofButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTOFButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.slowControlButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));

  foreach(QCheckBox* checkBox, m_topicCheckBoxes)
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(checkBoxChanged()));

  setupPlots();

  m_updateTimer.setInterval(50);
  m_ui.numberOfThreadsSpinBox->setValue(QThread::idealThreadCount());
}

MainWindow::~MainWindow()
{
  qDeleteAll(m_processors);
  m_processors.clear();
  delete m_reader;
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

void MainWindow::showButtonsClicked()
{
  QPushButton* b = static_cast<QPushButton*>(sender());
  AnalysisPlot::Topic topic = AnalysisPlot::TopicEnd;
  if (b == m_ui.signalHeightTrackerButton) {
    topic = AnalysisPlot::SignalHeightTracker;
  } else if (b == m_ui.signalHeightTRDButton) {
    topic = AnalysisPlot::SignalHeightTRD;
  } else if (b == m_ui.clusterShapeTrackerButton) {
    topic = AnalysisPlot::ClusterShapeTracker;
  } else if (b == m_ui.clusterShapeTRDButton) {
    topic = AnalysisPlot::ClusterShapeTRD;
  } else if (b == m_ui.timeOverThresholdButton) {
    topic = AnalysisPlot::TimeOverThreshold;
  } else if (b == m_ui.trackingButton) {
    topic = AnalysisPlot::Tracking;
  } else if (b == m_ui.occupancyButton) {
    topic = AnalysisPlot::Occupancy;
  } else if (b == m_ui.residualsTrackerButton) {
    topic = AnalysisPlot::ResidualsTracker;
  } else if (b == m_ui.residualsTRDButton) {
    topic = AnalysisPlot::ResidualsTRD;
  } else if (b == m_ui.momentumReconstructionButton) {
    topic = AnalysisPlot::MomentumReconstruction;
  } else if (b == m_ui.efficiencyTofButton) {
    topic = AnalysisPlot::EfficiencyTOF;
  } else if (b == m_ui.resolutionTofButton) {
    topic = AnalysisPlot::ResolutionTOF;
  } else if (b == m_ui.calibrationTofButton) {
    topic = AnalysisPlot::CalibrationTOF;
  } else if (b == m_ui.miscellaneousTrackerButton) {
    topic = AnalysisPlot::MiscellaneousTracker;
  } else if (b == m_ui.miscellaneousTRDButton) {
    topic = AnalysisPlot::MiscellaneousTRD;
  } else if (b == m_ui.miscellaneousTOFButton) {
    topic = AnalysisPlot::MiscellaneousTOF;
  } else if (b == m_ui.slowControlButton) {
    topic = AnalysisPlot::SlowControl;
  }
  if (b->text() == "+") {
    b->setText("-");
    QVector<unsigned int> plotIndices = m_ui.plotter->plotIndices(topic);
    for (int i = 0; i < plotIndices.size(); ++i) {
      m_activePlots.append(plotIndices[i]);
      QListWidgetItem* item = new QListWidgetItem(m_ui.plotter->plotTitle(plotIndices[i]));
      item->setCheckState(Qt::Checked);
      item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
      m_ui.listWidget->addItem(item);
    }
  } else {
    b->setText("+");
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
  if (i < 0 || m_activePlots.size() == 0) {
    m_ui.plotter->selectPlot(-1);
    return;
  }
  if (!m_inhibitDraw)
    m_ui.plotter->selectPlot(m_activePlots[i]);
}

void MainWindow::setupPlots()
{
  Setup* setup = Setup::instance();
  const ElementIterator elementStartIt = setup->firstElement();
  const ElementIterator elementEndIt = setup->lastElement();
  const LayerIterator layerStartIt = setup->firstLayer();
  const LayerIterator layerEndIt = setup->lastLayer();
  ElementIterator elementIt;
  LayerIterator layerIt;

  m_ui.plotter->clearPlots();
  m_activePlots.clear();
  m_ui.listWidget->clear();

  if (m_ui.signalHeightTrackerCheckBox->isChecked()) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tracker)
        m_ui.plotter->addPlot(new SignalHeightPlot(AnalysisPlot::SignalHeightTracker, element->id()));
    }
  }
  if (m_ui.signalHeightTRDCheckBox->isChecked()) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::trd)
        m_ui.plotter->addPlot(new SignalHeightPlot(AnalysisPlot::SignalHeightTRD, element->id()));
    }

    m_ui.plotter->addPlot(new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, 0 /* doesnt matter */,TRDSpectrumPlot::completeTRD));
    m_ui.plotter->addPlot(new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, 0 /* doesnt matter */,TRDSpectrumPlot::completeTRD, -3, -1.5));
    m_ui.plotter->addPlot(new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, 0 /* doesnt matter */,TRDSpectrumPlot::completeTRD, 1.5, 3));
    
    TRDFitPlot* mpvModuleTRDPlot = new TRDFitPlot(AnalysisPlot::SignalHeightTRD, "MPVs of TRD Modules");
    TRDFitPlot* mpvChannelTRDPlot = new TRDFitPlot(AnalysisPlot::SignalHeightTRD, "MPVs of TRD Channels");

    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::trd){
        TRDSpectrumPlot* trdModuleSpectrumPlot = new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, element->id(),TRDSpectrumPlot::module);
        m_ui.plotter->addPlot(trdModuleSpectrumPlot);
        mpvModuleTRDPlot->addLandauFit(trdModuleSpectrumPlot->landauFit());
        for(unsigned short tubeNo = 0; tubeNo < 16; tubeNo++){
          TRDSpectrumPlot* trdChannelSpectrumPlot = new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, element->id() | tubeNo,TRDSpectrumPlot::channel);
          m_ui.plotter->addPlot(trdChannelSpectrumPlot);
          mpvChannelTRDPlot->addLandauFit(trdChannelSpectrumPlot->landauFit());
        }
      }
    }
    m_ui.plotter->addPlot(mpvModuleTRDPlot);
    m_ui.plotter->addPlot(mpvChannelTRDPlot);
  }
  if (m_ui.clusterShapeTrackerCheckBox->isChecked()) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tracker) {
        m_ui.plotter->addPlot(new ClusterLengthPlot(AnalysisPlot::ClusterShapeTracker, element->id()));
        m_ui.plotter->addPlot(new ClusterShapePlot(element->id()));
      }
    }
  }
  if (m_ui.clusterShapeTRDCheckBox->isChecked()) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::trd)
        m_ui.plotter->addPlot(new ClusterLengthPlot(AnalysisPlot::ClusterShapeTRD, element->id()));
    }
  }
  if (m_ui.timeOverThresholdCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new TOTPlot);
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof)
        for (int ch = 0; ch < 4; ++ch)
          m_ui.plotter->addPlot(new TOTMomentumCorrelation(element->id() | ch));
    }
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
          m_ui.plotter->addPlot(new TOTTimeCorrelationPlot(element->id() | ch));
    }
  }
  if (m_ui.trackingCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new BendingPositionPlot);
    m_ui.plotter->addPlot(new BendingAnglePlot);
    for (double cut = .004; cut < .008; cut+=.001)
      m_ui.plotter->addPlot(new BendingAnglePositionPlot(cut));
    for (unsigned short ndf = 10; ndf <= 20; ndf++)
      m_ui.plotter->addPlot(new Chi2Plot(ndf));
    m_ui.plotter->addPlot(new Chi2PerNdfPlot);
    m_ui.plotter->addPlot(new Chi2VsMomentumPlot);
  }
  if (m_ui.occupancyCheckBox->isChecked()) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      m_ui.plotter->addPlot(new GeometricOccupancyPlot(layer->z()));
      m_ui.plotter->addPlot(new GeometricOccupancyProjectionPlot(layer->z()));
    }
  }
  if (m_ui.residualsTrackerCheckBox->isChecked()) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (layer->z() > -240 && layer->z() < 240)
        m_ui.plotter->addPlot(new ResidualPlot(AnalysisPlot::ResidualsTracker, layer));
    }
  }
  if (m_ui.residualsTRDCheckBox->isChecked()) {
    for (layerIt = layerStartIt; layerIt != layerEndIt; ++layerIt) {
      Layer* layer = *layerIt;
      if (layer->z() > -520 && layer->z() < -240)
        m_ui.plotter->addPlot(new ResidualPlot(AnalysisPlot::ResidualsTRD, layer));
    }
  }
  if (m_ui.momentumReconstructionCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new BetaMomentumCorrelationPlot());
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(MomentumSpectrumPlot::All));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(MomentumSpectrumPlot::Positive));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(MomentumSpectrumPlot::Negative));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(MomentumSpectrumPlot::Inverted));
    m_ui.plotter->addPlot(new AlbedosVsMomentumPlot());
  }
  if (m_ui.efficiencyTofCheckBox->isChecked()) {
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof) {
        for (int ch = 0; ch < 4; ++ch) {
          m_ui.plotter->addPlot(new TOFEfficiencyPlot(element->id() | ch));
        }
      }
    }
  }
  if (m_ui.resolutionTofCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8000, 0x8010, 0x8020, 0x8030));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8000, 0x8010, 0x8024, 0x8034));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8000, 0x8010, 0x8028, 0x8038));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8000, 0x8010, 0x802c, 0x803c));
    
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8004, 0x8014, 0x8020, 0x8030));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8004, 0x8014, 0x8024, 0x8034));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8004, 0x8014, 0x8028, 0x8038));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8004, 0x8014, 0x802c, 0x803c));

    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8008, 0x8018, 0x8020, 0x8030));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8008, 0x8018, 0x8024, 0x8034));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8008, 0x8018, 0x8028, 0x8038));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8008, 0x8018, 0x802c, 0x803c));
    
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x800c, 0x801c, 0x8020, 0x8030));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x800c, 0x801c, 0x8024, 0x8034));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x800c, 0x801c, 0x8028, 0x8038));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x800c, 0x801c, 0x802c, 0x803c));
  }
  if (m_ui.calibrationTofCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new ChannelTriggerProbabilityPlot);
    m_ui.plotter->addPlot(new TOFTimeShiftTriggerPlot);
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof)
        m_ui.plotter->addPlot(new TOFTimeDifferencePlot(element->id()));
    }
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8000, 0x8010));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8004, 0x8014));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8008, 0x8018));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x800c, 0x801c));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8020, 0x8030));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8024, 0x8034));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8028, 0x8038));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x802c, 0x803c));
  }
  if (m_ui.miscellaneousTrackerCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new TotalSignalHeightPlot);
    m_ui.plotter->addPlot(new CutStatisticsPlot);
    m_ui.plotter->addPlot(new TrackerLayerStatisticsPlot);
    m_ui.plotter->addPlot(new TrackingEfficiencyVsMomentumPlot);
    m_ui.plotter->addPlot(new SingleLayerTrackingEfficiencyPlot);
    m_ui.plotter->addPlot(new MultiLayerTrackingEfficiencyPlot);
  }
  if (m_ui.miscellaneousTRDCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new TRDClustersOnTrackPlot(AnalysisPlot::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TRDDistanceWireToTrackPlot(AnalysisPlot::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TRDDistanceInTube(AnalysisPlot::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TotalEnergyDepositionPlot(-100,100));
    m_ui.plotter->addPlot(new TRDEnergyDepositionOverMomentumPlot(AnalysisPlot::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TotalEnergyDepositionTRDvsTrackerPlot());
    m_ui.plotter->addPlot(new TRDEfficiencyPlot());
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::numberOfHits));
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::numberOfHits, true));
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::sumOfSignalHeights));
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::sumOfSignalHeights, true));
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::sumOfSignalHeightsNormalizedToHits));
    m_ui.plotter->addPlot(new TRDOccupancyPlot(TRDOccupancyPlot::sumOfSignalHeightsNormalizedToHits, true));
  }
  if (m_ui.miscellaneousTOFCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new BetaPlot());
    for (elementIt = elementStartIt; elementIt != elementEndIt; ++elementIt) {
      DetectorElement* element = *elementIt;
      if (element->type() == DetectorElement::tof)
        m_ui.plotter->addPlot(new TOFPositionCorrelationPlot(element->id()));
    }
    //m_ui.plotter->addPlot(new TOFAlignment);
  }
  if (m_ui.slowControlCheckBox->isChecked()) {
    QDateTime first = m_reader->time(m_ui.firstEventSpinBox->value());
    QDateTime last = m_reader->time(m_ui.lastEventSpinBox->value());
    QVector<SensorTypes::Type> temperatureSensors = QVector<SensorTypes::Type>::fromStdVector(SensorTypes::temperatureSensors());
    foreach(SensorTypes::Type sensor, temperatureSensors)
      m_ui.plotter->addPlot(new TemperatureTimePlot(sensor, first, last));
  }
}

void MainWindow::setupAnalysis(Track::Type& type, Corrections::Flags& flags)
{
  if (m_ui.alignmentCorrectionCheckBox->isChecked())
    flags|= Corrections::Alignment;
  if (m_ui.timeShiftCorrectionCheckBox->isChecked())
    flags|= Corrections::TimeShifts;
  if (m_ui.photonTravelTimeCorrectionCheckBox->isChecked())
    flags|= Corrections::PhotonTravelTime;
  if (m_ui.trdMopValueCorrectionCheckBox->isChecked())
    flags|= Corrections::TrdMopv;
  if (m_ui.timeOverThresholdCorrectionCheckBox->isChecked())
    flags|= Corrections::TofTimeOverThreshold;

  if (m_ui.trackComboBox->currentText() == "centered broken line") {
    type = Track::CenteredBrokenLine;
  } else if (m_ui.trackComboBox->currentText() == "centered broken line 2D") {
    type = Track::CenteredBrokenLine2D;
  } else if (m_ui.trackComboBox->currentText() == "broken line") {
    type = Track::BrokenLine;
  } else if (m_ui.trackComboBox->currentText() == "straight line") {
    type = Track::StraightLine;
  } else if (m_ui.trackComboBox->currentText() == "none") {
    type = Track::None;
  }
 
  m_ui.signalHeightTrackerButton->setText("+");
  m_ui.signalHeightTRDButton->setText("+");
  m_ui.clusterShapeTrackerButton->setText("+");
  m_ui.clusterShapeTRDButton->setText("+");
  m_ui.timeOverThresholdButton->setText("+");
  m_ui.trackingButton->setText("+");
  m_ui.occupancyButton->setText("+");
  m_ui.residualsTrackerButton->setText("+");
  m_ui.residualsTRDButton->setText("+");
  m_ui.momentumReconstructionButton->setText("+");
  m_ui.efficiencyTofButton->setText("+");
  m_ui.resolutionTofButton->setText("+");
  m_ui.calibrationTofButton->setText("+");
  m_ui.miscellaneousTrackerButton->setText("+");
  m_ui.miscellaneousTRDButton->setText("+");
  m_ui.miscellaneousTOFButton->setText("+");
  m_ui.slowControlButton->setText("+");

  m_ui.signalHeightTrackerButton->setEnabled(m_ui.signalHeightTrackerCheckBox->isChecked());
  m_ui.signalHeightTRDButton->setEnabled(m_ui.signalHeightTRDCheckBox->isChecked());
  m_ui.clusterShapeTrackerButton->setEnabled(m_ui.clusterShapeTrackerCheckBox->isChecked());
  m_ui.clusterShapeTRDButton->setEnabled(m_ui.clusterShapeTRDCheckBox->isChecked());
  m_ui.timeOverThresholdButton->setEnabled(m_ui.timeOverThresholdCheckBox->isChecked());
  m_ui.trackingButton->setEnabled(m_ui.trackingCheckBox->isChecked());
  m_ui.occupancyButton->setEnabled(m_ui.occupancyCheckBox->isChecked());
  m_ui.residualsTrackerButton->setEnabled(m_ui.residualsTrackerCheckBox->isChecked());
  m_ui.residualsTRDButton->setEnabled(m_ui.residualsTRDCheckBox->isChecked());
  m_ui.momentumReconstructionButton->setEnabled(m_ui.momentumReconstructionCheckBox->isChecked());
  m_ui.efficiencyTofButton->setEnabled(m_ui.efficiencyTofCheckBox->isChecked());
  m_ui.resolutionTofButton->setEnabled(m_ui.resolutionTofCheckBox->isChecked());
  m_ui.calibrationTofButton->setEnabled(m_ui.calibrationTofCheckBox->isChecked());
  m_ui.miscellaneousTrackerButton->setEnabled(m_ui.miscellaneousTrackerCheckBox->isChecked());
  m_ui.miscellaneousTRDButton->setEnabled(m_ui.miscellaneousTRDCheckBox->isChecked());
  m_ui.miscellaneousTOFButton->setEnabled(m_ui.miscellaneousTOFCheckBox->isChecked());
  m_ui.slowControlButton->setEnabled(m_ui.slowControlCheckBox->isChecked());
}

void MainWindow::analyzeButtonClicked()
{
  if (m_ui.analyzeButton->text() == "&start") {
    Track::Type type;
    Corrections::Flags flags;
    setupAnalysis(type, flags);
    setupPlots();

    qDeleteAll(m_processors);
    m_processors.clear();
    for (int i = 0; i < m_ui.numberOfThreadsSpinBox->value(); i++) {
      AnalysisProcessor* processor = new AnalysisProcessor;
      foreach(AnalysisPlot* plot, m_ui.plotter->plots())
        processor->addDestination(plot);
      processor->setTrackType(type);
      processor->setCorrectionFlags(flags);
      m_processors.append(processor);
    }      
    m_reader->start(m_processors, m_ui.firstEventSpinBox->value(), m_ui.lastEventSpinBox->value());
  } else {
    m_reader->stop();
  }
}

void MainWindow::setOrAddFileListDialogActionTriggered()
{
  QStringList files = QFileDialog::getOpenFileNames(this,
    "Select one or more file lists to open", "", "*.txt;;*.*;;*");
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
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "All Files(*.*);;" + filters.join(";;"), &selectedFilter);

  if (fileName.isEmpty())
    return;

  // if file name contains an ending, use that. Otherwise use selected filter
  QString fileEnding;
  if (fileName.contains('.')) {
    fileEnding = fileName.split('.').last().toLower();
  }
  else {
    fileEnding = selectedFilter.split("(").first().toLower();
  }

  // if filter == all, save all endings, otherwise use previously determined ending
  if(fileEnding == "all files"){
    foreach(QString fileFormatEnding, fileFormatEndings)
      m_ui.plotter->saveCanvas(fileName + "." + fileFormatEnding);
  }
  else{
    if (!fileEnding.startsWith('.'))
      fileEnding.prepend('.');
    if (!fileName.endsWith(fileEnding))
      fileName.append(fileEnding);
    m_ui.plotter->saveCanvas(fileName);
  }
}

void MainWindow::saveAllCanvasDialogActionTriggered()
{
  QFileDialog dialog(this, "save all canvases displayed", ".");
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  if (dialog.exec())
    for (int i = 0; i < m_ui.listWidget->count(); ++i) {
      m_ui.listWidget->setCurrentRow(i);
      QString directoryName = dialog.selectedFiles().first();
      m_ui.plotter->saveCanvas(directoryName + '/' + m_ui.plotter->plotTitle(m_activePlots[i]) + ".svg");
      m_ui.plotter->saveCanvas(directoryName + '/' + m_ui.plotter->plotTitle(m_activePlots[i]) + ".pdf");
      m_ui.plotter->saveCanvas(directoryName + '/' + m_ui.plotter->plotTitle(m_activePlots[i]) + ".root");
      m_ui.plotter->saveCanvas(directoryName + '/' + m_ui.plotter->plotTitle(m_activePlots[i]) + ".png");
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
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "*.root", &fileEnding);
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

void MainWindow::changeTopicGroupStatus(QVector<QCheckBox*>& group)
{
  bool groupAlreadyChecked = true;
  foreach(QCheckBox* checkBox, group)
    groupAlreadyChecked = groupAlreadyChecked && checkBox->isChecked();

  bool newState = groupAlreadyChecked ? false : true;
  foreach(QCheckBox* checkBox, group)
    checkBox->setChecked(newState);

  checkSelectAll();
}
 
void MainWindow::checkSelectAll()
{
  bool allTopicsChecked = true;
  foreach(QCheckBox* checkBox, m_topicCheckBoxes)
    allTopicsChecked = allTopicsChecked && checkBox->isChecked();

  m_ui.selectAllButton->setText(allTopicsChecked ? "deselect &all" : "select &all");
}

void MainWindow::selectAllButtonClicked()
{
  changeTopicGroupStatus(m_topicCheckBoxes);
}

void MainWindow::selectTrackerButtonClicked()
{
  changeTopicGroupStatus(m_trackerCheckBoxes);
}

void MainWindow::selectTrdButtonClicked()
{
  changeTopicGroupStatus(m_trdCheckBoxes);
}
 
void MainWindow::selectTofButtonClicked()
{
  changeTopicGroupStatus(m_tofCheckBoxes);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  m_reader->stop();
  event->accept();
}

void MainWindow::gridCheckBoxChanged(int value)
{
  m_ui.plotter->setGrid(value);
}

void MainWindow::logXCheckBoxChanged(int value)
{
  m_ui.plotter->setLogX(value);
}

void MainWindow::logYCheckBoxChanged(int value)
{
  m_ui.plotter->setLogY(value);
}

void MainWindow::logZCheckBoxChanged(int value)
{
  m_ui.plotter->setLogZ(value);
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
  firstOrLastEventChanged();
}

void MainWindow::toggleControlWidgetsStatus()
{
  foreach(QWidget* widget, m_controlWidgets)
    widget->setEnabled(!widget->isEnabled());
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
