#ifndef RigidityUnfolding_hh
#define RigidityUnfolding_hh

#include <QString>

class TH2D;
class TH1D;
class TGraph;

class RigidityUnfolding {
public:
  RigidityUnfolding(TH2D* migrationHistogram, TH1D* unfoldInput);
  virtual ~RigidityUnfolding();
  TH2D* rohIj() {return m_rhoIj;}
  TH1D* unfoldedHistogram() {return m_unfoldedHistogram;}
  TGraph* lCurve() {return m_lCurve;}
  TGraph* bestlcurve() {return m_bestLcurve;}
private:
  void unfold();
  TH2D* m_migrationHistogram;
  TH1D* m_unfoldInput;
  TGraph* m_bestLcurve;
  TH1D* m_unfoldedHistogram;
  TH2D* m_rhoIj;
  TGraph* m_lCurve;
};

#endif
