#ifndef AlignmentMatrix_hh
#define AlignmentMatrix_hh

class Track;
class TestbeamSetup;

class AlignmentMatrix
{

public:
  AlignmentMatrix();
  ~AlignmentMatrix();

public:
  void fillMatrixFromTrack(Track* track);
  void resetArrays();
  void init();

protected:
  unsigned int m_nGlobal;
  unsigned int m_nLocal;

  float* m_globalDerivatives;
  float* m_localDerivatives;

};

#endif /* AlignmentMatrix_hh */
