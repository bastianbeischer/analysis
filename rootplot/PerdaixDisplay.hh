#ifndef PerdaixDisplay_hh
#define PerdaixDisplay_hh

#include "H2DPlot.hh"
#include <QVector>

class TCanvas;
class TBox;
class TGaxis;

class PerdaixDisplay :
  public H2DPlot
{
  
public:
  PerdaixDisplay();
  ~PerdaixDisplay();

  double yStretchFactor() const {return m_yStretchFactor;}

  virtual void draw(TCanvas* can);

private:
  void constructTof();
  void constructTrd();
  void constructTracker();
  void setupHistogram();
  void setupAxis();

protected:
  double m_yStretchFactor;
  
  TGaxis*         m_yAxis;
  QVector<TBox*>  m_boxes;
  
};

#endif /* PerdaixDisplay_hh */
