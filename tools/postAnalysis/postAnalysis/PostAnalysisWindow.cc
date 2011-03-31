#include "PostAnalysisWindow.hh"
#include "PostAnalysisCanvas.hh"
#include "PostAnalysisPlot.hh"
#include "RootStyle.hh"

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
  , m_canvases()
  , m_plots()
  , m_ui(new Ui_postAnalysisWindow)
{
  m_ui->setupUi(this);
  connect(m_ui->canvasListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectCanvas(QListWidgetItem*)));
  connect(m_ui->canvasListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectCanvas(QListWidgetItem*, QListWidgetItem*)));
  connect(m_ui->plotListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPlot(QListWidgetItem*)));
  connect(m_ui->plotListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
    this, SLOT(selectPlot(QListWidgetItem*, QListWidgetItem*)));
  connect(m_ui->plotOptionComboBox, SIGNAL(currentIndexChanged(const QString&)),
    this, SLOT(plotOptionComboBoxCurrentIndexChanged(const QString&)));
  connect(m_ui->saveCanvasButton, SIGNAL(clicked()), this, SLOT(savePlotsButtonClicked()));
  connect(m_ui->saveAllCanvasesButton, SIGNAL(clicked()), this, SLOT(savePlotsButtonClicked()));
}

PostAnalysisWindow::~PostAnalysisWindow()
{
  delete m_ui;
}

void PostAnalysisWindow::selectCanvas(QListWidgetItem* item, QListWidgetItem*)
{
  selectCanvas(item);
}

void PostAnalysisWindow::selectCanvas(QListWidgetItem* item)
{
  RootStyle::setPalette(RootStyle::DefaultPalette);
  m_ui->qtWidget->GetCanvas()->cd();
  m_ui->qtWidget->GetCanvas()->Clear();
  m_canvases[m_ui->canvasListWidget->row(item)]->draw(m_ui->qtWidget->GetCanvas());
  plotOptionComboBoxCurrentIndexChanged(m_ui->plotOptionComboBox->currentText());
}

void PostAnalysisWindow::selectPlot(QListWidgetItem* item, QListWidgetItem*)
{
  selectPlot(item);
}

void PostAnalysisWindow::selectPlot(QListWidgetItem* item)
{
  gPad->Clear();
  m_plots[m_ui->plotListWidget->row(item)]->draw(m_ui->qtWidget->GetCanvas());
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

PostAnalysisCanvas* PostAnalysisWindow::addCanvas(TFile* file, const QString& name)
{
  PostAnalysisCanvas* canvas = new PostAnalysisCanvas(file, name);
  m_canvases.append(canvas);
  QListWidgetItem* item = new QListWidgetItem(canvas->name());
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
