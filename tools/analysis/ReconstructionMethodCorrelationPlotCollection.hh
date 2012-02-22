#ifndef ReconstructionMethodCorrelationPlotCollection_hh
#define ReconstructionMethodCorrelationPlotCollection_hh

#include "PlotCollection.hh"
#include "Enums.hh"

#include <QComboBox>

class ReconstructionMethodCorrelationPlotCollection : public PlotCollection {
Q_OBJECT
public:
  ReconstructionMethodCorrelationPlotCollection(Enums::ReconstructionMethods, Enums::Particles);
  ~ReconstructionMethodCorrelationPlotCollection();
public slots:
  virtual void update();
private:
  Enums::Particles m_particles;
  QComboBox* m_particleComboBox;
  QComboBox* m_reconstructionMethodXComboBox;
  QComboBox* m_reconstructionMethodYComboBox;
};

#endif
