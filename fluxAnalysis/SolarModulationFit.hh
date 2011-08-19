#ifndef SolarModulationFit_hh
#define SolarModulationFit_hh

#include <QString>

class TF1;
class TH1D;

class SolarModulationFit {
public:
  SolarModulationFit(TH1D* fluxHistogram, int pdgId = 2212);
	~SolarModulationFit();
  TF1* fit();
  QString gammaLabel() const;
  QString phiLabel() const;
  double gamma() const {return m_gamma;}
  double gammaError() const {return m_gammaError;}
  double phi() const {return m_phi;}
  double phiError() const {return m_phiError;}
private:
  SolarModulationFit();
  double powerLaw(double* x, double* p);
  TH1D* m_fluxHistogram;
  TF1* m_phiFit;
  int m_pdgId;
  double m_gamma;
  double m_gammaError;
  double m_phi;
  double m_phiError;
};


#endif
