#ifndef PDFParameters_hh
#define PDFParameters_hh

#include <QVector>

class PDFParameters : public QVector<double>
{
public:
  PDFParameters();
  PDFParameters(int numberOfParameters);
  void setNormalizationFactor(double);
  double normalizationFactor() const;
private:
  double m_normalizationFactor;
};

#endif
