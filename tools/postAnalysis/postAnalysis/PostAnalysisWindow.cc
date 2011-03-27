#include "PostAnalysisWindow.hh"
#include "PostAnalysisPlot.hh"

#include "ui_PostAnalysisWindow.h"

#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH2.h>
#include <TF2.h>
#include <TQtWidget.h>

#include <QDebug>
#include <QFileDialog>

PostAnalysisWindow::PostAnalysisWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_plots()
  , m_ui(new Ui_postAnalysisWindow)
{
  m_ui->setupUi(this);
  connect(m_ui->canvasListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(canvasListWidgetCurrentRowChanged(int)));
  connect(m_ui->plotListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(plotListWidgetCurrentRowChanged(int)));
  connect(m_ui->plotOptionComboBox, SIGNAL(currentIndexChanged(const QString&)),
    this, SLOT(plotOptionComboBoxCurrentIndexChanged(const QString&)));
}

PostAnalysisWindow::~PostAnalysisWindow()
{
  delete m_ui;
}

void PostAnalysisWindow::canvasListWidgetCurrentRowChanged(int i)
{
  m_ui->qtWidget->GetCanvas()->cd();
  gPad->Clear();
  m_canvases[i]->DrawClonePad();
  plotOptionComboBoxCurrentIndexChanged(m_ui->plotOptionComboBox->currentText());
}

void PostAnalysisWindow::plotListWidgetCurrentRowChanged(int i)
{
  gPad->Clear();
  m_plots[i]->draw(m_ui->qtWidget->GetCanvas());
  plotOptionComboBoxCurrentIndexChanged(m_ui->plotOptionComboBox->currentText());
}

void PostAnalysisWindow::plotOptionComboBoxCurrentIndexChanged(const QString& option)
{
  TCanvas* canvas = m_ui->qtWidget->GetCanvas();
  for (int i = 0; i < canvas->GetListOfPrimitives()->GetSize(); ++i) {
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TH2D"))
      (static_cast<TH2*>(canvas->GetListOfPrimitives()->At(i)))->SetDrawOption(qPrintable(option));
    if (!strcmp(canvas->GetListOfPrimitives()->At(i)->ClassName(), "TF2"))
      (static_cast<TF2*>(canvas->GetListOfPrimitives()->At(i)))->SetDrawOption(qPrintable(option));
  }
  gPad->Modified();
  gPad->Update();
}

void PostAnalysisWindow::setAnalysisFile(const QString& file)
{
  m_analysisFile = file;
  setupAnalysis();
}

TCanvas* PostAnalysisWindow::addCanvas(TFile* file, const char* name)
{
  TCanvas* canvas = static_cast<TCanvas*>(file->Get(name)->Clone());
  m_canvases.append(canvas);
  QListWidgetItem* item = new QListWidgetItem(canvas->GetName());
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  m_ui->canvasListWidget->addItem(item);
  return canvas;
}

void PostAnalysisWindow::addPlot(PostAnalysisPlot* plot)
{
  m_plots.append(plot);
  QListWidgetItem* item = new QListWidgetItem(plot->title());
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  m_ui->plotListWidget->addItem(item);
}

void PostAnalysisWindow::savePlotsButtonClicked()
{
  QString fileEnding;
  QString fileName = QFileDialog::getSaveFileName(this, "save current canvas", ".", "svg;;pdf;;root;;png", &fileEnding);
  if (fileName.isEmpty())
    return;
  fileEnding.prepend('.');
  if (!fileName.endsWith(fileEnding))
    fileName.append(fileEnding);
  m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(fileName));
}

void PostAnalysisWindow::saveAllPlotsButtonClicked()
{
  QFileDialog dialog(this, "save all canvases displayed", ".");
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  if (dialog.exec())
    for (int i = 0; i < m_ui->plotListWidget->count(); ++i) {
      m_ui->plotListWidget->setCurrentRow(i);
      QString directoryName = dialog.selectedFiles().first();
      m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui->titleLabel->text() + ".svg"));
      m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui->titleLabel->text() + ".pdf"));
      m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui->titleLabel->text() + ".root"));
      m_ui->qtWidget->GetCanvas()->SaveAs(qPrintable(directoryName + '/' + m_ui->titleLabel->text() + ".png"));
    }
}
