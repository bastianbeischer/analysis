#ifndef ParameterPlot_hh
#define ParameterPlot_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"
#include "Enums.hh"

#include <QVector>

class TLine;
class Likelihood;

class ParameterPlot : public PostAnalysisPlot, public GraphPlot {
public:
  ParameterPlot(Likelihood* likelihood, Enums::Particle);
  ~ParameterPlot();
  void setup();
  void addIntegral();
  virtual void draw(TCanvas*);
private:
  Likelihood* m_likelihood;
  Enums::Particle m_particle;
  int m_numberOfPoints;
  double m_min;
  double m_max;
  double m_step;
  QVector<TLine*> m_lines;
};

#endif
