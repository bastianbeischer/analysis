#ifndef MCFILTER_HH
#define MCFILTER_HH

#include <QVector>
#include <QList>

class Particle;
class SimpleEvent;
class Hit;

class MCFilter
{
public:
  MCFilter();
  bool passes(const QVector<Hit*>&, Particle*, SimpleEvent*);
  void addPdgID(int pdgID);
  void addPdgID(QList<int> pdgIds);
private:
  QList<int> m_pdgIDs;
};

#endif // MCFILTER_HH
