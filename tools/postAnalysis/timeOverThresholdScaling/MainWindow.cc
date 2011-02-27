#include "MainWindow.hh"

#include "PostAnalysisPlot.hh"
#include "timeOverThresholdScaling.hh"
#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_plots()
{
  m_ui.setupUi(this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::canvasListWidgetCurrentRowChanged(int i)
{
  m_ui.qtWidget->GetCanvas()->cd();
  gPad->Clear();
  m_canvases[i]->DrawClonePad();
}

void MainWindow::plotListWidgetCurrentRowChanged(int i)
{
  gPad->Clear();
  m_plots[i]->draw(m_ui.qtWidget->GetCanvas());
  gPad->Modified();
  gPad->Update();
}

void MainWindow::setAnalysisFile(const QString& file)
{
  m_analysisFile = file;
  setupAnalysis();
}

TCanvas* MainWindow::addCanvas(TFile* file, const char* name)
{
  TCanvas* canvas = static_cast<TCanvas*>(file->Get(name)->Clone());
  m_canvases.append(canvas);
  QListWidgetItem* item = new QListWidgetItem(canvas->GetName());
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  m_ui.canvasListWidget->addItem(item);
  return canvas;
}

TCanvas* MainWindow::addCanvas(TFile* file, const QString name)
{
  return addCanvas(file, name.toStdString().c_str());
}

void MainWindow::addPlot(PostAnalysisPlot* plot)
{
  m_plots.append(plot);
  QListWidgetItem* item = new QListWidgetItem(plot->title());
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  m_ui.plotListWidget->addItem(item);
}

void MainWindow::setupAnalysis()
{
  TCanvas* canvas = 0;
  TFile file(qPrintable(m_analysisFile));
  gROOT->cd();
  for (int i = 0x8000; i <= 0x803f; i++) {
    const QString canvasName = "time over threshold temperature correlation 0x"+QString::number(i, 16)+" canvas";
    canvas = addCanvas(&file, canvasName);
    addPlot(new TimeOverThresholdScaling(canvas, i));
  }
  file.Close();
  connect(m_ui.canvasListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(canvasListWidgetCurrentRowChanged(int)));
  connect(m_ui.plotListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(plotListWidgetCurrentRowChanged(int)));
}

void MainWindow::savePlotsButtonClicked()
{
  QString fileEnding;
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "svg;;pdf;;root;;png", &fileEnding);
  if (fileName.isEmpty())
    return;
  fileEnding.prepend('.');
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  m_ui.qtWidget->GetCanvas()->SaveAs(qPrintable(fileName));
}

void MainWindow::saveAllPlotsButtonClicked()
{
  QFileDialog dialog(this, "save all canvases displayed", ".");
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  if (dialog.exec())
    for (int i = 0; i < m_ui.plotListWidget->count(); ++i) {
      m_ui.plotListWidget->setCurrentRow(i);
      QString directoryName = dialog.selectedFiles().first();
      m_ui.qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui.titleLabel->text() + ".svg"));
      m_ui.qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui.titleLabel->text() + ".pdf"));
      m_ui.qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui.titleLabel->text() + ".root"));
      m_ui.qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui.titleLabel->text() + ".png"));
    }
}
