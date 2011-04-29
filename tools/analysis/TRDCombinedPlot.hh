#ifndef TRDCombinedPlot_hh
#define TRDCombinedPlot_hh

#include "PlotCollection.hh"

class DetectorIDSpinBox;
class QSpinBox;
class QCheckBox;

class TRDCombinedPlot :
  public PlotCollection
{
  
Q_OBJECT

public:
  TRDCombinedPlot();
  ~TRDCombinedPlot();
  
private slots:
  void selectedPlotChanged();
  void setChannelSpinBoxState(int);

private:
  DetectorIDSpinBox* m_moduleSpinBox;
  QSpinBox* m_channelSpinBox;
  QCheckBox* m_individualChannelCheckBox;

};

#endif /* TRDCombinedPlot_hh */
