#ifndef timeOverThresholdScaling_hh
#define timeOverThresholdScaling_hh

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

class TimeOverThresholdScaling : public QObject, public PostAnalysisPlot, public GraphPlot {
  Q_OBJECT
public:
  TimeOverThresholdScaling(PostAnalysisCanvas*, unsigned int tofId);
  virtual ~TimeOverThresholdScaling();
private:
  void fit(unsigned int tofId, TH2D* histogram);
  void save(unsigned int tofId);
  unsigned int m_tofId;
  static QMap<unsigned int, TGraphErrors> s_timeOverThresholdScalingGraphs;
  static QMap<unsigned int, TF1> s_timeOverThresholdScalingFits;
  static QMap<unsigned int, double> s_minTofTemps;
  static QMap<unsigned int , double> s_maxTofTemps;
private slots:
  void save();
  void saveAll();
};

#endif
