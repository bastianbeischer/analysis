#include "ParameterWidget.hh"

#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QDebug>

#include <cmath>

ParameterWidget::ParameterWidget(QWidget* parent)
  : QWidget(parent)
  , m_valueSpinBox(new QDoubleSpinBox)
  , m_minSpinBox(new QDoubleSpinBox)
  , m_maxSpinBox(new QDoubleSpinBox)
{
  QVBoxLayout* layout = new QVBoxLayout;
  
  m_valueSpinBox->setValue(0);
  connect(m_valueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
  layout->addWidget(m_valueSpinBox);

  m_minSpinBox->setPrefix("min: ");
  m_minSpinBox->setMinimum(-1e9);
  m_minSpinBox->setValue(-100);
  connect(m_minSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
  layout->addWidget(m_minSpinBox);

  m_maxSpinBox->setPrefix("max: ");
  m_maxSpinBox->setMaximum(+1e9);
  m_maxSpinBox->setValue(+100);
  connect(m_maxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(update()));
  layout->addWidget(m_maxSpinBox);
  
  update();

  setLayout(layout);
}

ParameterWidget::~ParameterWidget()
{
}

QSize ParameterWidget::minimumSizeHint() const
{
  return QSize(100, 100);
}

void ParameterWidget::update()
{
  double range = m_maxSpinBox->value() - m_minSpinBox->value();
  m_valueSpinBox->setRange(m_minSpinBox->value(), m_maxSpinBox->value());
  m_minSpinBox->setMaximum(m_maxSpinBox->value());
  m_maxSpinBox->setMinimum(m_minSpinBox->value());
  double step = qIsNull(range) ? 0.01 : 0.01 * pow(10, floor(log(range)/log(10)));
  m_valueSpinBox->setSingleStep(step);
  m_minSpinBox->setSingleStep(10*step);
  m_maxSpinBox->setSingleStep(10*step);
  emit(changed());
}
