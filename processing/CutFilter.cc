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

bool CutFilter::passes(const QVector<Hit*>& clusters, Particle* particle, SimpleEvent* event)
{
  for (int i = 0; i < m_cuts.size(); i++) {
    if(!m_cuts[i].passes(clusters, particle, event))
      return false;
  }
  return true;
}

void CutFilter::setCuts(CutFilter cuts)
{
  m_cuts = cuts.m_cuts;
}
