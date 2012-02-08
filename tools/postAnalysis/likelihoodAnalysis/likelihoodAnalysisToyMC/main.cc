#include "Enums.hh"
#include "Likelihood.hh"
#include "LikelihoodPDF.hh"
#include "Hypothesis.hh"
#include "RootStyle.hh"
#include "Particle.hh"
#include "Helpers.hh"

#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TRandom.h>
#include <TROOT.h>
#include <TFile.h>

#include <QSettings>
#include <QFile>
#include <QMapIterator>
#include <QVectorIterator>
#include <QDebug>

#include <iostream>
#include <cstdlib>
#include <cmath>

typedef QVector<LikelihoodPDF*> LikelihoodVector;
typedef QMap<double, LikelihoodVector> RigidityMap;
typedef QMap<Enums::Particle, RigidityMap> ParticleMap;

void dump(const ParticleMap& map)
{
  QMapIterator<Enums::Particle, RigidityMap> particleIt(map);
  while (particleIt.hasNext()) {
    particleIt.next();
    qDebug() << Enums::label(particleIt.key());
    QMapIterator<double, LikelihoodVector> rigidityIterator(particleIt.value());
    while (rigidityIterator.hasNext()) {
      rigidityIterator.next();
      qDebug() << " " << rigidityIterator.key() << "GV";
      QVectorIterator<LikelihoodPDF*> likelihoodIterator(rigidityIterator.value());
      while (likelihoodIterator.hasNext()) {
        LikelihoodPDF* pdf = likelihoodIterator.peekNext();
        qDebug() << "  " << Enums::label(pdf->likelihoodVariableType()) << pdf->variable();
        likelihoodIterator.next();
      }
    }
  }
}

Hypothesis randomHypothesis()
{

  Enums::Particle particle = Enums::NoParticle;
  if (gRandom->Uniform() < 0.5) {
    particle = Enums::Helium;
  } else {
    particle = Enums::Proton;
  }
  double rigidity = 3.;//.5 +gRandom->Uniform(9.);
  return Hypothesis(particle, 1./rigidity);
}

RigidityMap::ConstIterator closestRigidity(const RigidityMap& map, double rigidity)
{
  Q_ASSERT(map.count());
  RigidityMap::ConstIterator lower = map.lowerBound(rigidity);
  return (lower == map.end()) ? --map.end() : lower;
}

const LikelihoodVector& closestPdfs(const ParticleMap& particleMap, const Hypothesis& h)
{
  ParticleMap::ConstIterator particleIt = particleMap.find(h.particle());
  Q_ASSERT(particleIt != particleMap.end());
  const RigidityMap& rigidityMap = particleIt.value();
  RigidityMap::ConstIterator rigidityIt = closestRigidity(rigidityMap, h.absoluteRigidity());
  Q_ASSERT(rigidityIt != rigidityMap.end());
  return rigidityIt.value();
}

void saveCanvases(const QString& fileName, QVector<TCanvas*> vector)
{
  TFile file(qPrintable(fileName), "RECREATE");
  foreach (TCanvas* canvas, vector)
    canvas->Write();
  file.Close();
}

void save(TFile* file, QMap<Enums::Particle, TH2D*>& histograms) {
  foreach (TH2D* h, histograms) {
    QString canvasTitle = QString("%1 canvas").arg(h->GetName());
    TCanvas* canvas = new TCanvas(qPrintable(canvasTitle), qPrintable(canvasTitle));
    h->Draw("COLZ");
    gPad->SetLogz();
    file->cd();
    canvas->Write();
    gROOT->cd();
  }
}

int main(int argc, char* argv[])
{
  RootStyle::set();
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " numberOfEvents configFile.conf" << std::endl;
    return EXIT_FAILURE;
  }
  QString programName = argv[0];
  int nEvents = QString(argv[1]).toInt();
  QString configName = argv[2];

  Q_ASSERT(QFile::exists(configName));
  QSettings settings(configName, QSettings::IniFormat);
  settings.beginGroup(programName);
  QString particlesString = settings.value("particles").toString();
  Enums::Particles particles = Enums::particles(particlesString);
  QVector<Enums::Particle> particlesVector = Enums::particleVector(particles);
  QString variablesString = settings.value("likelihoods").toString();
  Enums::LikelihoodVariables variables = Enums::likelihoodVariables(variablesString);
  QVector<Enums::LikelihoodVariable> variablesVector = Enums::likelihoodVariableVector(variables);
  settings.endGroup();

  QVector<Likelihood*> likelihoods;
  foreach (Enums::LikelihoodVariable variable, variablesVector)
    likelihoods.append(Likelihood::newLikelihood(variable, particles));

  double rigidityMin = 0;
  double rigidityMax = 10.;
  double rigidityBinning = .01;
  int numberOfRigidityBins = (rigidityMax - rigidityMin) / rigidityBinning;
  QVector<double> logLikelihoodBinning = Helpers::logLikelihoodBinning();

  ParticleMap particleMap;
  foreach (Enums::Particle particle, particlesVector) {
    ParticleMap::Iterator particleIt = particleMap.insert(particle, RigidityMap());
    for (double rigidity = rigidityMin; rigidity <= rigidityMax; rigidity+= rigidityBinning) {
      RigidityMap::Iterator rigidityIt = particleIt.value().insert(rigidity, LikelihoodVector());
      foreach (Likelihood* likelihood, likelihoods) {
        KineticVariable variable(particle, Enums::AbsoluteRigidity, rigidity);
        rigidityIt.value().append(likelihood->pdf(variable));
      }
    }
  }

  TApplication application("likelihoodAnalysisToyMC", &argc, argv);

  QMap<Enums::Particle, TH2D*> logLikelihoodSignalHistograms;
  QMap<Enums::Particle, TH2D*> logLikelihoodBackgroundHistograms;
  foreach (Enums::Particle particle, particlesVector) {
    QString title = "log likelihood histogram " + Enums::label(particle);
    TH2D* h = 0;
    QString sLabel = Helpers::greekifyLetters(Enums::label(particle));
    QString bgLabel = Helpers::greekifyLetters(Enums::label(particles & ~particle));
    QString axisTitles = ";|R| / GV;-2 ln (L_{" + sLabel + "} / (L_{" + sLabel + "} + L_{"+ bgLabel + "}));";
    h = new TH2D(qPrintable(title + " signal"), qPrintable(axisTitles),
      numberOfRigidityBins, rigidityMin, rigidityMax, logLikelihoodBinning.count() - 1, logLikelihoodBinning.constData());
    logLikelihoodSignalHistograms.insert(particle, h);
    h = new TH2D(qPrintable(title + " background"), qPrintable(axisTitles),
      numberOfRigidityBins, rigidityMin, rigidityMax, logLikelihoodBinning.count() - 1, logLikelihoodBinning.constData());
    logLikelihoodBackgroundHistograms.insert(particle, h);
  }

  Particle particle;
  for (int event = 0; event < nEvents; ++event) {
    Hypothesis trueHypothesis = randomHypothesis();
    Q_ASSERT(rigidityMin <= trueHypothesis.absoluteRigidity() && trueHypothesis.absoluteRigidity() <= rigidityMax);
    const LikelihoodVector& likelihoodVector = closestPdfs(particleMap, trueHypothesis);

    QMap<Enums::LikelihoodVariable, double> randomValues;
    foreach (LikelihoodPDF* pdf, likelihoodVector) {
      //if (trueHypothesis.particle() == Enums::Proton) std::cout << pdf->GetRandom() << std::endl;
      randomValues.insert(pdf->likelihoodVariableType(), pdf->GetRandom());
    }

    particle.reset();
    foreach (Enums::Particle particleHypothesis, particlesVector) {
      //TODO: use a smeared curvature value for h instead of the true.
      Hypothesis* h = new Hypothesis(particleHypothesis, trueHypothesis.curvature());
      double L = 1.;
      int n = likelihoods.count();
      Q_ASSERT(n == randomValues.count());
      for (int i = 0; i < n; ++i) {
        Likelihood* lh = likelihoods[i];
        QMap<Enums::LikelihoodVariable, double>::ConstIterator randomIt = randomValues.find(lh->likelihoodVariableType());
        Q_ASSERT(randomIt != randomValues.end());
        Q_ASSERT(randomIt.key() == lh->likelihoodVariableType());
        L*= lh->eval(randomIt.value(), *h);
      }
      h->setLikelihood(n, L);
      particle.addHypothesis(Enums::UndefinedReconstructionMethod, h);
    }

    foreach (Enums::Particle signalParticle, particlesVector) {
      double s = particle.signalLikelihood(Enums::UndefinedReconstructionMethod, signalParticle);
      double b = particle.backgroundLikelihood(Enums::UndefinedReconstructionMethod, particles & ~signalParticle);

      QMap<Enums::Particle, TH2D*>::Iterator logLikelihoodSignalHistogramIt = logLikelihoodSignalHistograms.find(signalParticle);
      Q_ASSERT(logLikelihoodSignalHistogramIt != logLikelihoodSignalHistograms.end());
      QMap<Enums::Particle, TH2D*>::Iterator logLikelihoodBackgroundHistogramIt = logLikelihoodBackgroundHistograms.find(signalParticle);
      Q_ASSERT(logLikelihoodBackgroundHistogramIt != logLikelihoodBackgroundHistograms.end());
      const Hypothesis* h = particle.hypothesis(Enums::UndefinedReconstructionMethod, signalParticle);
      Q_ASSERT(h);
      double likelihoodRatio = -2*log(s/(s+b));
      if (trueHypothesis.particle() == signalParticle) {
        logLikelihoodSignalHistogramIt.value()->Fill(h->absoluteRigidity(), likelihoodRatio);
      } else {
        logLikelihoodBackgroundHistogramIt.value()->Fill(h->absoluteRigidity(), likelihoodRatio);
      }
    }

    if (event && (event % 10000) == 0)
      std::cerr << '.' << std::flush;
  }
  std::cerr << std::endl;

  TFile file("likelihoodAnalysisToyMC.root", "RECREATE");
  gROOT->cd();
  save(&file, logLikelihoodSignalHistograms);
  save(&file, logLikelihoodBackgroundHistograms);
  file.Close();

  application.Run();

  foreach (RigidityMap rigidityMap, particleMap)
    foreach (LikelihoodVector likelihoodVector, rigidityMap)
      qDeleteAll(likelihoodVector);
  qDeleteAll(likelihoods);
  likelihoods.clear();

  return EXIT_SUCCESS;
}
