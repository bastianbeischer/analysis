#ifndef Matrix_hh
#define Matrix_hh

class Track;
class TestbeamSetup;

class Matrix
{

public:
  Matrix();
  ~Matrix();

public:
  void fillMatrixFromTrack(Track* track);
  void init();

private:
  unsigned int indexForDetId(unsigned short detId);

protected:
  unsigned int m_nGlobal;
  unsigned int m_nLocal;

  float* m_globalDerivatives;
  float* m_localDerivatives;

};

#endif /* Matrix_hh */
