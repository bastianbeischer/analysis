#include "StringSpinBox.hh"

StringSpinBox::StringSpinBox(const QStringList& strings, QWidget* parent) :
  QSpinBox(parent),
  m_strings(strings)
{
  setMinimum(0);
  setMaximum(m_strings.size()-1);
  setValue(0);
}

StringSpinBox::~StringSpinBox()
{
}

QString StringSpinBox::textFromValue(int value) const
{
  return m_strings.at(value);
}

int StringSpinBox::valueFromText(const QString& string) const
{
  return m_strings.indexOf(string);
}
