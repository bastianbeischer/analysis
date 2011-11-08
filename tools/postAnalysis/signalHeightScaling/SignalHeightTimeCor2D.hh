#ifndef SignalHeightTimeCor2D_hh
#define SignalHeightTimeCor2D_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>

#include <TGraphErrors.h>
#include <TF1.h>
#include <TH2D.h>

class SignalHeightTimeCor2D : public PostAnalysisPlot, public H2DProjectionPlot {
public:
  SignalHeightTimeCor2D(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTimeCor2D();
private:
  unsigned short m_sipmId;
};

#endif
