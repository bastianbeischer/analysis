#include "CutFilter.hh"

#include "AnalyzedEvent.hh"
#include "Cluster.hh"
#include "Particle.hh"

#include <QDebug>
#include <QStringList>

CutFilter::CutFilter()
{
}

CutFilter::CutFilter(const QString& string)
{
  if (string.isEmpty())
    return;
  QStringList stringList = string.split(" || ");
  foreach (QString cutString, stringList) {
    Cut cut(cutString);
    addCut(cut);
  }
}

CutFilter::~CutFilter()
{
}

void CutFilter::clear()
{
  m_cuts.clear();
}

void CutFilter::addCut(Cut cut)
{
  m_cuts.append(cut);
}

const QList<Cut>& CutFilter::cuts() const
{
  return m_cuts;
}

void CutFilter::setCuts(const CutFilter& cuts)
{
  m_cuts = cuts.m_cuts;
}

bool CutFilter::passes(const SimpleEvent* event) const
{
  for (int i = 0; i < m_cuts.size(); i++) {
    if(!m_cuts[i].passes(event))
      return false;
  }
  return true;
}

bool CutFilter::passes(const AnalyzedEvent* analyzedEvent) const
{
  for (int i = 0; i < m_cuts.size(); i++) {
    if(!m_cuts[i].passes(analyzedEvent))
      return false;
  }
  return true;
}

QString CutFilter::toString() const
{
  QString string;
  foreach(Cut cut, m_cuts) {
    if (!string.isEmpty())
      string+= " || ";
    string+= cut.toString();
  }
  return string;
}
