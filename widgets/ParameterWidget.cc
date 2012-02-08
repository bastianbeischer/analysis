#include "ParameterWidget.hh"
#include "DoubleEdit.hh"

#include <QScrollBar>
#include <QGridLayout>
#include <QDebug>

#include <cfloat>
#include <cmath>

ParameterWidget::ParameterWidget(QWidget* parent)
  : QWidget(parent)
  , m_valueEdit(new DoubleEdit)
  , m_scrollBar(new QScrollBar(Qt::Horizontal))
  , m_minEdit(new DoubleEdit)
  , m_maxEdit(new DoubleEdit)
  , m_nTicks(10000)
{
  m_scrollBar->setRange(0, m_nTicks);
  m_scrollBar->setInvertedControls(false);
  m_scrollBar->setSingleStep(1);
  m_scrollBar->setPageStep(10);

  m_minEdit->setValue(-100);
  m_minEdit->setRange(-DBL_MAX, 100);
  m_maxEdit->setValue(100);
  m_maxEdit->setRange(-100, DBL_MAX);
  updateStepWidth();

  m_valueEdit->setValue(0);
  m_valueEdit->setRange(m_minEdit->value(), m_maxEdit->value());

  valueEditChanged(m_valueEdit->value());

  connect(m_valueEdit, SIGNAL(valueChanged(double)), this, SLOT(valueEditChanged(double)));
  connect(m_minEdit, SIGNAL(valueChanged(double)), this, SLOT(minEditChanged(double)));
  connect(m_maxEdit, SIGNAL(valueChanged(double)), this, SLOT(maxEditChanged(double)));

  setFixedWidth(120);
  QGridLayout* layout = new QGridLayout();
  layout->setSpacing(1);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_valueEdit, 0, 0, 1, 2);
  layout->addWidget(m_scrollBar, 1, 0, 1, 2);
  layout->addWidget(m_minEdit, 2, 0, 1, 1);
  layout->addWidget(m_maxEdit, 2, 1, 1, 1);
  setLayout(layout);
  adjustSize();
}

ParameterWidget::~ParameterWidget()
{
}

void ParameterWidget::setValue(double value)
{
  m_valueEdit->setValue(value);
  emit valueChanged(value);
}

double ParameterWidget::value() const
{
  return m_valueEdit->value();
}

void ParameterWidget::setMinimum(double value)
{
  m_minEdit->setValue(value);
}

double ParameterWidget::minimum() const
{
  return m_minEdit->value();
}

void ParameterWidget::setMaximum(double value)
{
  m_maxEdit->setValue(value);
}

double ParameterWidget::maximum() const
{
  return m_maxEdit->value();
}

void ParameterWidget::setRange(double min, double max)
{
  setMinimum(min);
  setMaximum(max);
}

void ParameterWidget::valueEditChanged(double value)
{
  double min = m_minEdit->value();
  double max = m_maxEdit->value();
  int position = m_nTicks * (value - min) / (max - min);
  m_scrollBar->disconnect();
  m_scrollBar->setSliderPosition(position);
  connect(m_scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scollBarChanged(int)));
  emit valueChanged(value);
}

void ParameterWidget::scollBarChanged(int position)
{
  double min = m_minEdit->value();
  double max = m_maxEdit->value();
  double value = min + position * (max - min) / m_nTicks;
  m_valueEdit->disconnect();
  m_valueEdit->setValue(value);
  connect(m_valueEdit, SIGNAL(valueChanged(double)), this, SLOT(valueEditChanged(double)));
  emit valueChanged(value);
}

void ParameterWidget::minEditChanged(double current)
{
  m_maxEdit->setRange(current, DBL_MAX);
  m_valueEdit->disconnect();
  m_valueEdit->setRange(current, m_maxEdit->value());
  valueEditChanged(m_valueEdit->value());
  connect(m_valueEdit, SIGNAL(valueChanged(double)), this, SLOT(valueEditChanged(double)));
  updateStepWidth();
}

void ParameterWidget::maxEditChanged(double current)
{
  m_minEdit->setRange(-DBL_MAX, current);
  m_valueEdit->disconnect();
  m_valueEdit->setRange(m_minEdit->value(), current);
  valueEditChanged(m_valueEdit->value());
  connect(m_valueEdit, SIGNAL(valueChanged(double)), this, SLOT(valueEditChanged(double)));
  updateStepWidth();
}

int ParameterWidget::exponent(double value)
{
  return floor(log(value) / log(10));
}

void ParameterWidget::updateStepWidth()
{
  m_valueEdit->setSingleStep(.01);
  m_minEdit->setSingleStep(.01);
  m_maxEdit->setSingleStep(.01);

  m_valueEdit->setPageStep(.1);
  m_minEdit->setPageStep(.1);
  m_maxEdit->setPageStep(.1);

  double range = m_maxEdit->value() - m_minEdit->value();

  if (qIsNull(range))
    return;

  int exp = exponent(range);
  double step = pow(10, exp - 1);
  double pageStep = 10 * step;

  if (step < 0.01)
    return;

  m_valueEdit->setSingleStep(0.1 * step);
  m_valueEdit->setPageStep(0.1 * pageStep);

  if (exponent(range - step) < exp) {
    double value = qRound(1e6 * (range - pow(10, exp))) * 1e-6;
    value = qIsNull(value) ? .1*step : value;
    m_minEdit->setIncreasingSingleStep(value);
    m_maxEdit->setDecreasingSingleStep(value);
  } else {
    m_minEdit->setIncreasingSingleStep(step);
    m_maxEdit->setDecreasingSingleStep(step);
  }

  if (exponent(range + step) > exp) {
    double value = qRound(1e6 * (pow(10, exp + 1) - range)) * 1e-6;
    value = qIsNull(value) ? 10*step : value;
    m_minEdit->setDecreasingSingleStep(value);
    m_maxEdit->setIncreasingSingleStep(value);
  } else {
    m_minEdit->setDecreasingSingleStep(step);
    m_maxEdit->setIncreasingSingleStep(step);
  }

  if (exponent(range - pageStep) < exp) {
    double value = qRound(1e6 * (range - pow(10, exp))) * 1e-6;
    value = qIsNull(value) ? .1*pageStep : value;
    m_minEdit->setIncreasingPageStep(value);
    m_maxEdit->setDecreasingPageStep(value);
  } else {
    m_minEdit->setIncreasingPageStep(pageStep);
    m_maxEdit->setDecreasingPageStep(pageStep);
  }

  if (exponent(range + pageStep) > exp) {
    double value = qRound(1e6 * (pow(10, exp + 1) - range)) * 1e-6;
    value = qIsNull(value) ? 10*pageStep : value;
    m_minEdit->setDecreasingPageStep(value);
    m_maxEdit->setIncreasingPageStep(value);
  } else {
    m_minEdit->setDecreasingPageStep(pageStep);
    m_maxEdit->setIncreasingPageStep(pageStep);
  }
}
