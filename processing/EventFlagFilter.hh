#ifndef EventFlagFilter_hh
#define EventFlagFilter_hh

#include "Enums.hh"

#include <QPair>
#include <QVector>

class AnalyzedEvent;

class EventFlagFilter
{
public:
  EventFlagFilter();
  void addRequired(Enums::EventFlags);
  void addRequiredAbsence(Enums::EventFlags);
  bool passes(const AnalyzedEvent*) const;
private:
  typedef QPair<bool, Enums::EventFlags> Filter;
  QVector<Filter> m_filters;
};

#endif
