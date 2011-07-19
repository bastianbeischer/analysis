#ifndef SettingTimePlot_hh
#define SettingTimePlot_hh

#include "AnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QDateTime>

class SimpleEvent;
class TH1D;

class SettingTimePlot : public AnalysisPlot, public H1DPlot {
public:
  enum SettingType{MagnetInstalled, Momentum, Polarity, AbsMomentum};
  SettingTimePlot(SettingType, QDateTime first, QDateTime last);
  virtual ~SettingTimePlot();
  virtual void processEvent(const QVector<Hit*>&, const Particle* const, const SimpleEvent* const);
  virtual void finalize();
  virtual void draw(TCanvas* canvas);
protected:
  SettingType m_type;
  TH1D* m_normalizationHistogram;
};

#endif
