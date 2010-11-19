#ifndef RootPlot_hh
#define RootPlot_hh

#include <QString>

class TCanvas;

class RootPlot
{
  
public:
  RootPlot();
  virtual ~RootPlot();
  
public:
  virtual void draw(TCanvas*) = 0;
  virtual void clear() = 0;

  unsigned long id() const {return m_id;}
  void setTitle(const QString& title) {m_title = title;}
  const QString& title() const {return m_title;}

private:
  QString m_title;
  unsigned long m_id;
  static unsigned long s_rootPlotCounter;

};

#endif /* RootPlot_hh */
