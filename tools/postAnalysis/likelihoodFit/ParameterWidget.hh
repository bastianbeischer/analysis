#ifndef ParameterWidget_h
#define ParameterWidget_h

#include <QWidget>
#include <QSize>

class QDoubleSpinBox;

class ParameterWidget : public QWidget {
Q_OBJECT
public:
  ParameterWidget(QWidget* parent = 0);
  ~ParameterWidget();
  virtual QSize minimumSizeHint() const;
signals:
  void changed();
private slots:
  void update();
private:
  QDoubleSpinBox* m_valueSpinBox;
  QDoubleSpinBox* m_minSpinBox;
  QDoubleSpinBox* m_maxSpinBox;
};

#endif
