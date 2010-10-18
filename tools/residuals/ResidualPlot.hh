#ifndef ResidualPlot_hh
#define ResidualPlot_hh

#include <map>

class Hit;
class Track;
class TCanvas;
class TH1I;
class TH2I;

class ResidualPlot
{
  
public:
  ResidualPlot();
  ResidualPlot(double z);
  ~ResidualPlot();
  
public:
  void fill(Hit* hit, Track* track);
  void draw();

private:
  double                               m_z;

  TCanvas*                             m_canvas1D;
  TCanvas*                             m_canvas2D;
  std::map<unsigned short, TH1I*>      m_residuals1D;
  TH2I*                                m_residuals2D;
  
};

#endif /* ResidualPlot_hh */
