#ifndef Track_hh
#define Track_hh

#include <QVector>

class Matrix;
class Hit;
class TVector3;

class Track
{
  
public:
  Track();
  ~Track();

public:
  enum Type {None, StraightLine, BrokenLine, CenteredBrokenLine};

  void             setVerbose(unsigned short verb) {m_verbose = verb;}

public:
  Type             type()             const {return m_type;}
  unsigned short   verbose()          const {return m_verbose;}
  int              fitGood()          const {return m_fitGood;}
  double           chi2()             const {return m_chi2;}
  unsigned int     ndf()              const {return m_ndf;}
  QVector<Hit*>    hits()             const {return m_hits;}
  
  virtual double   x(double z)        const = 0;
  virtual double   y(double z)        const = 0;
  virtual TVector3 position(double z) const = 0;

public:
  virtual int      fit   (QVector<Hit*> hits) = 0;

protected:
  Type              m_type;

  Matrix*           m_matrix;

  unsigned short    m_verbose;

  int               m_fitGood;
  double            m_chi2;
  unsigned int      m_ndf;

  QVector<Hit*>     m_hits;

};

#endif /* Track_hh */
