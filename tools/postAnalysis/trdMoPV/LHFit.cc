#include "LHFit.hh"

#include "PostAnalysisCanvas.hh"
#include "H1DPlot.hh"
#include "TRDLikelihoods.hh"

#include <TH1D.h>
#include <TF1.h>
#include <TROOT.h>
#include <TCanvas.h>

#include <iostream>

#include <QString>
#include <QList>
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>

LHFit::LHFit(PostAnalysisCanvas* canvas, int layer)
  : QObject()
  , PostAnalysisPlot()
  , H1DPlot()
  , m_fit()
  , m_trdLHs(TRDLikelihoods::instance())
  , m_buttonSave(0)
  , m_layer(layer)
{
  if (m_layer < 0 || m_layer > 8)
    m_layer = -1;

  gROOT->cd();
  setDrawOption(H1DPlot::BLANK);

  if (canvas != 0) {
    TH1D* histClone = (TH1D*) canvas->histograms1D().at(0)->Clone();
    histClone->Sumw2();
    histClone->SetStats(kFALSE);
    histClone->Scale(1./histClone->Integral("width"));
    histClone->SetMarkerStyle(0);
    addHistogram(histClone, H1DPlot::BLANK);
  }
  histogram()->GetYaxis()->SetRangeUser(10e-6, 1.1*histogram()->GetMaximum());

  setTitle(canvas->name());
  setAxisTitle(histogram()->GetXaxis()->GetTitle(), histogram()->GetYaxis()->GetTitle());

  //setup widget
  QWidget* secWidget = new QWidget();
  QHBoxLayout* layout = new QHBoxLayout(secWidget);
  layout->addStretch();
  QPushButton* buttonFitNonTR = new QPushButton("fit a non TR function");
  if (layer == -1) {
    layout->addWidget(buttonFitNonTR);
    layout->addStretch();
  }
  QPushButton* buttonFitTR = new QPushButton("fit a TR function");
  layout->addWidget(buttonFitTR);
  layout->addStretch();
  m_buttonSave = new QPushButton("save");
  m_buttonSave->setEnabled(false);
  layout->addWidget(m_buttonSave);
  layout->addStretch();
  setSecondaryWidget(secWidget);
  connect(buttonFitNonTR, SIGNAL(pressed()), this, SLOT(fitNonTRFunction()));
  connect(buttonFitTR, SIGNAL(pressed()), this, SLOT(fitTRFunction()));
  connect(buttonFitNonTR, SIGNAL(pressed()), this, SLOT(updateSaveButton()));
  connect(buttonFitTR, SIGNAL(pressed()), this, SLOT(updateSaveButton()));
  connect(m_buttonSave, SIGNAL(pressed()), this, SLOT(saveFit()));
}

void LHFit::fitTRFunction()
{
  if (m_fit)
    delete m_fit;
  m_fit = m_trdLHs->prototypeLHFunctionTR();
  histogram()->Fit(m_fit);
  gPad->Modified();
  gPad->Update();
}

void LHFit::fitNonTRFunction()
{
  if (m_fit)
    delete m_fit;
  m_fit = m_trdLHs->prototypeLHFunctionNonTR();
  histogram()->Fit(m_fit);
  gPad->Modified();
  gPad->Update();
}

void LHFit::saveFit()
{
  if (!m_fit)
    return;
  bool fitIsTRFun = (QString(m_fit->GetName()) == QString("TRDLikelihoodTR"));
  if (fitIsTRFun) {
    if (m_layer == -1)
      m_trdLHs->setLHFunctionTR(m_fit);
    else
      m_trdLHs->setLHFunctionTRLayer(m_layer, m_fit);
  } else
    m_trdLHs->setLHFunctionNonTR(m_fit);
}

void LHFit::updateSaveButton()
{
  if (!m_fit)
    m_buttonSave->setEnabled(false);
  else
    m_buttonSave->setEnabled(true);
}
