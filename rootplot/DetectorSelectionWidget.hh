#ifndef DetectorSelectionWidget_hh
#define DetectorSelectionWidget_hh

#include <QWidget>

class StringSpinBox;
class QSpinBox;
class QCheckBox;

class DetectorSelectionWidget :
  public QWidget
{
  
  Q_OBJECT

public:
  DetectorSelectionWidget(const QStringList&, const int&);
  ~DetectorSelectionWidget();
  
signals:
  void selectPlot(int);

private slots:
  void selectedPlotChanged();
  void setChannelSpinBoxState(int);

private:
  StringSpinBox* m_moduleSpinBox;
  QSpinBox* m_channelSpinBox;
  QCheckBox* m_individualChannelCheckBox;
  
};

#endif /* DetectorSelectionWidget_hh */
