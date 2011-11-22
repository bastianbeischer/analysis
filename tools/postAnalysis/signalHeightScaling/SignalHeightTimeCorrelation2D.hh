#ifndef SignalHeightTimeCorrelation2D_hh
#define SignalHeightTimeCorrelation2D_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H2DProjectionPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>

#include <TGraphErrors.h>
#include <TF1.h>
#include <TH2D.h>

class SignalHeightTimeCorrelation2D : public H2DProjectionPlot, public PostAnalysisPlot {
Q_OBJECT
public:
  SignalHeightTimeCorrelation2D(PostAnalysisCanvas*, unsigned short sipmId);
  virtual ~SignalHeightTimeCorrelation2D();
  virtual void updateProjection();
private:
  unsigned short m_sipmId;
};

#endif
