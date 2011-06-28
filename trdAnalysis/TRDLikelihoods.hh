#ifndef TRDLIKELIHOODS_HH
#define TRDLIKELIHOODS_HH

class QMutex;
class QSettings;
class QString;

class TF1;

class TRDLikelihoods
{
public:
    TRDLikelihoods* instance();

    TF1* getPrototypeLHFunctionTR();
    TF1* getPrototypeLHFunctionNonTR();

    const TF1* getLHFunctionTR() const {return m_LHFunctionTR;}
    const TF1* getLHFunctionNonTR() const {return m_LHFunctionNonTR;}

    void saveLHFunctionTR(const TF1*);
    void saveLHFunctionNonTR(const TF1*);

private:
    TRDLikelihoods();

    void saveLHs();
    void loadLHs();

    void saveFunctionParameters(QString name, const TF1* func);
    bool loadFunctionParameters(TF1* func, QString name);

private:
    static TRDLikelihoods* m_instance;
    static QMutex m_mutex;

    TF1* m_LHFunctionTR;
    TF1* m_LHFunctionNonTR;

    QSettings* m_trdLikelihoodSettings;
};

#endif // TRDLIKELIHOODS_HH
