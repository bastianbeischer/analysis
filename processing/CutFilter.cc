#include "CutFilter.hh"

#include <QDebug>
#include <QStringList>

#include "Particle.hh"
#include "ParticleDB.hh"
#include "SimpleEvent.hh"
#include <TVector3.h>

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
  //get mc rigidity
//  Particle particle(event->MCInformation()->primary()->pdgID);
//  double charge = particle.charge();
 // double genMom = event->MCInformation()->primary()->initialMomentum.Mag();
//  double genRigidity = genMom / charge;
//  if (qAbs(genRigidity) < 2.)
//    return false;
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
