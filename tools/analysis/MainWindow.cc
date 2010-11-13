#include "MainWindow.hh"

#include "Plotter.hh"
#include "Setup.hh"
#include "Layer.hh"
#include "AnalysisPlot.hh"
#include "CenteredBrokenLine.hh"
#include "BrokenLine.hh"
#include "StraightLine.hh"
#include "BendingPositionPlot.hh"

#include <QDebug>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTimer>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
  : QDialog(parent)
  , m_updateTimer(0)
  , m_plotter(0)
  , m_positionLabel(0)
  , m_tabBar(0)
  , m_activePlots()
{
  m_ui.setupUi(this);
  
  m_positionLabel = new QLabel(m_ui.rightWidget);
  
  m_plotter = new Plotter(m_ui.rightWidget);
  m_plotter->setPositionLabel(m_positionLabel);
  m_plotter->setProgressBar(m_ui.progressBar);
  
  m_tabBar = new QTabBar(m_ui.rightWidget);
  m_tabBar->setExpanding(false);
  m_tabBar->setTabsClosable(true);
  connect(m_tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(tabBarCloseRequested(int)));
  connect(m_tabBar, SIGNAL(currentChanged(int)), this, SLOT(tabBarCurrentChanged(int)));
  
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_tabBar);
  layout->addWidget(m_plotter);
  layout->addWidget(m_positionLabel);
  m_ui.rightWidget->setLayout(layout);

  m_updateTimer = new QTimer(this);
  connect(m_updateTimer, SIGNAL(timeout()), m_plotter, SLOT(update()));

  connect(m_ui.analyzeButton, SIGNAL(clicked()), this, SLOT(analyzeButtonClicked()));
  connect(m_ui.setFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.addFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));

  connect(m_ui.signalHeightUpperTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.signalHeightLowerTrackerButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
  connect(m_ui.signalHeightTRDButton, SIGNAL(clicked()), this, SLOT(showButtonsClicked()));
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
  if (b->text() == "show") {
    b->setText("hide");
    QVector<unsigned int> plotIndices = m_plotter->plotIndices(topic);
    for (int i = 0; i < plotIndices.size(); ++i) {
      m_activePlots.append(plotIndices[i]);
      m_tabBar->addTab(m_plotter->plotTitle(plotIndices[i]));
    }
  } else {
    b->setText("show");
    for (int i = m_tabBar->count() - 1; i >= 0; --i) {
      if (m_plotter->plotTopic(m_activePlots[i]) == topic)
        tabBarCloseRequested(i);
    }
  }
}

void MainWindow::tabBarCloseRequested(int i)
{
  m_activePlots.remove(i);
  m_tabBar->removeTab(i);
}

void MainWindow::tabBarCurrentChanged(int i)
{
  m_plotter->selectPlot(m_activePlots.size() ? m_activePlots[i] : -1);
}

void MainWindow::setupAnalysis()
{
  m_plotter->clearPlots();
  m_activePlots.clear();
  while (m_tabBar->count())
    m_tabBar->removeTab(0);
  if (m_ui.signalHeightUpperTrackerCheckBox->isChecked()) {
  }
  if (m_ui.signalHeightLowerTrackerCheckBox->isChecked()) {
  }
  if (m_ui.signalHeightTRDCheckBox->isChecked()) {
  }
  if (m_ui.timeOverThresholdCheckBox->isChecked()) {
  }
  if (m_ui.trackingCheckBox->isChecked()) {
    m_plotter->addPlot(new BendingPositionPlot);
    m_plotter->addPlot(new BendingPositionPlot);
    m_plotter->addPlot(new BendingPositionPlot);
  }
  if (m_ui.occupancyCheckBox->isChecked()) {
  }
  if (m_ui.residualsUpperTrackerCheckBox->isChecked()) {
  }
  if (m_ui.residualsLowerTrackerCheckBox->isChecked()) {
  }
  if (m_ui.residualsTRDCheckBox->isChecked()) {
  }
  if (m_ui.momentumReconstructionCheckBox->isChecked()) {
  }
  if (m_ui.miscellaneousTrackerCheckBox->isChecked()) {
  }
  if (m_ui.miscellaneousTRDCheckBox->isChecked()) {
  }
  if (m_ui.miscellaneousTOFCheckBox->isChecked()) {
  }
  
  m_ui.signalHeightUpperTrackerButton->setText("show");
  m_ui.signalHeightLowerTrackerButton->setText("show");
  m_ui.signalHeightTRDButton->setText("show");
  m_ui.timeOverThresholdButton->setText("show");
  m_ui.trackingButton->setText("show");
  m_ui.occupancyButton->setText("show");
  m_ui.residualsUpperTrackerButton->setText("show");
  m_ui.residualsLowerTrackerButton->setText("show");
  m_ui.residualsTRDButton->setText("show");
  m_ui.momentumReconstructionButton->setText("show");
  m_ui.miscellaneousTrackerButton->setText("show");
  m_ui.miscellaneousTRDButton->setText("show");
  m_ui.miscellaneousTOFButton->setText("show");

  m_ui.signalHeightUpperTrackerButton->setEnabled(m_ui.signalHeightUpperTrackerCheckBox->isChecked());
  m_ui.signalHeightLowerTrackerButton->setEnabled(m_ui.signalHeightLowerTrackerCheckBox->isChecked());
  m_ui.signalHeightTRDButton->setEnabled(m_ui.signalHeightTRDCheckBox->isChecked());
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

  if (m_ui.trackComboBox->currentText() == "centered broken line") {
    m_plotter->setTrackType(new CenteredBrokenLine());
  } else if (m_ui.trackComboBox->currentText() == "broken line") {
    m_plotter->setTrackType(new BrokenLine());
  } else if (m_ui.trackComboBox->currentText() == "straight line") {
    m_plotter->setTrackType(new StraightLine());
  } else if (m_ui.trackComboBox->currentText() == "none") {
    m_plotter->setTrackType(0);
  }

  Setup* setup = Setup::instance();
  Layer* layer = setup->firstLayer();
  while (layer) {
    //double z = layer->z();


      layer = setup->nextLayer();

  //plotter->addPlot(new ResidualPlot());
  }
  m_updateTimer->start(100);
}

void MainWindow::analyzeButtonClicked()
{
  if (m_ui.analyzeButton->text() == "start analysis") {
    m_ui.analyzeButton->setText("abort analysis");
    m_ui.trackComboBox->setEnabled(false);
    setupAnalysis();
    m_plotter->startAnalysis();
  } else {
    m_plotter->abortAnalysis();
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
  m_plotter->addFileList(fileName);
}

void MainWindow::setFileList(const QString& fileName)
{
  m_plotter->setFileList(fileName);
}

void MainWindow::saveButtonClicked()
{
  QString fileEnding;
  QString fileName = QFileDialog::getSaveFileName(this, "save event", "", "svg;;pdf;;root;;png", &fileEnding);
  if (fileName.isEmpty())
    return;
  fileEnding.prepend('.');
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  m_plotter->saveCanvas(fileName);
}
