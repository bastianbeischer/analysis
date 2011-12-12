#ifndef ReconstructionMethodCorrelationPlotCollection_hh
#define ReconstructionMethodCorrelationPlotCollection_hh

#include "PlotCollection.hh"

#include <QComboBox>

class ReconstructionMethodCorrelationPlotCollection : public PlotCollection {
public:
  ReconstructionMethodCorrelationPlotCollection();
  ~ReconstructionMethodCorrelationPlotCollection();
  virtual void update();
private:
  int methodsToIndex(Enums::ReconstructionMethod, Enums::ReconstructionMethod);
  QComboBox* m_reconstructionMethodXComboBox;
  QComboBox* m_reconstructionMethodYComboBox;
};

#endif
