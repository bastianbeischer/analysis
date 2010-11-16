#ifndef RootPlot_hh
#define RootPlot_hh

class TCanvas;

class RootPlot
{
  
public:
  RootPlot();
  virtual ~RootPlot();
  
public:
  virtual void draw(TCanvas*) = 0;
  virtual void clear() = 0;

};

#endif /* RootPlot_hh */
