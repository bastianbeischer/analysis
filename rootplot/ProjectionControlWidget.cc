#include "ProjectionControlWidget.hh"

#include "H2DProjectionPlot.hh"
#include "RootQtWidget.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>

ProjectionControlWidget::ProjectionControlWidget(H2DProjectionPlot* plot, QWidget* parent) :
  QWidget(parent),
  m_projectionPlot(plot),
  m_comboBox(new QComboBox),
  m_spinBox(new QSpinBox),
  m_checkBoxLogX(new QCheckBox),
  m_checkBoxLogY(new QCheckBox),
  m_checkBoxLogZ(new QCheckBox)
{
  m_comboBox->addItem("none");
  m_comboBox->addItem("projection on y");
  m_comboBox->addItem("projection on x");
  m_comboBox->setCurrentIndex(0);

  m_spinBox->setMinimum(1);
  m_spinBox->setValue(1);
  m_spinBox->setEnabled(false);
  
  m_checkBoxLogX->setText("log x");
  m_checkBoxLogY->setText("log y");
  m_checkBoxLogZ->setText("log z");
  m_checkBoxLogX->setEnabled(false);
  m_checkBoxLogY->setEnabled(false);
  m_checkBoxLogZ->setEnabled(false);

  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->addWidget(m_comboBox);
  layout->addSpacing(50);
  layout->addWidget(new QLabel("number of bins"));
  layout->addWidget(m_spinBox);
  layout->addWidget(m_checkBoxLogX);
  layout->addWidget(m_checkBoxLogY);
  layout->addWidget(m_checkBoxLogZ);
  layout->addStretch();

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeProjectionType(int)));
}

ProjectionControlWidget::~ProjectionControlWidget()
{
}

void ProjectionControlWidget::setProjectionType(H2DProjectionPlot::ProjectionType type)
{
  switch (type) {
    case H2DProjectionPlot::NoProjection: m_comboBox->setCurrentIndex(0); break;
    case H2DProjectionPlot::ProjectionOnY: m_comboBox->setCurrentIndex(1); break;
    case H2DProjectionPlot::ProjectionOnX: m_comboBox->setCurrentIndex(2); break;
  }
}

void ProjectionControlWidget::changeProjectionType(int index)
{
  switch (index) {
  case 0:
    m_projectionPlot->setProjectionType(H2DProjectionPlot::NoProjection);
    m_spinBox->setEnabled(false);
    break;
  case 1:
    m_projectionPlot->setProjectionType(H2DProjectionPlot::ProjectionOnY);
    m_spinBox->setEnabled(true);
    break;
  case 2:
    m_projectionPlot->setProjectionType(H2DProjectionPlot::ProjectionOnX);
    m_spinBox->setEnabled(true);
    break;
  default:
    Q_ASSERT(false);
    break;
  }
}
  
QSpinBox* ProjectionControlWidget::spinBox()
{
  return m_spinBox;
}

void ProjectionControlWidget::connectCheckBoxes()
{
  connect(m_checkBoxLogX, SIGNAL(stateChanged(int)), m_projectionPlot, SLOT(setLogX(int)));
  connect(m_checkBoxLogY, SIGNAL(stateChanged(int)), m_projectionPlot, SLOT(setLogY(int)));
  connect(m_checkBoxLogZ, SIGNAL(stateChanged(int)), m_projectionPlot, SLOT(setLogZ(int)));
}

void ProjectionControlWidget::activateLogCheckBoxes()
{
  m_checkBoxLogX->setEnabled(true);
  m_checkBoxLogY->setEnabled(true);
  m_checkBoxLogZ->setEnabled(true);
}
