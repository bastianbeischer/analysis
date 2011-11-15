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
  , m_projection(0)
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
  if (m_projection)
    delete m_projection;
  if (m_line)
    delete m_line;
}

void H2DProjectionPlot::setProjectionType(ProjectionType type)
{
  m_projectionWidget->setVisible(type != NoProjection);
  if (m_line) {
    delete m_line;
    m_line = 0;
  }
  m_type = type;
  m_bin = -1;
  gPad->Modified();
  gPad->Update();
}

TH1D* H2DProjectionPlot::projection()
{
  return m_projection;
}

void H2DProjectionPlot::updateProjection()
{
}

void H2DProjectionPlot::positionChanged(double posX, double posY)
{
  RootQtWidget* widget = projectionWidget();
  if (!widget->isVisible())
    return;
  if (!m_line) {
    m_line = new TLine;
    m_line->Draw();
  }

  int bin = -1;
  switch (m_type) {
    case ProjectionOnX:
      bin = histogram()->GetYaxis()->FindBin(posY);
      break;
    case ProjectionOnY:
      bin = histogram()->GetXaxis()->FindBin(posX);
      break;
    default:
      Q_ASSERT(false);
      break;
  }
  if (m_bin == bin)
    return;
  m_bin = bin;

  if (m_projection)
    delete m_projection;
  double xy = 0;
  switch (m_type) {
    case ProjectionOnX:
      m_projection = histogram()->ProjectionX(qPrintable(title() + "_px"), bin, bin);
      m_line->SetX1(gPad->GetUxmin());
      m_line->SetX2(gPad->GetUxmax());
      xy = qBound(gPad->GetUymin(), histogram()->GetYaxis()->GetBinCenter(bin), gPad->GetUymax());
      m_line->SetY1(xy);
      m_line->SetY2(xy);
      break;
    case ProjectionOnY:
      m_projection = histogram()->ProjectionY(qPrintable(title() + "_py"), bin, bin);
      xy = qBound(gPad->GetUxmin(), histogram()->GetXaxis()->GetBinCenter(bin), gPad->GetUxmax());
      m_line->SetX1(xy);
      m_line->SetX2(xy);
      m_line->SetY1(gPad->GetUymin());
      m_line->SetY2(gPad->GetUymax());
      break;
    default:
      Q_ASSERT(false);
      break;
  }
  updateProjection();
  TVirtualPad* prevPad = gPad;
  TCanvas* can = widget->GetCanvas();
  can->cd();
  can->Clear();
  m_projection->Draw();
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

