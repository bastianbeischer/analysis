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
  void setScaleFactor(double);
  double scaleFactor() const;
  void setParameters(const PDFParameters&);
  void setParameter(int parameterNumber, double);
  void setNormalizationFactor(double);
  void setLimits(int parameterNumber, double min, double max);
  PDFParameters parameters() const;
  const KineticVariable& variable() const;
  double integral();
  void setAxisTitle(const QString&);
private:
  double rootFunctionPointer(double*, double*) const;
  double m_normalizationFactor;
  KineticVariable m_variable;
  const Likelihood* m_likelihood;
  int m_numberOfParameters;
};

#endif
