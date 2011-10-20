#ifndef TRDELECTRONIDENTIFIERLIKELIHOOD_HH
#define TRDELECTRONIDENTIFIERLIKELIHOOD_HH

#include "TRDElectronIdentifier.hh"

class TF1;
class AnalyzedEvent;

class TRDElectronIdentifierLikelihood : public TRDElectronIdentifier
{
public:
  TRDElectronIdentifierLikelihood();
  bool isElectronish(const AnalyzedEvent*, bool &ok);
  bool isElectronish(const AnalyzedEvent*, bool &ok, double &logLH);
private:
  const TF1* m_LHFunctionTR;
  QVector<const TF1*> m_LHFunctionNonTR;
  double m_logLHCut;
};

#endif // TRDELECTRONIDENTIFIERLIKELIHOOD_HH
