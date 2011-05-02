#include "DetectorIDSpinBox.hh"

DetectorIDSpinBox::DetectorIDSpinBox(const QVector<unsigned short>& detIds, QWidget* parent) :
  QSpinBox(parent),
  m_detIds(detIds)
{
  setMinimum(0);
  setMaximum(m_detIds.size()-1);
  setValue(0);
}

DetectorIDSpinBox::~DetectorIDSpinBox()
{
}

QString DetectorIDSpinBox::textFromValue(int value) const
{
  return QString("0x%1").arg(m_detIds.at(value), 0, 16);
}

int DetectorIDSpinBox::valueFromText(const QString& string) const
{
  return m_detIds.indexOf(string.toUShort());
}
