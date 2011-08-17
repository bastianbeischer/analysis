#ifndef SolarModulationFit_hh
#define SolarModulationFit_hh

#include "SimpleEvent.hh"

#include <TH1D.h>
#include <TF1.h>

#include <QString>

class SolarModulationFit {

public:
  SolarModulationFit(TH1D* fluxHistogram);
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
  TH1D* m_fluxHistogram;
  TF1* m_phiFit;
  double m_gamma;
  double m_gammaError;
  double m_phi;
  double m_phiError;
};


#endif
