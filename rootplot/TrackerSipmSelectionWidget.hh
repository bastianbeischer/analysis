#ifndef TrackerSipmSelectionWidget_hh
#define TrackerSipmSelectionWidget_hh

#include <QWidget>

class TrackerSipmSelectionWidget : public QWidget {
Q_OBJECT
public:
  TrackerSipmSelectionWidget(const QStringList&, QWidget* = 0);
  ~TrackerSipmSelectionWidget();
signals:
  void sipmChanged(int);
};

#endif /* TrackerSipmSelectionWidget_hh */
