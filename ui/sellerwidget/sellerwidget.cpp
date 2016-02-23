#include "sellerwidget.h"
#include "ui_sellerwidget.h"

#include "sellerparser/sellergoods.h"
#include "sellerparser/sellerimporter.h"

#include <QDateTime>
#include <QFileDialog>
#include <QHeaderView>
#include <QList>
#include <QString>
#include <QStringList>

namespace garsale {

SellerWidget::SellerWidget(QWidget* parent) :
  QWidget(parent),
  ui_(new Ui::SellerWidget())
{
  ui_->setupUi(this);

  ui_->filesTreeView->setHeaderHidden(true);
  ui_->filesTreeView->setModel(&filesModel_);

  ui_->sellersTreeView->setHeaderHidden(false);
  ui_->sellersTreeView->setModel(&sellersModel_);
  makeSellersHeader();

  connect(ui_->cancelPushButton, &QPushButton::clicked, this, &SellerWidget::slotClose);
  connect(ui_->loadPushButton, &QPushButton::clicked, this, &SellerWidget::slotLoad);
  connect(ui_->savePushButton, &QPushButton::clicked, this, &SellerWidget::slotSave);
  connect(ui_->openFilesPushButton, &QPushButton::clicked, this, &SellerWidget::slotOpenFiles);
}

SellerWidget::~SellerWidget()
{
  delete ui_;
  ui_ = nullptr;
}

void SellerWidget::makeSellersHeader()
{
  QAbstractItemModel* headerModel = ui_->sellersTreeView->header()->model();
  if (headerModel != nullptr) {
    headerModel->setHeaderData(static_cast<int>(SellersHeaderColumns::ID_OR_LABEL), Qt::Horizontal, tr("ID/Label"), Qt::DisplayRole);
    headerModel->setHeaderData(static_cast<int>(SellersHeaderColumns::NICKNAME_OR_SIZE), Qt::Horizontal, tr("Nickname/Size"), Qt::DisplayRole);
    headerModel->setHeaderData(static_cast<int>(SellersHeaderColumns::NAME_OR_PRICE), Qt::Horizontal, tr("Name/Price"), Qt::DisplayRole);
    headerModel->setHeaderData(static_cast<int>(SellersHeaderColumns::PHONE_OR_BARCODE), Qt::Horizontal, tr("Phone/Barcode"), Qt::DisplayRole);
    headerModel->setHeaderData(static_cast<int>(SellersHeaderColumns::EMAIL), Qt::Horizontal, tr("Email"), Qt::DisplayRole);
    headerModel->setHeaderData(static_cast<int>(SellersHeaderColumns::PAYMENT_KIND), Qt::Horizontal, tr("Payment kind"), Qt::DisplayRole);
    headerModel->setHeaderData(static_cast<int>(SellersHeaderColumns::PAYMENT_TO), Qt::Horizontal, tr("Paymanet to"), Qt::DisplayRole);
    headerModel->setHeaderData(static_cast<int>(SellersHeaderColumns::INSPECTOR_CODE), Qt::Horizontal, tr("Inspector code"), Qt::DisplayRole);
  }
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

void SellerWidget::slotLoad()
{
  ui_->sellersTreeView->setModel(nullptr);
  sellersModel_.clear();

  foreach (const SellerGoods& sgood, loadFromFiles()) {
    QList<QStandardItem*> row = sellerGoodsToRowItems(sgood);
    if (row.isEmpty() == false) {
      sellersModel_.appendRow(row);
    }
  }

  ui_->sellersTreeView->setModel(&sellersModel_);
  makeSellersHeader();
}

const QList<SellerGoods> SellerWidget::loadFromFiles() const
{
  QList<SellerGoods> result;
  for (int i = 0, sz = filesModel_.rowCount(); i < sz; ++i) {
    result.append(SellerImporter::loadFromFile(filesModel_.item(i,0)->text()));
  }
  return result;
}

const QList<QStandardItem*> SellerWidget::sellerGoodsToRowItems(const SellerGoods& sgoods) const
{
  QList<QStandardItem*> result;
  QStandardItem* root = new QStandardItem(sgoods.id);
  result << root
         << new QStandardItem(sgoods.nickname)
         << new QStandardItem(sgoods.name)
         << new QStandardItem(sgoods.phone)
         << new QStandardItem(sgoods.email)
         << new QStandardItem(sgoods.payment_kind)
         << new QStandardItem(sgoods.payment_to)
         << new QStandardItem(sgoods.inspector_code);
  for (auto it = sgoods.goods.begin(), end = sgoods.goods.end(); it != end; ++it) {
    QList<QStandardItem*> rowGood;
    rowGood << new QStandardItem(it->label)
            << new QStandardItem(it->size)
            << new QStandardItem(it->price)
//            << new QStandardItem(it->discount)
//            << new QStandardItem(it->charity)
            << new QStandardItem(it->barcode);
    root->appendRow(rowGood);
  }
  return result;
}

void SellerWidget::slotSave()
{
  ui_->errorTextEdit->setPlainText(ui_->errorTextEdit->toPlainText()
                                   + "\n" + QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")
                                   + " " + tr("Start saving:")
                                   + "\n");
  QString result;
  for (int i = 0, sz = sellersModel_.rowCount(); i < sz; ++i) {
    bool ok = SellerImporter::saveSellerGoods(rowItemsToSellerGoods(i));
    QString fileName = filesModel_.item(i) != nullptr ? filesModel_.item(i)->text() : tr("Unknown error: filename not found");
    result += tr(QString("Load %1 [ %2 ]\n")
                 .arg(fileName)
                 .arg((ok ? tr("True") : tr("False")))
                 .toStdString().c_str());
  }

  ui_->errorTextEdit->setPlainText(ui_->errorTextEdit->toPlainText()
                                   + "\n" + result
                                   + "\n" + QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")
                                   + " " + tr("Finish saving")
                                   + "\n");
}

const SellerGoods SellerWidget::rowItemsToSellerGoods(int rowNumber) const
{
  SellerGoods result;
  QStandardItem* idItem = sellersModel_.item(rowNumber, static_cast<int>(SellersHeaderColumns::ID_OR_LABEL));
  if (idItem != nullptr) {
    result.id = idItem->text();
  }
  QStandardItem* item = sellersModel_.item(rowNumber, static_cast<int>(SellersHeaderColumns::NICKNAME_OR_SIZE));
  if (item != nullptr) {
    result.nickname = item->text();
  }
  item = sellersModel_.item(rowNumber, static_cast<int>(SellersHeaderColumns::NAME_OR_PRICE));
  if (item != nullptr) {
    result.name = item->text();
  }
  item = sellersModel_.item(rowNumber, static_cast<int>(SellersHeaderColumns::PHONE_OR_BARCODE));
  if (item != nullptr) {
    result.phone = item->text();
  }
  item = sellersModel_.item(rowNumber, static_cast<int>(SellersHeaderColumns::EMAIL));
  if (item != nullptr) {
    result.email = item->text();
  }
  item = sellersModel_.item(rowNumber, static_cast<int>(SellersHeaderColumns::PAYMENT_KIND));
  if (item != nullptr) {
    result.payment_kind = item->text();
  }
  item = sellersModel_.item(rowNumber, static_cast<int>(SellersHeaderColumns::PAYMENT_TO));
  if (item != nullptr) {
    result.payment_to = item->text();
  }
  item = sellersModel_.item(rowNumber, static_cast<int>(SellersHeaderColumns::INSPECTOR_CODE));
  if (item != nullptr) {
    result.inspector_code = item->text();
  }

  for (int i = 0, sz = sellersModel_.rowCount(idItem->index()); i < sz; ++i) {
    Good g;
    QStandardItem* gitem = idItem->child(i, static_cast<int>(SellersHeaderColumns::ID_OR_LABEL));
    if (gitem != nullptr) {
      g.label = gitem->text();
    }
    gitem = idItem->child(i, static_cast<int>(SellersHeaderColumns::NICKNAME_OR_SIZE));
    if (gitem != nullptr) {
      g.size = gitem->text();
    }
    gitem = idItem->child(i, static_cast<int>(SellersHeaderColumns::NAME_OR_PRICE));
    if (gitem != nullptr) {
      g.price = gitem->text();
    }
    gitem = idItem->child(i, static_cast<int>(SellersHeaderColumns::PHONE_OR_BARCODE));
    if (gitem != nullptr) {
      g.barcode = gitem->text();
    }
    if (g.isValid() == true) {
      result.goods.append(g);
    }
  }

  return result;
}

} // garsale
