#ifndef RootPlot_hh
#define RootPlot_hh

#include <QString>
#include <QVector>

class TCanvas;
class TLatex;
class TLegend;

class RootPlot
{
public:
  RootPlot();
  virtual ~RootPlot();
public:
  virtual void draw(TCanvas*);
  virtual void clear() {}
  unsigned long id() const {return m_id;}
  void setTitle(const QString& title) {m_title = title;}
  const QString& title() const {return m_title;}
  void addLatex(TLatex*);
  TLatex* latex(int i = 0);
  void addLegend(TLegend*);
  TLegend* legend(int i = 0);
private:
  QString m_title;
  unsigned long m_id;
  QVector<TLatex*> m_latex;
  QVector<TLegend*> m_legend;
};

#endif /* RootPlot_hh */
