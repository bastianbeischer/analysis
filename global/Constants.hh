#ifndef Constants_hh
#define Constants_hh

namespace Constants
{
  const double electronMass = 0.000511;
  const double muonMass = 0.105658;
  const double pionMass = 0.139570;
  const double pi0Mass = 0.1349766;
  const double protonMass = 0.938272;
  const double heliumMass = 4.002602 * protonMass;
  const double higgsMass = 200.314159;

  const double speedOfLight = 299.792458;           // mm/ns
  const double tofBarLength = 395;                  // mm
  const double tofBarWidth = 50;                    // mm
  const double tofBarHeight = 6;                    // mm
  const double tofRefractiveIndex = 1.58;
  const double upperTofPosition = 281.55;           // mm
  const double lowerTofPosition = -531.55;          // mm
  const double sigmaSipm = 0.6;                     // ns
  const double tofClusterSignalCut = 600;           // ns
  const double minimalTotPerSipm = 17.;             // ns
  const double triggerMatchingMinimum = 95.;        // ns
  const double triggerMatchingMaximum = 140.;       // ns
  const double idealTot = 30.;                      // ns
  const int nTofChannels = 64;
  const int nTofBars = 16;
  const int nTofSipmsPerBar = 4;

  const double TRDTubeRadius = 3.0;                 // mm

  const int runLength = 600;                        // seconds

  const double geometricAcceptance = 0.00318198;    // m^2sr
  const double geometricAcceptanceTof = 0.00848509; // m^2sr

  const int nRigidityBinsUnfolded = 31;
  const int nRigidityBinsRaw = 4 * nRigidityBinsUnfolded;
}

#endif
