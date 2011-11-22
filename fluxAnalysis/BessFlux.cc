#include "BessFlux.hh"

#include "BessFlux.hh"
#include "Constants.hh"
#include "Helpers.hh"

#include <QDebug>
#include <QStringList>

#include <TH1.h>

QMap<BessFlux::Type, QString> BessFlux::s_typeNames;

BessFlux* BessFlux::s_instance = 0;

BessFlux* BessFlux::instance()
{
  if (!s_instance)
    s_instance = new BessFlux();
  return s_instance;
}

BessFlux::BessFlux()
  : m_fluxes()
{
  int nBess = 65;
  const double bessXaxis[] = {0.576, 0.621, 0.669, 0.72, 0.776, 0.836, 0.901, 0.97, 1.04, 1.13, 1.21, 1.31, 1.41, 1.52, 1.63, 1.76, 1.9, 2.04, 2.2, 2.37, 2.55, 2.75, 2.96, 3.19, 3.44, 3.71, 3.99, 4.3, 4.63, 4.99, 5.38, 5.79, 6.24, 6.73, 7.25, 7.81, 8.41, 9.06, 9.76, 10.5, 11.3, 12.2, 13.2, 14.2, 15.3, 16.4, 17.7, 19.1, 20.6, 23.9, 27.7, 32.1, 37.3, 43.3, 50.2, 58.3, 67.7, 78.5, 91.1, 106, 132, 165, 207, 258, 323 , 404};
  const double bessMuPlusFlux[] = {13.4, 13.3, 13.2, 12.7, 12.6, 12.3, 12.1, 11.6, 11, 10.7, 10.2, 9.78, 9.38, 8.72, 8.59, 7.85, 7.41, 7.03, 6.38, 6.01, 5.45, 5.02, 4.62, 4.17, 3.79, 3.37, 3.02, 2.74, 2.41, 2.11, 1.87, 1.63, 1.44, 1.21, 1.06, 0.911, 0.807, 0.706, 0.567, 0.49, 0.438, 0.358, 0.307, 0.249, 0.213, 0.176, 0.149, 0.122, 0.0957, 0.0669, 0.0452, 0.0293, 0.0201, 0.0131, 0.00876, 0.00572, 0.00401, 0.00255, 0.00155, 0.000943, 0.000426, 0.000251, 0.000118, 0.0000558, 0.0000256};
  const double bessMuMinusFlux[] = {12.5, 12.2, 11.9, 11.5, 11.3, 10.8, 10.6, 10, 9.6, 9.1, 8.8, 8.53, 8.01, 7.53, 7.22, 6.72, 6.28, 5.71, 5.36, 4.92, 4.62, 4.09, 3.69, 3.31, 3.06, 2.7, 2.37, 2.11, 1.87, 1.66, 1.46, 1.24, 1.13, 0.96, 0.83, 0.693, 0.608, 0.515, 0.454, 0.385, 0.324, 0.274, 0.231, 0.197, 0.168, 0.14, 0.115, 0.096, 0.0736, 0.0504, 0.0348, 0.0226, 0.0155, 0.0104, 0.00691, 0.00454, 0.00288, 0.00193, 0.00117, 0.000695, 0.000365, 0.000147, 0.000075, 0.0000413, 0.0000245};
  const double bessMuPlusFluxErrorSys[] = {0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.2, 0.2, 0.2, 0.2, 0.21, 0.2, 0.18, 0.18, 0.17, 0.16, 0.15, 0.13, 0.13, 0.13, 0.12, 0.11, 0.1, 0.09, 0.08, 0.07, 0.06, 0.06, 0.05, 0.04, 0.04, 0.03, 0.03, 0.02, 0.021, 0.019, 0.016, 0.013, 0.011, 0.01, 0.008, 0.007, 0.006, 0.005, 0.004, 0.003, 0.003, 0.0022, 0.0016, 0.001, 0.0007, 0.0005, 0.0003, 0.00022, 0.00015, 0.00011, 0.00008, 0.00005, 0.00004, 0.000022, 0.000015, 0.000009, 0.0000052, 0.000003};
  const double bessMuMinusFluxErrorSys[] = {0.3, 0.3, 0.3, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.18, 0.17, 0.16, 0.15, 0.14, 0.13, 0.12, 0.11, 0.1, 0.1, 0.09, 0.08, 0.07, 0.06, 0.06, 0.05, 0.04, 0.04, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02, 0.015, 0.013, 0.011, 0.01, 0.008, 0.007, 0.006, 0.005, 0.004, 0.004, 0.003, 0.002, 0.002, 0.0015, 0.0011, 0.0007, 0.0005, 0.0003, 0.0002, 0.00016, 0.00011, 0.00008, 0.00006, 0.00004, 0.000028, 0.000018, 0.000009, 0.000006, 0.0000039, 0.0000028};
  TH1D* bessMuPlus = new TH1D("BESS mu+", "BESS mu+", nBess, bessXaxis);
  TH1D* bessMuMinus = new TH1D("BESS mu-", "BESS mu-", nBess, bessXaxis);
  for (int i = 0; i < nBess; i++) {
    bessMuPlus->SetBinContent(i+1, bessMuPlusFlux[i]);
    bessMuPlus->SetBinError(i+1, bessMuPlusFluxErrorSys[i]);
    bessMuPlus->SetLineColor(kViolet);
    bessMuPlus->SetMarkerColor(kViolet);
    bessMuMinus->SetBinContent(i+1, bessMuMinusFlux[i]);
    bessMuMinus->SetBinError(i+1, bessMuMinusFluxErrorSys[i]);
    bessMuMinus->SetLineColor(kViolet-3);
    bessMuMinus->SetMarkerColor(kViolet-3);
  }
  m_fluxes.insert(MuPlus, bessMuPlus);
  m_fluxes.insert(MuMinus, bessMuMinus);
  foreach (TH1D* flux, m_fluxes) {
    flux->Sumw2();
  }
}

BessFlux::~BessFlux()
{
  qDeleteAll(m_fluxes);
  s_instance = 0;
}

TH1D* BessFlux::spectrum(Type type) const
{
  TH1D* flux = new TH1D(*m_fluxes[type]);
  if (!flux->GetSumw2())
    flux->Sumw2();
  return flux;
}

TH1D* BessFlux::spectrum(const QList<Type>& keys) const
{
  TH1D* flux = spectrum(keys.at(0));
  if (!flux->GetSumw2())
    flux->Sumw2();
  QString title = flux->GetTitle();
  for (int i = 1; i < keys.size(); i++) {
    flux->Add(m_fluxes[keys.at(i)]);
    QString currentTitle = typeName(keys.at(i));
    currentTitle.remove("BESS");
    title.append(currentTitle);
  }
  title.append(" sum");
  flux->SetTitle(qPrintable(title));
  flux->SetLineColor(kViolet - 6);
  flux->SetMarkerColor(kViolet - 6);
  return flux;
}

QVector<BessFlux::Type> BessFlux::types() const
{
  QVector<Type> types;
  foreach (Type type, m_fluxes.keys())
    if (!types.contains(type))
      types.append(type);
  return types;
}

void BessFlux::construct()
{
  s_typeNames.insert(MuPlus, "BESS mu+");
  s_typeNames.insert(MuMinus, "BESS mu-");
}

BessFlux::Type BessFlux::type(const QString& name)
{
  construct();
  return s_typeNames.key(name);
}

QString BessFlux::typeName(Type type)
{
  construct();
  return s_typeNames[type];
}
