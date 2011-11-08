#ifndef SignalHeightTimeCor_hh
#define SignalHeightTimeCor_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>

#include <TGraphErrors.h>
#include <TF1.h>
#include <TH2D.h>

class SignalHeightTimeCor : public QObject, public PostAnalysisPlot, public GraphPlot {
  Q_OBJECT
public:
  SignalHeightTimeCor(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTimeCor();
private:
  void fit(unsigned short sipmId, TH2D*);
  void save(unsigned short sipmId);
  unsigned int m_sipmId;
  TGraphErrors m_graph;
  static QMap<unsigned short, TF1> s_SignalHeightTimeCorFits;
private slots:
  void save();
  void saveAll();
};

#endif
