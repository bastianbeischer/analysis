#include "MainWindow.hh"

#include "Plotter.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "AnalysisPlot.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "Corrections.hh"

#include "BendingPositionPlot.hh"
#include "BendingAnglePlot.hh"
#include "ResidualPlot.hh"
#include "GeometricOccupancyPlot.hh"
#include "GeometricOccupancyProjectionPlot.hh"
#include "BendingAnglePositionPlot.hh"
#include "Chi2Plot.hh"
#include "Chi2PerNdfPlot.hh"
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
#include "TimeResolutionPlot.hh"
#include "TOFTimeDifferencePlot.hh"
#include "TotalSignalHeightPlot.hh"
#include "TOFEfficiencyPlot.hh"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_activePlots()
{
  m_ui.setupUi(this);
  
  m_ui.plotter->setTitleLabel(m_ui.titleLabel);
  m_ui.plotter->setPositionLabel(m_ui.positionLabel);
  m_ui.plotter->setTimeLabel(m_ui.timeLabel);
  m_ui.plotter->setDataChainProgressBar(m_ui.dataChainProgressBar);
  m_ui.plotter->setEventQueueProgressBar(m_ui.eventQueueProgressBar);

  connect(m_ui.analyzeButton, SIGNAL(clicked()), this, SLOT(analyzeButtonClicked()));
  connect(m_ui.saveCanvasButton, SIGNAL(clicked()), this, SLOT(saveCanvasButtonClicked()));
  connect(m_ui.saveAllCanvasesButton, SIGNAL(clicked()), this, SLOT(saveAllCanvasButtonClicked()));
  connect(m_ui.saveForPostAnalysisButton, SIGNAL(clicked()), this, SLOT(saveForPostAnalysisButtonClicked()));
  connect(m_ui.toggleSelectionButton, SIGNAL(clicked()), this, SLOT(toggleSelectionButtonClicked()));
  connect(m_ui.setFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.addFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.toggleGridButton, SIGNAL(clicked()), this, SLOT(toggleGridButtonClicked()));
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
  connect(m_ui.miscellaneousTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTOFButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));

  setupPlots();
}

MainWindow::~MainWindow()
{
}

void MainWindow::processArguments(QStringList arguments)
{
  arguments.removeFirst();
  QRegExp onlyDigits("^\\d+$");
  foreach(QString argument, arguments) {
    if (!onlyDigits.exactMatch(argument)) {
      if (argument.endsWith(".root"))
        m_ui.plotter->addRootFile(argument);
      else
        m_ui.plotter->addFileList(argument);
    }
  }
  QStringList eventRange = arguments.filter(onlyDigits);
  if (eventRange.size() == 2) {
    int firstEvent = eventRange.at(0).toInt();
    int lastEvent = eventRange.at(1).toInt();
    m_ui.plotter->setFirstEvent(firstEvent);
    m_ui.plotter->setLastEvent(lastEvent);
    qDebug() << firstEvent << " " << lastEvent;
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
  } else if (b == m_ui.miscellaneousTrackerButton) {
    topic = AnalysisPlot::MiscellaneousTracker;
  } else if (b == m_ui.miscellaneousTRDButton) {
    topic = AnalysisPlot::MiscellaneousTRD;
  } else if (b == m_ui.miscellaneousTOFButton) {
    topic = AnalysisPlot::MiscellaneousTOF;
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
    for (int i = m_ui.listWidget->count() - 1; i >= 0; --i) {
      if (m_ui.plotter->plotTopic(m_activePlots[i]) == topic)
        removeListWidgetItem(i);
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
  m_ui.plotter->selectPlot(m_activePlots[i]);
}

void MainWindow::setupPlots()
{
  Setup* setup = Setup::instance();

  m_ui.plotter->clearPlots();
  m_activePlots.clear();
  m_ui.listWidget->clear();
  if (m_ui.signalHeightTrackerCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::tracker)
        m_ui.plotter->addPlot(new SignalHeightPlot(AnalysisPlot::SignalHeightTracker, element->id()));
      element = setup->nextElement();
    }
  }
  if (m_ui.signalHeightTRDCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::trd)
        m_ui.plotter->addPlot(new SignalHeightPlot(AnalysisPlot::SignalHeightTRD, element->id()));
      element = setup->nextElement();
    }

    m_ui.plotter->addPlot(new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, 0 /* doesnt matter */,TRDSpectrumPlot::completeTRD));
    m_ui.plotter->addPlot(new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, 0 /* doesnt matter */,TRDSpectrumPlot::completeTRD, -3, -1.5));
    m_ui.plotter->addPlot(new TRDSpectrumPlot(AnalysisPlot::SignalHeightTRD, 0 /* doesnt matter */,TRDSpectrumPlot::completeTRD, 1.5, 3));
    
    TRDFitPlot* mpvModuleTRDPlot = new TRDFitPlot(AnalysisPlot::SignalHeightTRD, "MPVs of TRD Modules");
    TRDFitPlot* mpvChannelTRDPlot = new TRDFitPlot(AnalysisPlot::SignalHeightTRD, "MPVs of TRD Channels");
    element = setup->firstElement();
    while(element) {
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
      element = setup->nextElement();
    }
    m_ui.plotter->addPlot(mpvModuleTRDPlot);
    m_ui.plotter->addPlot(mpvChannelTRDPlot);
  }
  if (m_ui.clusterShapeTrackerCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::tracker) {
        m_ui.plotter->addPlot(new ClusterLengthPlot(AnalysisPlot::ClusterShapeTracker, element->id()));
        m_ui.plotter->addPlot(new ClusterShapePlot(element->id()));
      }
      element = setup->nextElement();
    }
  }
  if (m_ui.clusterShapeTRDCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::trd)
        m_ui.plotter->addPlot(new ClusterLengthPlot(AnalysisPlot::ClusterShapeTRD, element->id()));
      element = setup->nextElement();
    }
  }
  if (m_ui.timeOverThresholdCheckBox->isChecked()) {
  }
  if (m_ui.trackingCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new BendingPositionPlot);
    m_ui.plotter->addPlot(new BendingAnglePlot);
    for (double cut = .004; cut < .008; cut+=.001)
      m_ui.plotter->addPlot(new BendingAnglePositionPlot(cut));
    for (unsigned short ndf = 10; ndf <= 20; ndf++)
      m_ui.plotter->addPlot(new Chi2Plot(ndf));
    m_ui.plotter->addPlot(new Chi2PerNdfPlot);
  }
  if (m_ui.occupancyCheckBox->isChecked()) {
    Layer* layer = setup->firstLayer();
    while(layer) {
      m_ui.plotter->addPlot(new GeometricOccupancyPlot(layer->z()));
      m_ui.plotter->addPlot(new GeometricOccupancyProjectionPlot(layer->z()));
      layer = setup->nextLayer();
    }
  }
  if (m_ui.residualsTrackerCheckBox->isChecked()) {
    Layer* layer = setup->firstLayer();
    while(layer) {
      if (layer->z() > -240 && layer->z() < 240)
        m_ui.plotter->addPlot(new ResidualPlot(AnalysisPlot::ResidualsTracker, layer));
      layer = setup->nextLayer();
    }
  }
  if (m_ui.residualsTRDCheckBox->isChecked()) {
    Layer* layer = setup->firstLayer();
    while(layer) {
      if (layer->z() > -520 && layer->z() < -240)
        m_ui.plotter->addPlot(new ResidualPlot(AnalysisPlot::ResidualsTRD, layer));
      layer = setup->nextLayer();
    }
  }
  if (m_ui.momentumReconstructionCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(MomentumSpectrumPlot::All));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(MomentumSpectrumPlot::Positive));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(MomentumSpectrumPlot::Negative));
    m_ui.plotter->addPlot(new MomentumSpectrumPlot(MomentumSpectrumPlot::Inverted));
    m_ui.plotter->addPlot(new AlbedosVsMomentumPlot());
  }
  if (m_ui.efficiencyTofCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while (element) {
      if (element->type() == DetectorElement::tof) {
        for (int ch = 0; ch < 4; ++ch) {
          m_ui.plotter->addPlot(new TOFEfficiencyPlot(element->id() | ch));
        }
      }
      element = setup->nextElement();
    }
  }
  if (m_ui.miscellaneousTrackerCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new TotalSignalHeightPlot);
    m_ui.plotter->addPlot(new CutStatisticsPlot);
    m_ui.plotter->addPlot(new TrackerLayerStatisticsPlot);
  }
  if (m_ui.miscellaneousTRDCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new TRDClustersOnTrackPlot(AnalysisPlot::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TRDDistanceWireToTrackPlot(AnalysisPlot::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TRDDistanceInTube(AnalysisPlot::MiscellaneousTRD));
    m_ui.plotter->addPlot(new TotalEnergyDepositionPlot(2,10));
    m_ui.plotter->addPlot(new TRDEnergyDepositionOverMomentumPlot(AnalysisPlot::MiscellaneousTRD));
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
    m_ui.plotter->addPlot(new BetaMomentumCorrelationPlot());

    DetectorElement* element = 0;
    
    element = setup->firstElement();
    while (element) {
      if (element->type() == DetectorElement::tof)
        m_ui.plotter->addPlot(new TOFPositionCorrelationPlot(element->id()));
      element = setup->nextElement();
    }

    element = setup->firstElement();
    while (element) {
      if (element->type() == DetectorElement::tof)
        m_ui.plotter->addPlot(new TOFTimeDifferencePlot(element->id()));
      element = setup->nextElement();
    }

    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8000, 0x8010));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8004, 0x8014));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8008, 0x8018));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x800c, 0x801c));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8020, 0x8030));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8024, 0x8034));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x8028, 0x8038));
    m_ui.plotter->addPlot(new TOFTimeShiftPlot(0x802c, 0x803c));

    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8000, 0x8010, 0x8020, 0x8030));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8004, 0x8014, 0x8024, 0x8034));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x8008, 0x8018, 0x8028, 0x8038));
    m_ui.plotter->addPlot(new TimeResolutionPlot(0x800c, 0x801c, 0x802c, 0x803c));
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
  m_ui.miscellaneousTrackerButton->setText("+");
  m_ui.miscellaneousTRDButton->setText("+");
  m_ui.miscellaneousTOFButton->setText("+");

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
  m_ui.miscellaneousTrackerButton->setEnabled(m_ui.miscellaneousTrackerCheckBox->isChecked());
  m_ui.miscellaneousTRDButton->setEnabled(m_ui.miscellaneousTRDCheckBox->isChecked());
  m_ui.miscellaneousTOFButton->setEnabled(m_ui.miscellaneousTOFCheckBox->isChecked());
}

void MainWindow::analyzeButtonClicked()
{
  if (m_ui.analyzeButton->text() == "start analysis") {
    m_ui.analyzeButton->setText("abort analysis");
    m_ui.trackComboBox->setEnabled(false);
    Track::Type type;
    Corrections::Flags flags;
    setupAnalysis(type, flags);
    setupPlots();
    m_ui.plotter->startAnalysis(type, flags, m_ui.numberOfThreadsSpinBox->value());
  } else {
    m_ui.plotter->abortAnalysis();
    m_ui.trackComboBox->setEnabled(true);
    m_ui.analyzeButton->setText("start analysis");
  }
}

void MainWindow::setOrAddFileListButtonClicked()
{
  QStringList files = QFileDialog::getOpenFileNames(this,
    "Select one or more file lists to open", "", "*.txt;;*.*;;*");
  if (sender() == m_ui.setFileListButton) {
    foreach(QString file, files)
      m_ui.plotter->setFileList(file);
  } else if (sender() == m_ui.addFileListButton) {
    foreach(QString file, files)
      m_ui.plotter->addFileList(file);
  }
}

void MainWindow::saveCanvasButtonClicked()
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

void MainWindow::saveAllCanvasButtonClicked()
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

void MainWindow::saveForPostAnalysisButtonClicked()
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

void MainWindow::toggleSelectionButtonClicked()
{
  bool b = m_ui.toggleSelectionButton->text() == "select all";
  m_ui.toggleSelectionButton->setText(b ? "deselect all" : "select all");
  m_ui.signalHeightTrackerCheckBox->setChecked(b);
  m_ui.signalHeightTRDCheckBox->setChecked(b);
  m_ui.clusterShapeTrackerCheckBox->setChecked(b);
  m_ui.clusterShapeTRDCheckBox->setChecked(b);
  m_ui.timeOverThresholdCheckBox->setChecked(b);
  m_ui.trackingCheckBox->setChecked(b);
  m_ui.occupancyCheckBox->setChecked(b);
  m_ui.residualsTrackerCheckBox->setChecked(b);
  m_ui.residualsTRDCheckBox->setChecked(b);
  m_ui.momentumReconstructionCheckBox->setChecked(b);
  m_ui.efficiencyTofCheckBox->setChecked(b);
  m_ui.resolutionTofCheckBox->setChecked(b);
  m_ui.miscellaneousTrackerCheckBox->setChecked(b);
  m_ui.miscellaneousTRDCheckBox->setChecked(b);
  m_ui.miscellaneousTOFCheckBox->setChecked(b);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  m_ui.plotter->abortAnalysis();
  event->accept();
}

void MainWindow::toggleGridButtonClicked()
{
  if (m_ui.toggleGridButton->text() == "show grid") {
    m_ui.plotter->setGrid(true);
    m_ui.toggleGridButton->setText("hide grid");
  } else {
    m_ui.plotter->setGrid(false);
    m_ui.toggleGridButton->setText("show grid");
  }
}
