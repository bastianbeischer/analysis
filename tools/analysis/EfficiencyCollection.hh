#ifndef EfficiencyCollection_hh
#define EfficiencyCollection_hh

#include "PlotCollection.hh"

class TH1D;

class EfficiencyCollection : public PlotCollection {
public:
  EfficiencyCollection(Enums::Particles);
  ~EfficiencyCollection();
};

#endif /* EfficiencyCollection_hh */
