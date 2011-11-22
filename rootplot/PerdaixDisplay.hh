#ifndef PerdaixDisplay_hh
#define PerdaixDisplay_hh

#include "H2DPlot.hh"
#include <QVector>

class TCanvas;
class TBox;
class TGaxis;

class PerdaixDisplay : public H2DPlot {
public:
  PerdaixDisplay();
  ~PerdaixDisplay();
  double yStretchFactor() const {return m_yStretchFactor;}
  virtual void draw(TCanvas*);
protected:
  double m_yStretchFactor;
  TGaxis* m_yAxis;
  QVector<TBox*> m_boxes;
private:
  void constructTof();
  void constructTrd();
  void constructTracker();
  void constructMagnet();
  void setupHistogram();
  void setupAxis();
};

#endif /* PerdaixDisplay_hh */
