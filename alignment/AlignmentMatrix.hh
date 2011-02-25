#ifndef AlignmentMatrix_hh
#define AlignmentMatrix_hh

#include "EventDestination.hh"

class Track;
class TestbeamSetup;

class AlignmentMatrix :
  public EventDestination
{

public:
  AlignmentMatrix();
  ~AlignmentMatrix();

public:
  void processEvent(const QVector<Hit*>&, Track* = 0, SimpleEvent* = 0);
  void resetArrays();
  void init();

protected:
  unsigned int m_nGlobal;
  unsigned int m_nLocal;

  float* m_globalDerivatives;
  float* m_localDerivatives;

};

#endif /* AlignmentMatrix_hh */
