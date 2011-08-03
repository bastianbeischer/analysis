#ifndef RootPlot_hh
#define RootPlot_hh

#include <QString>
#include <QVector>

class QWidget;
class TCanvas;
class TLatex;
class TLegend;
class TF1;

class RootPlot
{
public:
  enum DrawOption {
    UndefinedDrawOption,
    BLANK, E1X0,
    COLZ, SCAT, CONT4Z, LEGO, LEGO2, SURF1, COLZTEXT,
    AP, AC, ALP, ACP, ALX, P, L, C, HIST, LX
  };
  enum Type {Undefined, H1DPlot, H2DPlot, GraphPlot};

  RootPlot();
  virtual ~RootPlot();
  static TLatex* newLatex(double rx, double ry);
  static QString drawOption(DrawOption);
  virtual void draw(TCanvas*);
  virtual void positionChanged(double, double) {}
  virtual void finalize() {}
  virtual void update() {}
  virtual void unzoom() {}
  virtual void clear() {}
  void setTitle(const QString& title) {m_title = title;}
  const QString& title() const {return m_title;}
  int numberOfLatexs() const;
  void addLatex(TLatex*);
  TLatex* latex(int i = 0);
  int numberOfLegends() const;
  void addLegend(TLegend*);
  TLegend* legend(int i = 0);
  int numberOfFunctions() const;
  void addFunction(TF1*);
  TF1* function(int i = 0);
  DrawOption drawOption();
  void setDrawOption(DrawOption);
  Type type() {return m_type;}
  bool drawn() const {return m_drawn;}
  void setSecondaryWidget(QWidget* widget);
  QWidget* secondaryWidget() const {return m_secondaryWidget;}
protected:
  QString m_title;
  DrawOption m_drawOption;
  Type m_type;
  bool m_drawn;
private:
  QWidget* m_secondaryWidget;
  QVector<TLatex*> m_latex;
  QVector<TLegend*> m_legend;
  QVector<TF1*> m_function;
};

#endif /* RootPlot_hh */
