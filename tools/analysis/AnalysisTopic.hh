#ifndef AnalysisTopic_h
#define AnalysisTopic_h

#include "Enums.hh"

#include <QVector>

class AnalysisTopic {
public:
  static const QVector<Enums::AnalysisTopic>& topics();
  static const QVector<Enums::AnalysisTopic>& trackerTopics();
  static const QVector<Enums::AnalysisTopic>& trdTopics();
  static const QVector<Enums::AnalysisTopic>& tofTopics();
  static const QVector<Enums::AnalysisTopic>& otherTopics();
private:
  static const QVector<Enums::AnalysisTopic> s_topics;
  static const QVector<Enums::AnalysisTopic> s_trackerTopics;
  static const QVector<Enums::AnalysisTopic> s_trdTopics;
  static const QVector<Enums::AnalysisTopic> s_tofTopics;
  static const QVector<Enums::AnalysisTopic> s_otherTopics;
};

#endif
