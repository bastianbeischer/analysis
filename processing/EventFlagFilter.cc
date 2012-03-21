#include "EventFlagFilter.hh"

#include "AnalyzedEvent.hh"

EventFlagFilter::EventFlagFilter()
  : m_filters()
{
}

void EventFlagFilter::addRequired(Enums::EventFlags flags)
{
  m_filters.append(Filter(true, flags));
}

void EventFlagFilter::addRequiredAbsence(Enums::EventFlags flags)
{
  m_filters.append(Filter(false, flags));
}

bool EventFlagFilter::passes(const AnalyzedEvent* event) const
{
  foreach (const Filter& filter, m_filters) {
    if (filter.first) {
      if (!event->flagsSet(filter.second))
        return false;
    } else {
      if (event->flagsSet(filter.second))
        return false;
    }
  }
  return true;
}
