#ifndef RigidityResolutionPlot_hh
#define RigidityResolutionPlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"
#include "KineticVariable.hh"
#include "Enums.hh"

#include <QMap>
#include <QPair>

class ParticleProperties;
class RootQtWidget;
class TLatex;

class RigidityResolutionPlot : public AnalysisPlot, public H1DPlot {
public:
  RigidityResolutionPlot(Enums::AnalysisTopic, Enums::Particle);
  ~RigidityResolutionPlot();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();
  virtual void finalize();
protected:
  virtual double referenceRigidity(const AnalyzedEvent*) const = 0;
  const ParticleProperties m_particleProperties;
private:
  void positionChanged(double, double);
  TF1* newTrackerResolutionFunction(const QString&) const;
  TF1* newTofResolutionFunction(const QString&) const;
  TF1* newWeightedMeanResolutionFunction(const QString&) const;
  double trackerGauss(double*, double*) const;
  double tofGauss(double*, double*) const;
  double trackerResolution(double*, double*) const;
  double tofResolution(double*, double*) const;
  double weightedMeanResolution(double*, double*) const;
  bool fitTracker(TH1D*, double curvature);
  bool fitTof(TH1D*, double inverseBeta);

  RootQtWidget* m_rigDistributionWidget;
  const int m_numberOfBins;
  const double m_min;
  const double m_max;
  TF1* m_trackerFunction;
  TF1* m_tofFunction;
  TF1* m_trackerResolutionGauss;
  TF1* m_tofResolutionGauss;
  QVector<TH1D*> m_trackerResolutionHistograms;
  QVector<TH1D*> m_tofResolutionHistograms;
  QVector<TH1D*> m_weightedMeanResolutionHistograms;
  QVector<TH1D*> m_chi2ResolutionHistograms;
  QVector<TH1D*> m_likelihoodResolutionHistograms;
  TLatex* m_projectionTrackerLatex;
  TLatex* m_projectionTofLatex;
  TLatex* m_projectionWeightedMeanLatex;
  TLatex* m_projectionChi2FitLatex;
  TLatex* m_projectionLikelihoodLatex;

  static const double s_probability;
};

#endif // RigidityResolutionPlot_hh
