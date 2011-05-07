#ifndef TOFChannelSelectionWidget_hh
#define TOFChannelSelectionWidget_hh

#include <QWidget>

class TOFChannelSelectionWidget :
  public QWidget
{
  
  Q_OBJECT

public:
  TOFChannelSelectionWidget(const QVector<unsigned short>&, QWidget* = 0);
  ~TOFChannelSelectionWidget();
  
signals:
  void channelChanged(int);

};

#endif /* TOFChannelSelectionWidget_hh */
