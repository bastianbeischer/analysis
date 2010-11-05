#ifndef Track_hh
#define Track_hh

#include <QVector>

#include <TVector3.h>

class Hit;

class Track
{
  
public:
  Track();
  ~Track();

public:
  void           setVerbose(unsigned short verb) {m_verbose = verb;}
  void           setX0(double x0)                {m_x0 = x0;}
  void           setY0(double y0)                {m_y0 = y0;}
  void           setSlopeX(double slopeX)        {m_slopeX = slopeX;}
  void           setSlopeY(double slopeY)        {m_slopeY = slopeY;}

public:
  unsigned short verbose()          const {return m_verbose;}
  double         x0()               const {return m_x0;}
  double         y0()               const {return m_y0;}
  double         slopeX()           const {return m_slopeX;}
  double         slopeY()           const {return m_slopeY;}
  double         chi2()             const {return m_chi2;}
  unsigned int   ndf()              const {return m_ndf;}
  QVector<Hit*>  hits()             const {return m_hits;}

  double         x(double z)        const {return m_x0 + z*m_slopeX;}
  double         y(double z)        const {return m_y0 + z*m_slopeY;}
  TVector3       position(double z) const {return TVector3(x(z), y(z), z);}

public:
  int            fit   (QVector<Hit*> hits);
  int            fit2D (QVector<Hit*> hits);
  int            fitTrd(QVector<Hit*> hits);

private:
  unsigned short    m_verbose;

  double            m_x0;
  double            m_slopeX;
  double            m_y0;
  double            m_slopeY;
  double            m_chi2;
  unsigned int      m_ndf;

  QVector<Hit*>     m_hits;

};

#endif /* Track_hh */
