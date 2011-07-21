#ifndef TRDLIKELIHOODS_HH
#define TRDLIKELIHOODS_HH

class QMutex;
class QSettings;
class QString;

class TF1;

#include <QList>

class TRDLikelihoods
{
public:
  static TRDLikelihoods* instance();

  TF1* prototypeLHFunctionTR();
  TF1* prototypeLHFunctionNonTR();

  const TF1* lhFunctionTR() const {return m_LHFunctionTR;}
  const TF1* lhFunctionTRLayer(int i) const {return m_LHFunctionTRLayer.at(i);}
  const TF1* lhFunctionNonTR() const {return m_LHFunctionNonTR;}

  void setLHFunctionTR(const TF1*);
  void setLHFunctionTRLayer(int i, const TF1*);
  void setLHFunctionNonTR(const TF1*);

private:
  TRDLikelihoods();

  void saveLHs();
  void loadLHs();

  void saveFunctionParameters(QString name, const TF1* func);
  bool loadFunctionParameters(TF1* func, QString name);

private:
  static TRDLikelihoods* m_instance;
  static QMutex m_mutex;

  bool m_normalizedToLength;

  TF1* m_LHFunctionTR;
  TF1* m_LHFunctionNonTR;

  QList<TF1*> m_LHFunctionTRLayer;

  QSettings* m_trdLikelihoodSettings;
};

#endif // TRDLIKELIHOODS_HH
