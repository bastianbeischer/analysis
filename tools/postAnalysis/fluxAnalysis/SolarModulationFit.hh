#ifndef SolarModulationFit_hh
#define SolarModulationFit_hh

#include <QString>

class TF1;
class TH1D;

class SolarModulationFit {
public:
  SolarModulationFit(TH1D* fluxHistogram, int pdgId, double rangeMin = 1., double rangeMax = 5.);
  ~SolarModulationFit();
  void setGamma(double gamma);
  void setGammaC(double gammaC);
  void setRc(double Rc);
  TF1* fit();
  QString J0Label() const;
  QString gammaLabel() const;
  QString phiLabel() const;
  QString gammaClabel() const;
  QString rClabel() const;
  double J0() const;
  double J0Error() const;
  double gamma() const;
  double gammaError() const;
  double phi() const;
  double phiError() const;
  double gammaC() const;
  double gammaCerror() const;
  double rC() const;
  double rCerror() const;
private:
  SolarModulationFit();
  double powerLaw(double* x, double* p);
  TH1D* m_fluxHistogram;
  TF1* m_phiFit;
  int m_pdgId;
};


#endif
