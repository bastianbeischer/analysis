#ifndef RigiditySpectrumRatio_hh
#define RigiditySpectrumRatio_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QObject>

class Hit;
class TH1D;
class Particle;
class SimpleEvent;

class RigiditySpectrumRatio : public QObject, public AnalysisPlot, public H1DPlot {
Q_OBJECT
public:
  RigiditySpectrumRatio();
  ~RigiditySpectrumRatio();
  virtual void processEvent(const AnalyzedEvent*);
  virtual void update();
private slots:
  void selectionChanged();
private:
  TH1D* m_numerator;
  TH1D* m_denominator;
};

#endif
