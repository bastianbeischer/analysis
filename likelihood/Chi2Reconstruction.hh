#ifndef Chi2Reconstruction_hh
#define Chi2Reconstruction_hh

#include "Reconstruction.hh"
#include "Hypothesis.hh"

#include <QVector>
#include <QPointF>

class AnalyzedEvent;
class Likelihood;
class TMultiGraph;
class Chi2Function;
namespace ROOT {namespace Math {class BrentMinimizer1D;}}

class Chi2Reconstruction : public Reconstruction {
public:
  Chi2Reconstruction(Enums::LikelihoodVariables, Enums::Particles, bool additionalInformation = false);
  ~Chi2Reconstruction();
  virtual void identify(AnalyzedEvent*);
  virtual TMultiGraph* graph() const;
private:
  double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  double rootFunctorPointer(const double* momentum);
  ROOT::Math::BrentMinimizer1D* m_chi2Minimizer;
  Chi2Function* m_chi2Function;
};

#endif
