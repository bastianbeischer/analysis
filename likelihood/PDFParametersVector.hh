#ifndef PDFParametersVector_hh
#define PDFParametersVector_hh

#include "PDFParameters.hh"

#include <QVector>

class PDFParametersVector : public QVector<PDFParameters>
{
public:
  PDFParametersVector(int numberOfElements, int numberOfParameters);
  void setRange(double min, double max);
  double min() const;
  double max() const;
private:
  double m_min;
  double m_max;
};

#endif
