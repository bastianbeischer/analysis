#include "MCFilter.hh"

#include "SimpleEvent.hh"

MCFilter::MCFilter()
{
}


void MCFilter::addPdgID(int pdgID)
{
  m_pdgIDs << pdgID;
}

void MCFilter::addPdgID(QList<int> pdgIds)
{
  m_pdgIDs << pdgIds;
}

bool MCFilter::passes(const QVector<Hit*>&, Particle*, SimpleEvent* event)
{
 if (event->contentType() != SimpleEvent::MonteCarlo)
   return true;

 return m_pdgIDs.contains(event->MCInformation()->primary()->pdgID);
}
