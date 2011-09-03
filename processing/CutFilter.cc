#include "CutFilter.hh"

CutFilter::CutFilter()
{
}

CutFilter::~CutFilter()
{
}

void CutFilter::addCut(Cut cut)
{
  m_cuts.push_back(cut);
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

bool CutFilter::passes(const QVector<Hit*>& clusters, const Particle* particle) const
{
  for (int i = 0; i < m_cuts.size(); i++) {
    if(!m_cuts[i].passes(clusters, particle))
      return false;
  }
  return true;
}
