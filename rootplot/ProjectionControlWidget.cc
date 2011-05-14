#include "ProjectionControlWidget.hh"

#include "H2DProjectionPlot.hh"

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

ProjectionControlWidget::ProjectionControlWidget(H2DProjectionPlot* plot, QWidget* parent) :
  QWidget(parent),
  m_projectionPlot(plot),
  m_comboBox(new QComboBox),
  m_spinBox(new QSpinBox)
{
  m_comboBox->addItem("none");
  m_comboBox->addItem("projection on y");
  m_comboBox->addItem("projection on x");
  m_comboBox->setCurrentIndex(0);

  m_spinBox->setMinimum(1);
  m_spinBox->setValue(1);
  m_spinBox->setEnabled(false);

  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->addWidget(m_comboBox);
  layout->addSpacing(50);
  layout->addWidget(new QLabel("number of bins"));
  layout->addWidget(m_spinBox);
  layout->addStretch();

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeProjectionType(int)));
}

ProjectionControlWidget::~ProjectionControlWidget()
{
}

void ProjectionControlWidget::switchState(bool state)
{
  m_projectionPlot->setProjectionWidgetState(state);
  m_spinBox->setEnabled(state);
}

void ProjectionControlWidget::changeProjectionType(int index)
{
  switch (index) {
  case 0:
    switchState(false);
    break;
  case 1:
    m_projectionPlot->setProjectionType(H2DProjectionPlot::y);
    switchState(true);
    break;
  case 2:
    m_projectionPlot->setProjectionType(H2DProjectionPlot::x);
    switchState(true);
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
