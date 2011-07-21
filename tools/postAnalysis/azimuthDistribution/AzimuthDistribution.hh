#ifndef AzimuthDistribution_hh
#define AzimuthDistribution_hh

#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "H1DPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH1D.h>

class AzimuthDistribution : public PostAnalysisPlot, public H1DPlot {
public:
  AzimuthDistribution(PostAnalysisCanvas*);
  virtual ~AzimuthDistribution();
  TH1D* distribution() {return histogram();}
  TH1D* azimuthAcceptance() {return m_azimuthAcceptance;}
  
public:

private:
  TH1D* m_azimuthAcceptance;
  
  void readFile();
  double angleMovement(double azimuth);

};

#endif
