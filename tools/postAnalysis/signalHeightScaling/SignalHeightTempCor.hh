#ifndef SignalHeightTempCor_hh
#define SignalHeightTempCor_hh

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

class SignalHeightTempCor : public QObject, public PostAnalysisPlot, public GraphPlot {
  Q_OBJECT
public:
  SignalHeightTempCor(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTempCor();
private:
  void fit(unsigned short sipmId, TH2D*);
  void save(unsigned short sipmId);
  unsigned int m_sipmId;
  TGraphErrors m_graph;
  static QMap<unsigned short, TF1> s_SignalHeightTempCorFits;
private slots:
  void save();
  void saveAll();
};

#endif
