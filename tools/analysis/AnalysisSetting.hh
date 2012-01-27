#ifndef AnalysisSetting_hh
#define AnalysisSetting_hh

#include "AnalysisProcessorSetting.hh"

class AnalysisSetting : public AnalysisProcessorSetting {
public:
  AnalysisSetting();
  void clear();
  void save(const QString&) const;
  void load(const QString&);
  Enums::AnalysisTopics analysisTopics() const {return m_analysisTopics;}
  int firstEvent() const {return m_firstEvent;}
  int lastEvent() const {return m_lastEvent;}
  int numberOfThreads() const {return m_numberOfThreads;}
  void setAnalysisTopics(Enums::AnalysisTopics analysisTopics) {m_analysisTopics = analysisTopics;}
  void setFirstEvent(int firstEvent) {m_firstEvent = firstEvent;}
  void setLastEvent(int lastEvent) {m_lastEvent = lastEvent;}
  void setNumberOfThreads(int numberOfThreads) {m_numberOfThreads = numberOfThreads;}
protected:
  Enums::AnalysisTopics m_analysisTopics;
  int m_firstEvent;
  int m_lastEvent;
  int m_numberOfThreads;
};

QDebug operator<<(QDebug, const AnalysisSetting&);

#endif
