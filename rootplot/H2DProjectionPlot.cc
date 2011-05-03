#include "H2DProjectionPlot.hh"

#include "ProjectionControlWidget.hh"
#include "RootQtWidget.hh"

#include <QVBoxLayout>

#include <TPad.h>
#include <TH2D.h>

H2DProjectionPlot::H2DProjectionPlot() :
  QObject(),
  H2DPlot(),
  m_projectionHistX(0),
  m_projectionHistY(0),
  m_controlWidget(new ProjectionControlWidget(this)),
  m_projectionWidget(new RootQtWidget),
  m_type(y),
  m_numberOfBins(32)
{
  m_projectionWidget->hide();

  setSecondaryWidget(new QWidget);
  QVBoxLayout* layout = new QVBoxLayout(secondaryWidget());
  layout->addWidget(m_controlWidget);
  layout->addWidget(m_projectionWidget);
}

H2DProjectionPlot::~H2DProjectionPlot()
{
  delete m_projectionHistX;
  delete m_projectionHistY;
}

void H2DProjectionPlot::setProjectionWidgetState(bool state)
{
  state ? m_projectionWidget->show() : m_projectionWidget->hide();
}

void H2DProjectionPlot::setProjectionType(ProjectionType type)
{
  m_type = type;
}

void H2DProjectionPlot::setNumberOfBins(int bins)
{
  m_numberOfBins = bins;
}

void H2DProjectionPlot::positionChanged(double posX, double posY)
{
  RootQtWidget* widget = projectionWidget();
  if (widget->isVisible()) {
    int bin = histogram()->FindBin(posX,posY);
    int binx, biny, binz;
    histogram()->GetBinXYZ(bin, binx, biny, binz);
    TH1D* proj = 0;
    switch(m_type) {
    case (x): 
      m_projectionHistX = histogram()->ProjectionX(qPrintable(title() + "_px"), biny - m_numberOfBins/2, biny + m_numberOfBins/2);
      proj = m_projectionHistX;
      break;
    case (y) :
      m_projectionHistY = histogram()->ProjectionY(qPrintable(title() + "_py"), binx - m_numberOfBins/2, binx + m_numberOfBins/2);
      proj = m_projectionHistY;
      break;
    default:
      Q_ASSERT(false);
      break;
    }
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
