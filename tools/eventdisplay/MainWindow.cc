#include "MainWindow.hh"

#include <QDebug>
#include <QFileDialog>

#include "Track.hh"

MainWindow::MainWindow(QWidget* parent)
  : QDialog(parent)
{
  m_ui.setupUi(this);
  connect(m_ui.editEventListButton, SIGNAL(clicked()), this, SLOT(editEventListButtonClicked()));
  connect(m_ui.setFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.addFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
  connect(m_ui.eventSpinBox, SIGNAL(valueChanged(int)), this, SLOT(update()));
  connect(m_ui.eventListSpinBox, SIGNAL(valueChanged(int)), this, SLOT(eventSpinBoxValueChanged(int)));
  connect(m_ui.drawTrackCheckBox, SIGNAL(stateChanged(int)), this, SLOT(update()));
  connect(m_ui.allClustersCheckBox, SIGNAL(stateChanged(int)), this, SLOT(update()));
  connect(m_ui.slopeBinsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(update()));
  connect(m_ui.offsetBinsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(update()));
  connect(m_ui.trackerPullSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
  connect(m_ui.trdPullSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
  connect(m_ui.fitMethodComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(update()));
  m_ui.plotter->setPositionLabel(m_ui.positionLabel);
  connect(m_ui.unzoomButton, SIGNAL(clicked()), m_ui.plotter, SLOT(unzoom()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::editEventListButtonClicked()
{
  if (m_ui.editEventListButton->text() == "edit event list") {
    m_ui.textEdit->clear();
    foreach(int id, m_eventList)
      m_ui.textEdit->appendPlainText(QString::number(id));
    m_ui.textEdit->setReadOnly(false);
    m_ui.editEventListButton->setText("finish");
  } else {
    m_eventList.clear();
    QStringList list = m_ui.textEdit->toPlainText().split('\n');
    foreach(QString id, list)
      m_eventList.append(id.toInt());
    m_ui.eventListSpinBox->setMaximum(m_eventList.size()-1);
    m_ui.eventListSpinBox->setValue(0);
    m_ui.textEdit->setReadOnly(true);
    m_ui.editEventListButton->setText("edit event list");
    update();
  }
}

void MainWindow::eventSpinBoxValueChanged(int i)
{
  m_ui.eventSpinBox->setValue(m_eventList[i]);
}

void MainWindow::processArguments(QStringList arguments)
{
  arguments.removeFirst();
  foreach(QString argument, arguments) {
    if (argument.endsWith(".root"))
      addRootFile(argument);
    else
      addFileList(argument);
  }
}

void MainWindow::setFileList(const QString& fileList)
{
  m_ui.plotter->setFileList(fileList);
  updateEventSpinBox();
}


void MainWindow::addFileList(const QString& fileList)
{
  m_ui.plotter->addFileList(fileList);
  updateEventSpinBox();
}

void MainWindow::addRootFile(const QString& file)
{
  m_ui.plotter->addRootFile(file);
  updateEventSpinBox();
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

void MainWindow::updateEventSpinBox()
{
  m_ui.eventSpinBox->setMaximum(m_ui.plotter->numberOfEvents()-1);
  m_ui.eventSpinBox->setSuffix(QString("/%1").arg(m_ui.plotter->numberOfEvents()-1));
  update();
}

void MainWindow::update()
{
  int slopeBins = m_ui.slopeBinsSpinBox->value();
  int offsetBins = m_ui.offsetBinsSpinBox->value();
  double trackerPull = m_ui.trackerPullSpinBox->value();
  double trdPull = m_ui.trdPullSpinBox->value();
  m_ui.plotter->updateTrackFindingParameters(slopeBins, offsetBins, trackerPull, trdPull);
  
  bool drawTracks = m_ui.drawTrackCheckBox->isChecked();
  m_ui.fitMethodComboBox->setEnabled(drawTracks);
  m_ui.allClustersCheckBox->setEnabled(drawTracks);

  Enums::TrackType type = Enums::NoTrack;
  int index = m_ui.fitMethodComboBox->currentIndex();
  if (!drawTracks) type = Enums::NoTrack;
  else if (index == 0) type = Enums::CenteredBrokenLine;
  else if (index == 1) type = Enums::StraightLine;
  else if (index == 2) type = Enums::BrokenLine;
  m_ui.plotter->drawEvent(m_ui.eventSpinBox->value(), type, m_ui.allClustersCheckBox->isChecked(), *m_ui.textEdit, *m_ui.trackFindingWidget);
}

void MainWindow::saveButtonClicked()
{
  QString fileEnding;
  QString fileName = QFileDialog::getSaveFileName(this, "save event", "", "svg;;pdf;;png", &fileEnding);
  if (fileName.isEmpty())
    return;
  fileEnding.prepend('.');
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  m_ui.plotter->saveCanvas(fileName);
}
