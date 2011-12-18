#ifndef SignalHeightPdfPlotCollection_hh
#define SignalHeightPdfPlotCollection_hh

#include "PlotCollection.hh"
#include "Hit.hh"
#include "Enums.hh"

class QComboBox;

class SignalHeightPdfPlotCollection : public PlotCollection {
Q_OBJECT
public:
  SignalHeightPdfPlotCollection(Hit::ModuleType, Enums::Particles);
  ~SignalHeightPdfPlotCollection();
public slots:
  void update();
private:
  Enums::Particles m_particles;
  QComboBox* m_particleComboBox;
};

#endif /* SignalHeightPdfPlotCollection_hh */
