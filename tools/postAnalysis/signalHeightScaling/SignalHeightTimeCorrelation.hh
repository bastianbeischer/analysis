#ifndef SignalHeightTimeCorrelation_hh
#define SignalHeightTimeCorrelation_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QObject>
#include <QString>
#include <QMap>

class TGraphErrors;
class TH2D;

class SignalHeightTimeCorrelation : public QObject, public PostAnalysisPlot, public GraphPlot {
Q_OBJECT
public:
  SignalHeightTimeCorrelation(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTimeCorrelation();
private:
  TGraphErrors* meanGraph(unsigned short sipmId, TH2D*);
  void save(unsigned short sipmId);
  unsigned short m_sipmId;
  static QMap<unsigned short, TGraph*> s_graphs;
private slots:
  void save();
  void saveAll();
};

#endif
