#include "RootQtWidget.hh"

#include <QMouseEvent>

RootQtWidget::RootQtWidget(QWidget* parent)
  : TQtWidget(parent)
  , m_inhibitResizeEvent(false)
  , m_aspectRatio(-1.)
{
  setContentsMargins(0, 0, 0, 0);
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

void RootQtWidget::setAspectRatio(double ratio)
{
  m_aspectRatio = ratio;
  updateGeometry();
  adjustSize();
}

void RootQtWidget::resizeEvent(QResizeEvent* event)
{
  if (m_inhibitResizeEvent || m_aspectRatio <= 0.) {
    TQtWidget::resizeEvent(event);
    return;
  }
  m_inhibitResizeEvent = true;
  int w = event->size().width();
  int h = event->size().height();
  if (h * m_aspectRatio < w) {
    resize(h * m_aspectRatio, h);
  } else {
    resize(w, w / m_aspectRatio);
  }
  m_inhibitResizeEvent = false;
}

QSize RootQtWidget::sizeHint() const
{
  return QSize(300, 300);
}
