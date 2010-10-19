#ifndef Matrix_hh
#define Matrix_hh

class Track;
class TestbeamSetup;

class Matrix
{

public:
  Matrix();
  virtual ~Matrix();

public:
  virtual void fillMatrixFromTrack(Track* track) = 0;
  void init();

protected:
  unsigned int m_nGlobal;
  unsigned int m_nLocal;

  float* m_globalDerivatives;
  float* m_localDerivatives;

};

#endif /* Matrix_hh */
