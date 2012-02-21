#ifndef MCInformation_hh
#define MCInformation_hh

#include "Reconstruction.hh"
#include "Hypothesis.hh"

#include <QVector>
#include <QPointF>

class AnalyzedEvent;

class MCInformation : public Reconstruction {
public:
  MCInformation(Enums::LikelihoodVariables, Enums::Particles);
  ~MCInformation();
  virtual void identify(AnalyzedEvent*);
};

#endif
