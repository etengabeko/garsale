#include "sellerwidget.h"
#include "ui_sellerwidget.h"

#include <QFileDialog>
#include <QString>
#include <QStringList>

namespace garsale {

SellerWidget::SellerWidget(QWidget* parent) :
  QWidget(parent),
  ui_(new Ui::SellerWidget())
{
  ui_->setupUi(this);

  ui_->filesTreeView->setHeaderHidden(true);
  ui_->filesTreeView->setModel(nullptr);

  connect(ui_->cancelPushButton, &QPushButton::clicked, this, &SellerWidget::slotClose);
  connect(ui_->applyPushButton, &QPushButton::clicked, this, &SellerWidget::slotApply);
  connect(ui_->openFilesPushButton, &QPushButton::clicked, this, &SellerWidget::slotOpenFiles);
}

SellerWidget::~SellerWidget()
{
  delete ui_;
  ui_ = nullptr;
}

void SellerWidget::slotClose()
{
  close();
}

void SellerWidget::slotOpenFiles()
{
  QStringList fileNames(QFileDialog::getOpenFileNames());
  if (fileNames.isEmpty() == true) {
    return;
  }

  ui_->filesTreeView->setModel(nullptr);
  filesModel_.clear();
  foreach (const QString& fileName, fileNames) {
    filesModel_.appendRow(new QStandardItem(fileName));
  }
  ui_->filesTreeView->setModel(&filesModel_);
}

void SellerWidget::slotApply()
{
  //TODO
}

} // garsale
