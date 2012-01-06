#ifndef DoubleEdit_h
#define DoubleEdit_h

#include <QWidget>
#include <QSize>

class QLineEdit;
class QKeyEvent;
class QWheelEvent;

class DoubleEdit : public QWidget {
Q_OBJECT
public:
  DoubleEdit(QWidget* parent = 0);
  ~DoubleEdit();
  double value() const;
  void setValue(double);
  void setRange(double, double);
  void setPageStep(double);
  void setIncreasingPageStep(double);
  void setDecreasingPageStep(double);
  void setSingleStep(double);
  void setIncreasingSingleStep(double);
  void setDecreasingSingleStep(double);
signals:
  void valueChanged(double current);
protected:
  virtual void keyPressEvent(QKeyEvent*);
  virtual void wheelEvent(QWheelEvent*);
private slots:
  void textEdited(const QString&);
  void colorize();
private:
  double m_value;
  double m_minimum;
  double m_maximum;
  double m_increasingPageStep;
  double m_decreasingPageStep;
  double m_increasingSingleStep;
  double m_decreasingSingleStep;
  QLineEdit* m_edit;
};

#endif
