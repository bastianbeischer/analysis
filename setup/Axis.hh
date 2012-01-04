#ifndef Axis_hh
#define Axis_hh

class Axis
{

public:
  explicit Axis(double, double, int);
  ~Axis();

  double binCenter(int);
  int bin(double);
  int nBins() {return m_nBins;}

private:
  double m_x0;
  double m_x1;
  int m_nBins;

};

#endif /* Axis_hh */
