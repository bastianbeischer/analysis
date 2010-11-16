#ifndef PerdaixDisplay_hh
#define PerdaixDisplay_hh

#include "H2DPlot.hh"
#include <QVector>

class TBox;
class TGaxis;

class PerdaixDisplay :
  public H2DPlot
{
  
public:
  PerdaixDisplay();
  ~PerdaixDisplay();
  
  double yStretchFactor() const {return m_yStretchFactor;}

protected:
  TGaxis*             m_yAxis;
  QVector<TBox*>      m_boxes;
  
  double m_yStretchFactor;
  
};

#endif /* PerdaixDisplay_hh */
