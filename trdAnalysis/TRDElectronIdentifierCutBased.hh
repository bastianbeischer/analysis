#ifndef TRDELECTRONIDENTIFIERCUTBASED_HH
#define TRDELECTRONIDENTIFIERCUTBASED_HH

#include "TRDElectronIdentifier.hh"

class TRDElectronIdentifierCutBased : public TRDElectronIdentifier
{
public:
    TRDElectronIdentifierCutBased();
    bool isElectronish(const QVector<Hit*>& hits, const Particle* particle, const SimpleEvent* event, bool &ok);
private:
    unsigned int m_layerCut;
    double m_energyDepositionCut;
    double m_energyDepositionPerLengthCut;
};

#endif // TRDELECTRONIDENTIFIERCUTBASED_HH
