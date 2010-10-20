#ifndef MatrixStraightLine_hh
#define MatrixStraightLine_hh

#include "Matrix.hh"

class MatrixStraightLine : public Matrix
{

public:
  MatrixStraightLine();
  ~MatrixStraightLine();

public:
  void fillMatrixFromTrack(Track* track);

};

#endif /* MatrixStraightLine_hh */
