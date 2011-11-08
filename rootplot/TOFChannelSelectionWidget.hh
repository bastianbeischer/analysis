#ifndef TOFChannelSelectionWidget_hh
#define TOFChannelSelectionWidget_hh

#include <QWidget>

class TOFChannelSelectionWidget : public QWidget {
Q_OBJECT
public:
  TOFChannelSelectionWidget(const QStringList&, QWidget* = 0);
  ~TOFChannelSelectionWidget();
signals:
  void channelChanged(int);
};

#endif /* TOFChannelSelectionWidget_hh */
