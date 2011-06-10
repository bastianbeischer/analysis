#include "MoPVConstant.hh"

#include "PostAnalysisCanvas.hh"
#include "Corrections.hh"

#include <TH1D.h>
#include <TF1.h>

#include <QRegExp>
#include <QStringList>
#include <QString>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

#include <QDebug>

MoPVConstant::MoPVConstant(QList<PostAnalysisCanvas*> canvasList, bool modules)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
  , m_modules(modules)
{
  int nMoPVs = canvasList.size();
  setTitle(qPrintable("Constant MoPVs" + QString::number(nMoPVs)));
  TH1D* hist = new TH1D(qPrintable(title()), "", nMoPVs, 0-0.5, nMoPVs-0.5);
  setAxisTitle("module/channel","MoPV");

  for (int i = 0; i < nMoPVs; ++i) {
    PostAnalysisCanvas* canvas = canvasList.at(i);
    QString canvasTitle = canvas->name();
    QRegExp regExSearch("(\\dx\\d\\d\\d.)");
    regExSearch.indexIn(canvasTitle);
    QString channelNoString = regExSearch.capturedTexts().at(1);
    int channelNo = channelNoString.toInt(0, 16);
    TH1D* trdHist = canvas->histograms1D().at(0);

    hist->GetXaxis()->SetBinLabel(i+1, qPrintable(channelNoString));
    double mopv = 0;
    double mopvErr = 0;
    if (!fitMoPV(trdHist, mopv, mopvErr))
      continue;
    hist->SetBinContent(i+1, mopv);
    hist->SetBinError(i+1, mopvErr);
    QPair<double, double> mopvErrPair(mopv, mopvErr);
    m_mopvs.insert(channelNo, mopvErrPair );
  }
  drawOption(RootPlot::E1X0);
  addHistogram(hist, RootPlot::E1X0);

  //setup widget
  QWidget* secWidget = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(secWidget);
  layout->addStretch();
  m_pushButton = new QPushButton("save tube corrections for normalization");
  layout->addWidget(m_pushButton);
  layout->addStretch();
  m_spinBox = new QDoubleSpinBox();
  m_spinBox->setRange(0.5, 3);
  m_spinBox->setSingleStep(0.01);
  m_spinBox->setValue(1);
  layout->addWidget(m_spinBox);
  layout->addStretch();
  setSecondaryWidget(secWidget);
  connect(m_pushButton, SIGNAL(pressed()), this, SLOT(saveConstants()));
}


bool MoPVConstant::fitMoPV(TH1D* hist, double& mopv, double& mopvErr)
{
  if (hist->GetEntries() < 30)
    return false;
  TF1* fit = new TF1("fitMopv","landau");

  hist->Fit(fit, "QN0R", "", 0.1, 6);

  mopv = fit->GetParameter(1);
  mopvErr = fit->GetParError(1);

  delete fit;
  return true;
}

void MoPVConstant::saveConstants()
{
  saveConstants(m_spinBox->value());
}

void MoPVConstant::saveConstants(double norm)
{
  //calculate mopv mean
  double mopvMean = 0;
  int nMopvs = 0;
  QMap<int, QPair<double, double> >::const_iterator it;
  for (it = m_mopvs.constBegin(); it != m_mopvs.constEnd(); ++it) {
    mopvMean += it.value().first;
    ++nMopvs;
  }
  mopvMean /= nMopvs;

  //get corrections class:
  Corrections* corr = new Corrections();

  for (it = m_mopvs.constBegin(); it != m_mopvs.constEnd(); ++it) {
    QPair<double, double> mopv = it.value();
    if(mopv.second / mopv.first > 0.2 || qAbs(mopv.first-mopvMean)/mopvMean > 0.2) {
      qDebug("NOT saving for 0x%x as mopv is too bad: %f+-%f", it.key(), mopv.first, mopv.second);
    } else {
      if (m_modules) {
        for (int i = 0; i < 16; ++i) {
          double oldScalingFactor = corr->trdScalingFactor(it.key() + i);
          double newScalingFactor = oldScalingFactor * norm/ mopv.first;
          qDebug("saving for 0x%x with mopv=%f+-%f for norm=%f new scaling factor %f was %f", it.key(), mopv.first, mopv.second, norm, newScalingFactor, oldScalingFactor);
          corr->setTrdScalingFactor(it.key() + i, newScalingFactor);
        }
      } else{
        double oldScalingFactor = corr->trdScalingFactor(it.key());
        double newScalingFactor = oldScalingFactor * norm/ mopv.first;
        qDebug("saving for 0x%x with mopv=%f+-%f for norm=%f new scaling factor %f was %f", it.key(), mopv.first, mopv.second, norm, newScalingFactor, oldScalingFactor);
        corr->setTrdScalingFactor(it.key(), newScalingFactor);
      }
    }
  }
}
