#ifndef GARSALE_UI_SELLERWIDGET_H
#define GARSALE_UI_SELLERWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

class QHeaderView;
template<typename T> class QList;

namespace Ui {
  class SellerWidget;
} // Ui

namespace garsale {

class SellerGoods;

class SellerWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SellerWidget(QWidget* parent = 0);
  ~SellerWidget();

  enum class SellersHeaderColumns
  {
    ID_OR_LABEL = 0,
    NICKNAME_OR_SIZE,
    NAME_OR_PRICE,
    PHONE_OR_BARCODE,
    EMAIL,
    PAYMENT_KIND,
    PAYMENT_TO,
    INSPECTOR_CODE
  };

private slots:
  void slotOpenFiles();
  void slotClose();
  void slotLoad();
  void slotSave();

private:
  void makeSellersHeader();
  const QList<SellerGoods> loadFromFiles() const;
  const QList<QStandardItem*> sellerGoodsToRowItems(const SellerGoods& sgoods) const;

private:
  Ui::SellerWidget* ui_;

  QStandardItemModel filesModel_;
  QStandardItemModel sellersModel_;

};

} // garsale

#endif // GARSALE_UI_SELLERWIDGET_H
