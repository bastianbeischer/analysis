#ifndef StringSpinBox_hh
#define StringSpinBox_hh

#include <QSpinBox>
#include <QStringList>

class StringSpinBox :
  public QSpinBox
{

public:
  StringSpinBox(const QStringList&, QWidget* = 0);
  ~StringSpinBox();

  QString textFromValue(int) const;
  int valueFromText(const QString&) const;

private:
  QStringList m_strings;

};

#endif /* StringSpinBox_hh */
