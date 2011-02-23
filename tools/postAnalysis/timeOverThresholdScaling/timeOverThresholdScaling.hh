#ifndef timeOverThresholdScaling_hh
#define timeOverThresholdScaling_hh

#include "PostAnalysisPlot.hh"
#include "GraphPlot.hh"

#include <QString>
#include <QVector>
#include <QMap>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TH2D.h>

class TimeOverThresholdScaling : public PostAnalysisPlot, public GraphPlot {
public:
  TimeOverThresholdScaling(TCanvas*, unsigned int tofId);
  virtual ~TimeOverThresholdScaling();
  
public:
  static QMap<unsigned int, TGraphErrors> timeOverThresholdScalingGraphs;
  static QMap<unsigned int, TF1> timeOverThresholdScalingFits;
  static QMap<unsigned int, double> minTofTemps;
  static QMap<unsigned int , double> maxTofTemps;
  
private:
  void scaling(unsigned int tofId, TH2D* histogram);

};

#endif
