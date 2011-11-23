#ifndef LocationSelectonWidget_hh
#define LocationSelectonWidget_hh

#include "Enums.hh"

#include <QWidget>

class QButtonGroup;
class QAbstractButton;

class LocationSelectonWidget : public QWidget {
Q_OBJECT
public:
  LocationSelectonWidget(QWidget* = 0);
  ~LocationSelectonWidget();
  Enums::Situation situation();
signals:
  void selectionChanged(Enums::Situation);
private slots:
  void selectionChanged(QAbstractButton*);
private:
  QButtonGroup* m_buttonGroup;
};

#endif
