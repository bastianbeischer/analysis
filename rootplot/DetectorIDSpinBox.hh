#ifndef DetectorIDSpinBox_hh
#define DetectorIDSpinBox_hh

#include <QSpinBox>

class DetectorIDSpinBox :
  public QSpinBox
{
  
public:
  DetectorIDSpinBox(const QVector<unsigned short>&, QWidget* = 0);
  ~DetectorIDSpinBox();
  
  QString textFromValue(int) const;
  int valueFromText(const QString&) const;

private:
  QVector<unsigned short> m_detIds;
  
};

#endif /* DetectorIDSpinBox_hh */
