#ifndef SignalHeightTrdLikelihood_hh
#define SignalHeightTrdLikelihood_hh

#include "Likelihood.hh"

class SignalHeightTrdLikelihood : public Likelihood {
public:
  SignalHeightTrdLikelihood(Enums::Particles = Enums::NoParticle);
  ~SignalHeightTrdLikelihood();
  int layer() const;
  void setLayer(int layer);
  virtual bool parametersExist(const Hypothesis&);
  virtual void removeParameters(const Hypothesis&);
  virtual void saveParameters(const Hypothesis&, const PDFParameters&);
  virtual void saveNormalization(Enums::Particle, const QVector<double>& rigidities, const QVector<double>& normalizationFactors);
  virtual double eval(const AnalyzedEvent*, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double trdSignal, const Hypothesis&, bool* goodInterpolation = 0) const;
  virtual double eval(double trdSignal, const Hypothesis&, const PDFParameters&) const;
protected:
  virtual void loadParameters();
private:
  double transitionRadiation(double signal, const PDFParameters&) const;
  double noTransitionRadiation(double signal, const PDFParameters&) const;
  int m_layer;
};

#endif
