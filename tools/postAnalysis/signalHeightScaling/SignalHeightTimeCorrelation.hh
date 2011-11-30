#ifndef SignalHeightTimeCorrelation_hh
#define SignalHeightTimeCorrelation_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QObject>
#include <QString>
#include <QMap>

class RootQtWidget;
class TGraphErrors;
class TH2D;
class TSpline3;
class TF1;

class SignalHeightTimeCorrelation : public QObject, public PostAnalysisPlot, public GraphPlot {
Q_OBJECT
public:
  SignalHeightTimeCorrelation(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTimeCorrelation();
private:
  TGraphErrors* meanGraph(unsigned short sipmId, TH2D*);
  void save(unsigned short sipmId);
  TH2D* m_histogram;
  unsigned short m_sipmId;
  RootQtWidget* m_secondaryRootWidget;
  TSpline3* m_spline;
  static QMap<unsigned short, TGraph*> s_factorGraphs;
  void drawFactorGraph();
private slots:
  void save();
  void saveAll();
};

#endif
