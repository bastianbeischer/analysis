#ifndef SignalHeightTimeCorrelation_hh
#define SignalHeightTimeCorrelation_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QObject>
#include <QString>

#include <TGraphErrors.h>
#include <TH2D.h>

class SignalHeightTimeCorrelation : public QObject, public PostAnalysisPlot, public GraphPlot {
  Q_OBJECT
public:
  SignalHeightTimeCorrelation(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTimeCorrelation();
private:
  TGraphErrors* meanGraph(unsigned short sipmId, TH2D*);
  void save(unsigned short sipmId);
  unsigned int m_sipmId;
private slots:
  void save();
  void saveAll();
};

#endif
