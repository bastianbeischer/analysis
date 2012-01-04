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
  m_checkBoxLogY(new QCheckBox)
{
  m_comboBox->addItem("none");
  m_comboBox->addItem("projection on y");
  m_comboBox->addItem("projection on x");
  m_comboBox->setCurrentIndex(0);

  m_spinBox->setMinimum(1);
  m_spinBox->setValue(1);

  m_checkBoxLogX->setText("log x");
  m_checkBoxLogY->setText("log y");

  // deactivate all elements
  setElementStatus(false);

  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->addWidget(m_comboBox);
  layout->addSpacing(50);
  layout->addWidget(new QLabel("number of bins"));
  layout->addWidget(m_spinBox);
  layout->addWidget(m_checkBoxLogX);
  layout->addWidget(m_checkBoxLogY);
  layout->addStretch();

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeProjectionType(int)));
  connect(m_checkBoxLogX, SIGNAL(stateChanged(int)), m_projectionPlot, SLOT(setLogX(int)));
  connect(m_checkBoxLogY, SIGNAL(stateChanged(int)), m_projectionPlot, SLOT(setLogY(int)));
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
    setElementStatus(false);
    break;
  case 1:
    m_projectionPlot->setProjectionType(H2DProjectionPlot::ProjectionOnY);
    setElementStatus(true);
    break;
  case 2:
    m_projectionPlot->setProjectionType(H2DProjectionPlot::ProjectionOnX);
    setElementStatus(true);
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

QComboBox* ProjectionControlWidget::comboBox()
{
  return m_comboBox;
}

void ProjectionControlWidget::setElementStatus(bool status)
{
  m_spinBox->setEnabled(status);
  m_checkBoxLogX->setEnabled(status);
  m_checkBoxLogY->setEnabled(status);
}
