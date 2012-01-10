#ifndef ParameterWidget_h
#define ParameterWidget_h

#include <QWidget>
#include <QSize>

class DoubleEdit;
class QScrollBar;

class ParameterWidget : public QWidget {
Q_OBJECT
public:
  ParameterWidget(QWidget* parent = 0);
  ~ParameterWidget();
  void setValue(double);
  double value() const;
  void setMinimum(double);
  double minimum() const;
  void setMaximum(double);
  double maximum() const;
  void setRange(double min, double max);
signals:
  void valueChanged(double);
private slots:
  void valueEditChanged(double);
  void scollBarChanged(int);
  void minEditChanged(double);
  void maxEditChanged(double);
private:
  int exponent(double);
  void updateStepWidth();

  DoubleEdit* m_valueEdit;
  QScrollBar* m_scrollBar;
  DoubleEdit* m_minEdit;
  DoubleEdit* m_maxEdit;
  int m_nTicks;
};

#endif
