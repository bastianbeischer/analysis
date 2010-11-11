#include "MainWindow.hh"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
  : QDialog(parent)
{
  m_ui.setupUi(this);
  connect(m_ui.setFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.addFileListButton, SIGNAL(clicked()), this, SLOT(setOrAddFileListButtonClicked()));
  connect(m_ui.saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
  connect(m_ui.eventSpinBox, SIGNAL(valueChanged(int)), this, SLOT(update()));
  connect(m_ui.drawTrackCheckBox, SIGNAL(stateChanged(int)), this, SLOT(update()));
  m_ui.plotter->setPositionLabel(m_ui.positionLabel);
  connect(m_ui.unzoomButton, SIGNAL(clicked()), m_ui.plotter, SLOT(unzoom()));
}

MainWindow::~MainWindow()
{
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
  m_ui.plotter->drawEvent(m_ui.eventSpinBox->value(), m_ui.drawTrackCheckBox->isChecked());
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
