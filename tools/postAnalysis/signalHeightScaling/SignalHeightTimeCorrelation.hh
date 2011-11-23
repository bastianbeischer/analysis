#ifndef SignalHeightTimeCorrelation_hh
#define SignalHeightTimeCorrelation_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"
#include "Enums.hh"

#include <QObject>
#include <QString>
#include <QMap>

class RootQtWidget;
class TGraphErrors;
class TH2D;
class TSpline3;

class SignalHeightTimeCorrelation : public QObject, public PostAnalysisPlot, public GraphPlot {
Q_OBJECT
public:
  SignalHeightTimeCorrelation(PostAnalysisCanvas*, unsigned short sipmId, Enums::Situation);
  virtual ~SignalHeightTimeCorrelation();
public slots:
  void updateLocation(Enums::Situation);
private:
  TGraphErrors* meanGraph(unsigned short sipmId, TH2D*, TGraphErrors*, double referenceValue);
  void save(unsigned short sipmId);
  TH2D* m_histogram;
  unsigned short m_sipmId;
  RootQtWidget* m_secondaryRootWidget;
  TSpline3* m_spline;
  static QMap<unsigned short, TGraph*> s_graphs;
  void drawFactorGraph();
private slots:
  void save();
  void saveAll();
};

#endif
