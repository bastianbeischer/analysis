#ifndef LikelihoodPDF_hh
#define LikelihoodPDF_hh

#include "Enums.hh"
#include "KineticVariable.hh"
#include "PDFParameters.hh"

#include <TF1.h>

class Likelihood;

class LikelihoodPDF : public TF1 {
public:
  LikelihoodPDF(const Likelihood*, const KineticVariable&);
  ~LikelihoodPDF();
  void setParameters(const PDFParameters&);
  const KineticVariable& variable() const;
  double integral();
  void setAxisTitle(const QString&);
private:
  double rootFunctionPointer(double*, double*) const;
  PDFParameters m_parameters;
  KineticVariable m_variable;
  const Likelihood* m_likelihood;
};

#endif
