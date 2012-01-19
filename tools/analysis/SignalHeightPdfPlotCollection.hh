#ifndef SignalHeightPdfPlotCollection_hh
#define SignalHeightPdfPlotCollection_hh

#include "PlotCollection.hh"
#include "Hit.hh"
#include "Enums.hh"

class QComboBox;
class SignalHeightPdfPlot;

class SignalHeightPdfPlotCollection : public PlotCollection {
Q_OBJECT
public:
  SignalHeightPdfPlotCollection(Hit::ModuleType, Enums::Particles);
  ~SignalHeightPdfPlotCollection();
public slots:
  void update();
  void particleChanged();
private:
  Hit::ModuleType m_type;
  QComboBox* m_particleComboBox;
  QComboBox* m_layerComboBox;
  QVector<SignalHeightPdfPlot*> m_signalHeightPdfPlots;
};

#endif /* SignalHeightPdfPlotCollection_hh */
