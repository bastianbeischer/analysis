#ifndef LikelihoodPDF_hh
#define LikelihoodPDF_hh

#include "Enums.hh"
#include "KineticVariable.hh"

#include <TF1.h>

class Likelihood;

class LikelihoodPDF : public TF1 {
public:
  LikelihoodPDF(const Likelihood*, const KineticVariable&);
  ~LikelihoodPDF();
  const KineticVariable& variable() const;
  double integral();
private:
  double rootFunctionPointer(double*, double*) const;
  KineticVariable m_variable;
  const Likelihood* m_likelihood;
};

#endif