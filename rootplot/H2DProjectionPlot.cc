#include "H2DProjectionPlot.hh"

#include "ProjectionControlWidget.hh"
#include "RootQtWidget.hh"

#include <QVBoxLayout>
#include <QSpinBox>
#include <QDebug>

#include <TPad.h>
#include <TH2D.h>
#include <TLine.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TF1.h>

H2DProjectionPlot::H2DProjectionPlot()
  : QObject()
  , H2DPlot()
  , m_projectionHistX(0)
  , m_projectionHistY(0)
  , m_controlWidget(new ProjectionControlWidget(this))
  , m_projectionWidget(new RootQtWidget)
  , m_type(NoProjection)
  , m_line(0)
  , m_bin(-1)
  , m_projectionLatex()
  , m_projectionLegend()
  , m_projectionFunction()
{
  m_projectionWidget->hide();

  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_controlWidget);
  layout->addWidget(m_projectionWidget);
  QWidget* widget = new QWidget;
  widget->setLayout(layout);
  setSecondaryWidget(widget);
}

H2DProjectionPlot::~H2DProjectionPlot()
{
  if (m_projectionHistX)
    delete m_projectionHistX;
  if (m_projectionHistY)
    delete m_projectionHistY;
  if (m_line)
    delete m_line;
}

void H2DProjectionPlot::setProjectionType(ProjectionType type)
{
  m_projectionWidget->setVisible(type != NoProjection);
  m_type = type;
  if (m_line) {
    delete m_line;
    m_line = 0;
    gPad->Modified();
    gPad->Update();
  }
}

TH1D* H2DProjectionPlot::projection()
{
  switch(m_type) {
    case ProjectionOnX:
      return m_projectionHistX;
    case ProjectionOnY:
      return m_projectionHistY;
    default:
      Q_ASSERT(false);
      return 0;
  }
}

void H2DProjectionPlot::updateProjection()
{
}

void H2DProjectionPlot::positionChanged(double posX, double posY)
{
  RootQtWidget* widget = projectionWidget();
  if (widget->isVisible()) {
    if (!m_line) {
      m_line = new TLine;
      m_line->Draw();
    }
    bool projectionChanged = false;
    int bin = histogram()->FindBin(posX,posY);
    if (m_bin != bin)
      projectionChanged = true;
    int binx, biny, binz;
    histogram()->GetBinXYZ(bin, binx, biny, binz);
    TH1D* proj = 0;
    double x = qBound(gPad->GetUxmin(), posX, gPad->GetUxmax());
    double y = qBound(gPad->GetUymin(), posY, gPad->GetUymax());
    switch(m_type) {
    case ProjectionOnX:
      if (projectionChanged == true)
        m_projectionHistX = histogram()->ProjectionX(qPrintable(title() + "_px"), biny, biny);
      proj = m_projectionHistX;
      m_line->SetX1(gPad->GetUxmin());
      m_line->SetX2(gPad->GetUxmax());
      m_line->SetY1(y);
      m_line->SetY2(y);
      break;
    case ProjectionOnY:
      if (projectionChanged == true)
        m_projectionHistY = histogram()->ProjectionY(qPrintable(title() + "_py"), binx, binx);
      proj = m_projectionHistY;
      m_line->SetX1(x);
      m_line->SetX2(x);
      m_line->SetY1(gPad->GetUymin());
      m_line->SetY2(gPad->GetUymax());
      break;
    default:
      Q_ASSERT(false);
      break;
    }
    if (projectionChanged == true)
      updateProjection();
    TVirtualPad* prevPad = gPad;
    TCanvas* can = widget->GetCanvas();
    can->cd();
    can->Clear();
    proj->Draw();
    foreach(TLatex* latex, m_projectionLatex)
    latex->Draw("SAME");
    foreach(TLegend* legend, m_projectionLegend)
    legend->Draw("SAME");
    foreach(TF1* function, m_projectionFunction)
    function->Draw("SAME");
    can->Modified();
    can->Update();
    prevPad->cd();
    gPad->Modified();
    gPad->Update();
    m_bin = bin;
  }
}

void H2DProjectionPlot::setLogX(int b)
{
  TVirtualPad* prevPad = gPad;
  TCanvas* can = projectionWidget()->GetCanvas();
  can->cd();
  can->SetLogx(b);
  can->Modified();
  can->Update();
  prevPad->cd();
}

void H2DProjectionPlot::setLogY(int b)
{
  TVirtualPad* prevPad = gPad;
  TCanvas* can = projectionWidget()->GetCanvas();
  can->cd();
  can->SetLogy(b);
  can->Modified();
  can->Update();
  prevPad->cd();
}

void H2DProjectionPlot::setLogZ(int b)
{
  TVirtualPad* prevPad = gPad;
  TCanvas* can = projectionWidget()->GetCanvas();
  can->cd();
  can->SetLogz(b);
  can->Modified();
  can->Update();
  prevPad->cd();
}

void H2DProjectionPlot::addProjectionLatex(TLatex* latex)
{
  Q_ASSERT(latex);
  m_projectionLatex.append(latex);
}

TLatex* H2DProjectionPlot::projectionLatex(int i)
{
  Q_ASSERT(0 <= i && i < m_projectionLatex.size());
  return m_projectionLatex[i];
}

void H2DProjectionPlot::addProjectionLegend(TLegend* legend)
{
  Q_ASSERT(legend);
  legend->SetFillColor(10);
  m_projectionLegend.append(legend);
}

TLegend* H2DProjectionPlot::projectionLegend(int i)
{
  Q_ASSERT(0 <= i && i < m_projectionLegend.size());
  return m_projectionLegend[i];
}

void H2DProjectionPlot::addProjectionFunction(TF1* function)
{
  Q_ASSERT(function);
  m_projectionFunction.append(function);
}

TF1* H2DProjectionPlot::projectionFunction(int i)
{
  Q_ASSERT(0 <= i && i < m_projectionFunction.size());
  return m_projectionFunction[i];
}

