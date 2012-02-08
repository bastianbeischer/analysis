#include "DoubleEdit.hh"

#include <QLineEdit>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDebug>

#include <cfloat>

DoubleEdit::DoubleEdit(QWidget* parent)
  : QWidget(parent)
  , m_value(0)
  , m_minimum(-DBL_MAX)
  , m_maximum(DBL_MAX)
  , m_increasingPageStep(10)
  , m_decreasingPageStep(10)
  , m_increasingSingleStep(1)
  , m_decreasingSingleStep(1)
  , m_edit(new QLineEdit("0"))
{
  setMinimumSize(50, 20);
  m_edit->setMinimumSize(50, 20);
  m_edit->setValidator(new QDoubleValidator(this));
  m_edit->setAlignment(Qt::AlignCenter);
  connect(m_edit, SIGNAL(textEdited(const QString&)), this, SLOT(textEdited(const QString&)));
  QHBoxLayout* layout = new QHBoxLayout;
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_edit);
  setLayout(layout);
}

DoubleEdit::~DoubleEdit()
{
}

double DoubleEdit::value() const
{
  return m_value;
}

void DoubleEdit::setValue(double value)
{
  bool empty = m_edit->text().isEmpty();
  if (qFuzzyCompare(m_value, value) && !empty)
    return;
  m_value = qBound(m_minimum, value, m_maximum);
  if (empty || !qFuzzyCompare(m_value, m_edit->text().toDouble()))
    m_edit->setText(QString::number(m_value));
  emit(valueChanged(m_value));
  colorize();
}

void DoubleEdit::textEdited(const QString& text)
{
  setValue(text.isEmpty() ? 0 : text.toDouble());
}

void DoubleEdit::colorize()
{
  QPalette palette = m_edit->palette();
  if (qFuzzyCompare(m_value, m_minimum) || qFuzzyCompare(m_value, m_maximum)) {
    palette.setBrush(QPalette::Base, Qt::red);
  } else {
    palette.setBrush(QPalette::Base, Qt::white);
  }
  m_edit->setPalette(palette);
}

void DoubleEdit::setRange(double min, double max)
{
  Q_ASSERT(min <= max);
  m_minimum = min;
  m_maximum = max;
  if (m_value < min)
    setValue(min);
  if (m_value > max)
    setValue(max);
  colorize();
}

void DoubleEdit::keyPressEvent(QKeyEvent* event)
{
  int key = event->key();
  if (key == Qt::Key_Up) {
    setValue(m_value + m_increasingSingleStep);
  } else if (key == Qt::Key_Down) {
    setValue(m_value - m_decreasingSingleStep);
  } else if (key == Qt::Key_PageUp) {
    setValue(m_value + m_increasingPageStep);
  } else if (key == Qt::Key_PageDown) {
    setValue(m_value - m_decreasingPageStep);
  } else {
    event->ignore();
    QWidget::keyPressEvent(event);
  }
}

void DoubleEdit::wheelEvent(QWheelEvent* event)
{
  if (event->orientation() == Qt::Vertical) {
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    double step = numSteps < 0 ? m_decreasingSingleStep : m_increasingSingleStep;
    setValue(m_value + numSteps * step);
    event->accept();
  } else {
    event->ignore();
    QWidget::wheelEvent(event);
  }
}

void DoubleEdit::setPageStep(double step)
{
  m_increasingPageStep = step;
  m_decreasingPageStep = step;
}

void DoubleEdit::setIncreasingPageStep(double step)
{
  m_increasingPageStep = step;
}

void DoubleEdit::setDecreasingPageStep(double step)
{
  m_decreasingPageStep = step;
}

void DoubleEdit::setSingleStep(double step)
{
  m_increasingSingleStep = step;
  m_decreasingSingleStep = step;
}

void DoubleEdit::setIncreasingSingleStep(double step)
{
  m_increasingSingleStep = step;
}

void DoubleEdit::setDecreasingSingleStep(double step)
{
  m_decreasingSingleStep = step;
}
