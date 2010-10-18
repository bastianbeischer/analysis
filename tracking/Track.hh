#ifndef Track_hh
#define Track_hh

#include <vector>

#include <TVector3.h>

class Hit;
class TH2I;

class Track
{
  
public:
  Track();
  ~Track();
  
public:
  double       x0()     const {return m_x0;}
  double       y0()     const {return m_y0;}
  double       slopeX() const {return m_slopeX;}
  double       slopeY() const {return m_slopeY;}
  double       chi2()   const {return m_chi2;}
  unsigned int ndf()    const {return m_ndf;}

public:
  void setX0(double x0) {m_x0 = x0;}
  void setY0(double y0) {m_y0 = y0;}
  void setSlopeX(double slopeX) {m_slopeX = slopeX;}
  void setSlopeY(double slopeY) {m_slopeY = slopeY;}

public:
  int fit   (std::vector<Hit*> hits);
  int fit2D (std::vector<Hit*> hits);
  int fitTrd(std::vector<Hit*> hits);

public:
  double   x(double z)        const {return m_x0 + z*m_slopeX;}
  double   y(double z)        const {return m_y0 + z*m_slopeY;}
  TVector3 position(double z) const {return TVector3(x(z), y(z), z);}

  bool isInCorridor(Hit* hit) const;

private:
  double            m_x0;
  double            m_slopeX;
  double            m_y0;
  double            m_slopeY;
  double            m_chi2;
  unsigned int      m_ndf;
  
};

#endif /* Track_hh */
