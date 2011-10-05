#ifndef Analysis_hh
#define Analysis_hh

#include "Enums.hh"
#include "AnalysisSetting.hh"

#include <QObject>
#include <QVector>
#include <QString>

class AnalysisPlot;
class EventReader;
class EventProcessor;
class TCanvas;

class Analysis : public QObject {
Q_OBJECT
public:
  Analysis();
  virtual ~Analysis();
  void start(const AnalysisSetting&);
  void stop();
  void clearPlots();
  unsigned int numberOfPlots();
  AnalysisPlot* plot(unsigned int);
  QVector<AnalysisPlot*> plots();
  QVector<unsigned int> plotIndices(Enums::AnalysisTopic topic);
  const QString& plotTitle(unsigned int);
  Enums::AnalysisTopic plotTopic(unsigned int);
  void save(const QString&, TCanvas*);
signals:
  void started();
  void finished();
private slots:
  void finalize();
  void addPlot(AnalysisPlot*);
private:
  AnalysisSetting m_analysisSetting;
  EventReader* m_reader;
  QVector<EventProcessor*> m_processors;
  QVector<AnalysisPlot*> m_plots;
  void setupPlots();
};

#endif
