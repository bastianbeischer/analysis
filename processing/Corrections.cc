#include "Corrections.hh"

#include "Hit.hh"
#include "TOFSipmHit.hh"
#include "TOFCluster.hh"
#include "Setup.hh"
#include "DetectorElement.hh"
#include "TOFBar.hh"

#include <cstring>

Corrections::Corrections()
{
}

Corrections::~Corrections()
{
}

void Corrections::apply(QVector<Hit*>& hits, CorrectionFlags flags)
{
  foreach(Hit* hit, hits) {
    if (flags & Alignment) alignment(hit);
    if (flags & TimeShifts) timeShift(hit);
    if (flags & TrdMopv) trdMopv(hit);
    if (flags & TofTimeOverThreshold) tofTimeOverThreshold(hit);
  }
}

void Corrections::alignment(Hit* hit)
{
  Setup* setup = Setup::instance();
  DetectorElement* element = setup->element(hit->detId() - hit->channel());
  hit->setPosition(element->positionForHit(hit));
}

void Corrections::timeShift(Hit* hit)
{
  Setup* setup = Setup::instance();
  if (strcmp(hit->ClassName(), "TOFSipmHit") == 0) {
    TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(hit);
    DetectorElement* element = setup->element(hit->detId() - hit->channel());
    double timeShift = static_cast<TOFBar*>(element)->timeShift(hit->channel());
    tofHit->applyTimeShift(timeShift);
  }
  else if (strcmp(hit->ClassName(), "TOFCluster") == 0) {
    TOFCluster* cluster = static_cast<TOFCluster*>(hit);
    std::vector<Hit*> subHits = cluster->hits();
    for (std::vector<Hit*>::iterator it = subHits.begin(); it != subHits.end(); it++) {
      TOFSipmHit* tofHit = static_cast<TOFSipmHit*>(*it);
      DetectorElement* element = setup->element(cluster->detId() - cluster->channel());
      double timeShift = static_cast<TOFBar*>(element)->timeShift((*it)->channel());
      tofHit->applyTimeShift(timeShift);
    }
    cluster->processHits();
  }
}

void Corrections::trdMopv(Hit*)
{
}

void Corrections::tofTimeOverThreshold(Hit*)
{
}
