#ifndef SignalHeightTimeCor_hh
#define SignalHeightTimeCor_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QObject>
#include <QString>

#include <TGraphErrors.h>
#include <TH2D.h>

class SignalHeightTimeCor : public QObject, public PostAnalysisPlot, public GraphPlot {
  Q_OBJECT
public:
  SignalHeightTimeCor(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTimeCor();
private:
  TGraphErrors* meanGraph(unsigned short sipmId, TH2D*);
  void save(unsigned short sipmId);
  unsigned int m_sipmId;
private slots:
  void save();
  void saveAll();
};

#endif
