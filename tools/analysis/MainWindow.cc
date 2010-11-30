#include "MainWindow.hh"

#include "Plotter.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "AnalysisPlot.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"

#include "BendingPositionPlot.hh"
#include "BendingAnglePlot.hh"
#include "ResidualPlot.hh"
#include "GeometricOccupancyPlot.hh"
#include "GeometricOccupancyProjectionPlot.hh"
#include "BendingAnglePositionPlot.hh"
#include "Chi2Plot.hh"
#include "TOFPositionCorrelationPlot.hh"
#include "MomentumSpectrumPlot.hh"
#include "SignalHeightPlot.hh"
#include "ClusterLengthPlot.hh"
#include "TimeOfFlightPlot.hh"

#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>

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
  connect(m_ui.setFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.addFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(listWidgetItemChanged(QListWidgetItem*)));
  connect(m_ui.listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(listWidgetCurrentRowChanged(int)));

  connect(m_ui.signalHeightUpperTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.signalHeightLowerTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.signalHeightTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.clusterLengthUpperTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.clusterLengthLowerTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.clusterLengthTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.timeOverThresholdButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.trackingButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.occupancyButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.residualsUpperTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.residualsLowerTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.residualsTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.momentumReconstructionButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.miscellaneousTOFButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));

  setupAnalysis();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showButtonsClicked()
{
  QPushButton* b = static_cast<QPushButton*>(sender());
  AnalysisPlot::Topic topic = AnalysisPlot::TopicEnd;
  if (b == m_ui.signalHeightUpperTrackerButton) {
    topic = AnalysisPlot::SignalHeightUpperTracker;
  } else if (b == m_ui.signalHeightLowerTrackerButton) {
    topic = AnalysisPlot::SignalHeightLowerTracker;
  } else if (b == m_ui.signalHeightTRDButton) {
    topic = AnalysisPlot::SignalHeightTRD;
  } else if (b == m_ui.clusterLengthUpperTrackerButton) {
    topic = AnalysisPlot::ClusterLengthUpperTracker;
  } else if (b == m_ui.clusterLengthLowerTrackerButton) {
    topic = AnalysisPlot::ClusterLengthLowerTracker;
  } else if (b == m_ui.clusterLengthTRDButton) {
    topic = AnalysisPlot::ClusterLengthTRD;
  } else if (b == m_ui.timeOverThresholdButton) {
    topic = AnalysisPlot::TimeOverThreshold;
  } else if (b == m_ui.trackingButton) {
    topic = AnalysisPlot::Tracking;
  } else if (b == m_ui.occupancyButton) {
    topic = AnalysisPlot::Occupancy;
  } else if (b == m_ui.residualsUpperTrackerButton) {
    topic = AnalysisPlot::ResidualsUpperTracker;
  } else if (b == m_ui.residualsLowerTrackerButton) {
    topic = AnalysisPlot::ResidualsLowerTracker;
  } else if (b == m_ui.residualsTRDButton) {
    topic = AnalysisPlot::ResidualsTRD;
  } else if (b == m_ui.momentumReconstructionButton) {
    topic = AnalysisPlot::MomentumReconstruction;
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

void MainWindow::setupAnalysis()
{
  Setup* setup = Setup::instance();

  m_ui.plotter->clearPlots();
  m_activePlots.clear();
  m_ui.listWidget->clear();
  if (m_ui.signalHeightUpperTrackerCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::tracker && element->position().z() > 0)
        m_ui.plotter->addPlot(new SignalHeightPlot(AnalysisPlot::SignalHeightUpperTracker, element->id()));
      element = setup->nextElement();
    }
  }
  if (m_ui.signalHeightLowerTrackerCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::tracker && element->position().z() < 0)
        m_ui.plotter->addPlot(new SignalHeightPlot(AnalysisPlot::SignalHeightLowerTracker, element->id()));
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
  }
  if (m_ui.clusterLengthUpperTrackerCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::tracker && element->position().z() > 0)
        m_ui.plotter->addPlot(new ClusterLengthPlot(AnalysisPlot::ClusterLengthUpperTracker, element->id()));
      element = setup->nextElement();
    }
  }
  if (m_ui.clusterLengthLowerTrackerCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::tracker && element->position().z() < 0)
        m_ui.plotter->addPlot(new ClusterLengthPlot(AnalysisPlot::ClusterLengthLowerTracker, element->id()));
      element = setup->nextElement();
    }
  }
  if (m_ui.clusterLengthTRDCheckBox->isChecked()) {
    DetectorElement* element = setup->firstElement();
    while(element) {
      if (element->type() == DetectorElement::trd)
        m_ui.plotter->addPlot(new ClusterLengthPlot(AnalysisPlot::ClusterLengthTRD, element->id()));
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
    m_ui.plotter->addPlot(new Chi2Plot);
  }
  if (m_ui.occupancyCheckBox->isChecked()) {
    Layer* layer = setup->firstLayer();
    while(layer) {
      m_ui.plotter->addPlot(new GeometricOccupancyPlot(layer->z()));
      m_ui.plotter->addPlot(new GeometricOccupancyProjectionPlot(layer->z()));
      layer = setup->nextLayer();
    }
  }
  if (m_ui.residualsUpperTrackerCheckBox->isChecked()) {
    Layer* layer = setup->firstLayer();
    while(layer) {
      if (layer->z() > 0 && layer->z() < 240)
        m_ui.plotter->addPlot(new ResidualPlot(AnalysisPlot::ResidualsUpperTracker, layer));
      layer = setup->nextLayer();
    }
  }
  if (m_ui.residualsLowerTrackerCheckBox->isChecked()) {
    Layer* layer = setup->firstLayer();
    while(layer) {
      if (layer->z() > -240 && layer->z() < 0)
        m_ui.plotter->addPlot(new ResidualPlot(AnalysisPlot::ResidualsLowerTracker, layer));
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
    m_ui.plotter->addPlot(new MomentumSpectrumPlot);
  }
  if (m_ui.miscellaneousTrackerCheckBox->isChecked()) {
  }
  if (m_ui.miscellaneousTRDCheckBox->isChecked()) {
  }
  if (m_ui.miscellaneousTOFCheckBox->isChecked()) {
    m_ui.plotter->addPlot(new TimeOfFlightPlot());
    DetectorElement* element = setup->firstElement();
    while (element) {
      if (element->type() == DetectorElement::tof)
        m_ui.plotter->addPlot(new TOFPositionCorrelationPlot(element->id()));
      element = setup->nextElement();
    }
  }
  
  m_ui.signalHeightUpperTrackerButton->setText("+");
  m_ui.signalHeightLowerTrackerButton->setText("+");
  m_ui.signalHeightTRDButton->setText("+");
  m_ui.clusterLengthUpperTrackerButton->setText("+");
  m_ui.clusterLengthLowerTrackerButton->setText("+");
  m_ui.clusterLengthTRDButton->setText("+");
  m_ui.timeOverThresholdButton->setText("+");
  m_ui.trackingButton->setText("+");
  m_ui.occupancyButton->setText("+");
  m_ui.residualsUpperTrackerButton->setText("+");
  m_ui.residualsLowerTrackerButton->setText("+");
  m_ui.residualsTRDButton->setText("+");
  m_ui.momentumReconstructionButton->setText("+");
  m_ui.miscellaneousTrackerButton->setText("+");
  m_ui.miscellaneousTRDButton->setText("+");
  m_ui.miscellaneousTOFButton->setText("+");

  m_ui.signalHeightUpperTrackerButton->setEnabled(m_ui.signalHeightUpperTrackerCheckBox->isChecked());
  m_ui.signalHeightLowerTrackerButton->setEnabled(m_ui.signalHeightLowerTrackerCheckBox->isChecked());
  m_ui.signalHeightTRDButton->setEnabled(m_ui.signalHeightTRDCheckBox->isChecked());
  m_ui.clusterLengthUpperTrackerButton->setEnabled(m_ui.clusterLengthUpperTrackerCheckBox->isChecked());
  m_ui.clusterLengthLowerTrackerButton->setEnabled(m_ui.clusterLengthLowerTrackerCheckBox->isChecked());
  m_ui.clusterLengthTRDButton->setEnabled(m_ui.clusterLengthTRDCheckBox->isChecked());
  m_ui.timeOverThresholdButton->setEnabled(m_ui.timeOverThresholdCheckBox->isChecked());
  m_ui.trackingButton->setEnabled(m_ui.trackingCheckBox->isChecked());
  m_ui.occupancyButton->setEnabled(m_ui.occupancyCheckBox->isChecked());
  m_ui.residualsUpperTrackerButton->setEnabled(m_ui.residualsUpperTrackerCheckBox->isChecked());
  m_ui.residualsLowerTrackerButton->setEnabled(m_ui.residualsLowerTrackerCheckBox->isChecked());
  m_ui.residualsTRDButton->setEnabled(m_ui.residualsTRDCheckBox->isChecked());
  m_ui.momentumReconstructionButton->setEnabled(m_ui.momentumReconstructionCheckBox->isChecked());
  m_ui.miscellaneousTrackerButton->setEnabled(m_ui.miscellaneousTrackerCheckBox->isChecked());
  m_ui.miscellaneousTRDButton->setEnabled(m_ui.miscellaneousTRDCheckBox->isChecked());
  m_ui.miscellaneousTOFButton->setEnabled(m_ui.miscellaneousTOFCheckBox->isChecked());

}

void MainWindow::analyzeButtonClicked()
{
  if (m_ui.analyzeButton->text() == "start analysis") {
    m_ui.analyzeButton->setText("abort analysis");
    m_ui.trackComboBox->setEnabled(false);
    setupAnalysis();
    if (m_ui.trackComboBox->currentText() == "centered broken line") {
      m_ui.plotter->startAnalysis(Track::CenteredBrokenLine, m_ui.numberOfThreadsSpinBox->value());
    } else if (m_ui.trackComboBox->currentText() == "broken line") {
      m_ui.plotter->startAnalysis(Track::BrokenLine , m_ui.numberOfThreadsSpinBox->value());
    } else if (m_ui.trackComboBox->currentText() == "straight line") {
      m_ui.plotter->startAnalysis(Track::StraightLine, m_ui.numberOfThreadsSpinBox->value());
    } else if (m_ui.trackComboBox->currentText() == "none") {
      m_ui.plotter->startAnalysis(Track::None, m_ui.numberOfThreadsSpinBox->value());
    }
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
      setFileList(file);
  } else if (sender() == m_ui.addFileListButton) {
    foreach(QString file, files)
      addFileList(file);
  }
}

void MainWindow::addFileList(const QString& fileName)
{
  m_ui.plotter->addFileList(fileName);
}

void MainWindow::setFileList(const QString& fileName)
{
  m_ui.plotter->setFileList(fileName);
}

void MainWindow::saveCanvasButtonClicked()
{
  QString fileEnding;
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "svg;;pdf;;root;;png", &fileEnding);
  if (fileName.isEmpty())
    return;
  fileEnding.prepend('.');
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  m_ui.plotter->saveCanvas(fileName);
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

void MainWindow::closeEvent(QCloseEvent* event)
{
  m_ui.plotter->abortAnalysis();
  event->accept();
}
