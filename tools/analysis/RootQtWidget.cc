#include "RootQtWidget.hh"

#include <QMouseEvent>

RootQtWidget::RootQtWidget(QWidget* parent) :
  TQtWidget(parent)
{
}

RootQtWidget::~RootQtWidget()
{
}

void RootQtWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::MidButton)
    emit(unzoomButtonPressed());

  TQtWidget::mousePressEvent(event);
}

void RootQtWidget::mouseMoveEvent(QMouseEvent* event)
{
  double x = gPad->AbsPixeltoX(event->x());
  double y = gPad->AbsPixeltoY(event->y());
  emit(positionChanged(x, y));
  TQtWidget::mouseMoveEvent(event);
}

void RootQtWidget::updateCanvas()
{
  gPad->Modified();
  gPad->Update();
}

void RootQtWidget::setGrid(int b)
{
  gPad->SetGridx(b);
  gPad->SetGridy(b);
  updateCanvas();
}

void RootQtWidget::setLogX(int b)
{
  gPad->SetLogx(b);
  updateCanvas();
}

void RootQtWidget::setLogY(int b)
{
  gPad->SetLogy(b);
  updateCanvas();
}

void RootQtWidget::setLogZ(int b)
{
  gPad->SetLogz(b);
  updateCanvas();
}
