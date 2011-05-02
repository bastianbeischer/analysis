#include "H2DProjectionPlot.hh"

#include "ProjectionControlWidget.hh"
#include "RootQtWidget.hh"

#include <QVBoxLayout>

#include <TPad.h>
#include <TH2D.h>

H2DProjectionPlot::H2DProjectionPlot() :
  QObject(),
  H2DPlot(),
  m_controlWidget(new ProjectionControlWidget),
  m_projectionWidget(new RootQtWidget)
{
  m_projectionWidget->hide();

  setSecondaryWidget(new QWidget);
  QVBoxLayout* layout = new QVBoxLayout(secondaryWidget());
  layout->addWidget(m_controlWidget);
  layout->addWidget(m_projectionWidget);

  connect(m_controlWidget, SIGNAL(showProjectionChanged(int)), this, SLOT(toggleProjectionWidget(int)));
}

H2DProjectionPlot::~H2DProjectionPlot()
{
}

void H2DProjectionPlot::toggleProjectionWidget(int state)
{
  state ? m_projectionWidget->show() : m_projectionWidget->hide();
}

void H2DProjectionPlot::positionChanged(double x, double y)
{
  RootQtWidget* widget = projectionWidget();
  if (widget->isVisible()) {
    int bin = histogram()->FindBin(x,y);
    int binx, biny, binz;
    histogram()->GetBinXYZ(bin, binx, biny, binz);
    int nChannels = 32;
    TH1D* proj = histogram()->ProjectionY(qPrintable(title() + "_pr"), binx - nChannels/2, binx + nChannels/2);
    TVirtualPad* prevPad = gPad;
    TCanvas* can = widget->GetCanvas();
    can->cd();
    can->Clear();
    proj->Draw();
    can->Modified();
    can->Update();
    prevPad->cd();
  }
}
