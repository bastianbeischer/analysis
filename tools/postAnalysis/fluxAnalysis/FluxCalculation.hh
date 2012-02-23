#ifndef FluxCalculation_hh
#define FluxCalculation_hh

class TH1D;

class FluxCalculation {
public:
  FluxCalculation(TH1D* particleHistogram, double measurementTime);
  ~FluxCalculation();
  TH1D* fluxHistogram() const;
  void update(double measurementTime);
private:
  FluxCalculation();
  TH1D* m_particleHistogram;
  TH1D* m_fluxHistogram;
};


#endif
