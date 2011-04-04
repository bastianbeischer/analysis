#ifndef RootPlot_hh
#define RootPlot_hh

#include <QString>
#include <QVector>

class TCanvas;
class TLatex;
class TLegend;
class TF1;

class RootPlot
{
public:
  enum DrawOption {
    UndefinedDrawOption,
    BLANK, NOSTACK,
    COLZ, CONT4Z, LEGO, LEGO2, LEGOCOLZ, SURF1, COLZTEXT
  };
  enum Type {Undefined, H1DPlot, H2DPlot, GraphPlot};

  RootPlot();
  virtual ~RootPlot();
  static TLatex* newLatex(double rx, double ry);
  static QString drawOption(DrawOption);
  virtual void draw(TCanvas*);
  virtual void unzoom() {}
  virtual void clear() {}
  void setTitle(const QString& title) {m_title = title;}
  const QString& title() const {return m_title;}
  void addLatex(TLatex*);
  TLatex* latex(int i = 0);
  void addLegend(TLegend*);
  TLegend* legend(int i = 0);
  void addFunction(TF1*);
  TF1* function(int i = 0);
  virtual void finalize() {}
  virtual void update() {}
  DrawOption drawOption();
  void setDrawOption(DrawOption);
  Type type() {return m_type;}
  bool drawn() const {return m_drawn;}
protected:
  QString m_title;
  DrawOption m_drawOption;
  Type m_type;
  bool m_drawn;
private:
  QVector<TLatex*> m_latex;
  QVector<TLegend*> m_legend;
  QVector<TF1*> m_function;
};

#endif /* RootPlot_hh */
