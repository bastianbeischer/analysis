#ifndef AlignmentMatrix_hh
#define AlignmentMatrix_hh

#include "EventDestination.hh"

class Particle;
class TestbeamSetup;

class AlignmentMatrix :
  public EventDestination
{

public:
  AlignmentMatrix();
  ~AlignmentMatrix();

public:
  void processEvent(const QVector<Hit*>&, const Particle* const = 0, const SimpleEvent* const = 0);
  void resetArrays();
  void init();

protected:
  unsigned int m_nGlobal;
  unsigned int m_nLocal;

  float* m_globalDerivatives;
  float* m_localDerivatives;

};

#endif /* AlignmentMatrix_hh */
