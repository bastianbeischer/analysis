#ifndef TRDMoPVTimeEvolutionPlot_hh
#define TRDMoPVTimeEvolutionPlot_hh


#include "AnalysisPlot.hh"
#include "GraphPlot.hh"
#include "SensorsData.hh"

#include <TGraphErrors.h>
#include <TH1.h>

#include <QMap>
#include <QStringList>

class TRDMoPVTimeEvolutionPlot : public AnalysisPlot, public GraphPlot
{
  
public:
  TRDMoPVTimeEvolutionPlot(AnalysisPlot::Topic);
  ~TRDMoPVTimeEvolutionPlot();

  virtual void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  virtual void finalize();

  void update();

private:
  void updateGraph();

  bool  m_graphNeedsUpdate;

  const double m_landauFitRange_lower;
  const double m_landauFitRange_upper;

  const unsigned int m_timeBinLength;

  QMap < unsigned int, QMap < unsigned short, TH1* > > m_binningMap;
  QMap < unsigned int, TGraphErrors* > m_mopvGraphs;

  //SensorsData* m_SensorsData;
  QStringList m_TrdTemperatureSensorNames;

  QVector < QMap<unsigned int, float> > m_TRDTempMaps;

};

#endif /* TRDMoPVTimeEvolutionPlot_hh */
