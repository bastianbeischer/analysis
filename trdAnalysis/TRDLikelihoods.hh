#ifndef TRDLIKELIHOODS_HH
#define TRDLIKELIHOODS_HH

class TF1;

class TRDLikelihoods
{
public:
    TRDLikelihoods();

    static TF1* getPrototypeLHFunctionTR();
    static TF1* getPrototypeLHFunctionNonTR();

    static double functionNonTR(double *x, double *par);
};

#endif // TRDLIKELIHOODS_HH
